// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/step_tpk_patch_icons.h"

#include <boost/filesystem/path.hpp>
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
                    const bf::path& root_path, uid_t uid) {
  std::vector<bf::path> locations;
  // FIXME: icons for preloaded apps should also be moved to "shared/res"
  bf::path system_location = bf::path(getIconPath(uid)) / filename;
  bf::path small_system_location =
      bf::path(getIconPath(uid)) / "default" / "small" / filename;
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

}  // namespace

namespace tpk {
namespace filesystem {

common_installer::Step::Status StepTpkPatchIcons::process() {
  bf::path common_icon_location = context_->pkg_path.get() / "shared" / "res";
  bs::error_code error;
  bf::create_directories(common_icon_location, error);
  for (application_x* app :
      GListRange<application_x*>(context_->manifest_data.get()->application)) {
    if (strcmp(app->type, "capp") != 0)
      continue;
    if (app->icon) {
      icon_x* icon = reinterpret_cast<icon_x*>(app->icon->data);
      bf::path icon_text(icon->text);
      if (!bf::exists(icon->text)) {
        bf::path source = LocateIcon(icon_text.filename(),
                                     context_->pkgid.get(),
                                     context_->root_application_path.get(),
                                     context_->uid.get());
        if (!source.empty()) {
          LOG(DEBUG) << "Fix location of icon: " << source << " to: "
                     << icon_text;
          if (!common_installer::CopyFile(source, icon_text)) {
            return Status::ICON_ERROR;
          }
        }
      }
    }
  }
  return Status::OK;
}

}  // namespace filesystem
}  // namespace tpk
