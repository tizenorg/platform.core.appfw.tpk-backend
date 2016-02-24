// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/step_tpk_patch_icons.h"

#include <boost/filesystem/operations.hpp>
#include <boost/system/error_code.hpp>
#include <common/utils/file_util.h>
#include <common/utils/glist_range.h>
#include <pkgmgr-info.h>

#include <cstring>
#include <string>
#include <vector>

namespace bf = boost::filesystem;
namespace bs = boost::system;

namespace {

// This function checks for alternative locations of icon file of tpk package
bf::path LocateIcon(const bf::path& filename, const std::string& pkgid,
                    const bf::path& root_path, uid_t uid, bool is_preload) {
  std::vector<bf::path> locations;
  // FIXME: icons for preloaded apps should also be moved to "shared/res"
  bf::path system_location = bf::path(getIconPath(uid, is_preload)) / filename;
  bf::path small_system_location =
      bf::path(getIconPath(uid, is_preload)) / "default" / "small" / filename;
  bf::path res_icons_location = root_path / pkgid / "res" / "icons" / filename;

  locations.push_back(system_location);
  locations.push_back(small_system_location);
  locations.push_back(res_icons_location);

  for (auto& location : locations) {
    if (bf::exists(location)) {
      return location;
    }
  }
  return {};
}

bool IsTpkApp(application_x* app) {
  return strcmp(app->type, "capp") == 0 || strcmp(app->type, "jsapp") == 0;
}

}  // namespace

namespace tpk {
namespace filesystem {

common_installer::Step::Status StepTpkPatchIcons::ProcessIconOutsidePackage(
    const bf::path& common_icon_location, const bf::path& icon_text,
    application_x* app, icon_x* icon) {
  bf::path destination = common_icon_location / app->appid;
  if (!icon_text.extension().empty()) {
    destination += icon_text.extension();
  } else {
    destination += ".png";
  }
  if (!common_installer::CopyFile(icon_text, destination)) {
    return Status::ICON_ERROR;
  }
  free(const_cast<char*>(icon->text));
  icon->text = strdup(destination.c_str());
  return Status::OK;
}

common_installer::Step::Status StepTpkPatchIcons::FixIconLocation(
    const bf::path& icon_text) {
  bf::path source = LocateIcon(icon_text.filename(),
                               context_->pkgid.get(),
                               context_->root_application_path.get(),
                               context_->uid.get(),
                               context_->is_preload_request.get());
  if (!source.empty()) {
    LOG(DEBUG) << "Fix location of icon: " << source << " to: " << icon_text;
    if (!common_installer::CopyFile(source, icon_text)) {
      return Status::ICON_ERROR;
    }
  }
  return Status::OK;
}

common_installer::Step::Status StepTpkPatchIcons::process() {
  bf::path common_icon_location = context_->pkg_path.get() / "shared" / "res";
  bs::error_code error;
  bf::create_directories(common_icon_location, error);
  for (application_x* app :
      GListRange<application_x*>(context_->manifest_data.get()->application)) {
    if (!IsTpkApp(app))
      continue;
    if (app->icon) {
      icon_x* icon = reinterpret_cast<icon_x*>(app->icon->data);
      bf::path icon_text(icon->text);
      if (icon_text.parent_path() != common_icon_location) {
        // if location of icon was absolute and icon is not in common icon
        // location, we just need to copy it and replace icon->text to insert
        // correct information into database
        Status status = ProcessIconOutsidePackage(common_icon_location,
                                                  icon_text, app, icon);

        if (status != Status::OK) {
          // ignore copying result in offline for preload apps
          if (strcmp(context_->manifest_data.get()->preload, "true") == 0) {
            // FIXME: there is no destructor of icon_x exposed but we need to
            // remove it. Removing whole list as no valid copy made of any icon.
            g_list_free_full(app->icon, [](gpointer data) {
                  icon_x* icon = reinterpret_cast<icon_x*>(data);
                  free(const_cast<char*>(icon->text));
                  free(const_cast<char*>(icon->lang));
                  free(const_cast<char*>(icon->section));
                  free(const_cast<char*>(icon->size));
                  free(const_cast<char*>(icon->resolution));
                  free(icon);
                });
            app->icon = nullptr;
          } else {
            return status;
          }
        }
      } else {
        // look for icon in different location if it doesn't exist
        if (!bf::exists(icon->text)) {
          Status status = FixIconLocation(icon_text);
          if (status != Status::OK)
            return status;
        }
      }
    }
  }
  return Status::OK;
}

}  // namespace filesystem
}  // namespace tpk
