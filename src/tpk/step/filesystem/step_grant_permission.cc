// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/filesystem/step_grant_permission.h"
#include "tpk/step/filesystem/step_tpk_prepare_package_directory.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/system/error_code.hpp>

#include <common/utils/file_util.h>

namespace bf = boost::filesystem;
namespace bs = boost::system;
namespace ci = common_installer;

namespace tpk {
namespace filesystem {

ci::Step::Status StepTpkGrantPermission::precheck() {
  if (context_->pkgid.get().empty()) {
    LOG(ERROR) << "Pkgid is not set";
    return Status::INVALID_VALUE;
  }
  return Status::OK;
}

ci::Step::Status StepTpkGrantPermission::process() {
  Status status = Status::OK;
  context_->pkg_path.set(
      context_->root_application_path.get() / context_->pkgid.get());

  bf::path app_root = context_->pkg_path.get();
  for (auto& entry :
      boost::make_iterator_range(bf::directory_iterator(app_root), {})) {
    auto path = entry.path();

    // skip path, which is related to mount or directory installer creates
    if (bf::is_directory(path) &&
        (path.filename() == ".mmc" || path.filename() == ".pkg" ||
        path.filename() == "tep"))
      continue;

    // if mount-install, apply to extracted directories only
    if (context_->request_type.get() == ci::RequestType::MountInstall ||
        context_->request_type.get() == ci::RequestType::MountUpdate) {
      bool skip = true;
      for (auto& entry : kExtractEntries) {
        if (bf::is_directory(path) && path.filename() == entry) {
          skip = false;
          break;
        }
      }
      if (skip)
        continue;
    }

    if (bf::is_directory(path) && path.filename() == "bin") {
      auto permission = bf::perms::owner_all |
          bf::perms::group_read | bf::perms::group_exe |
          bf::perms::others_read | bf::perms::others_exe;
      if (!ci::SetDirPermissions(path, permission)) {
        LOG(ERROR) << "Grant permission error" << " path: " << path
            << " permission: " << permission;
        return Status::ERROR;
      }
      for (auto& entry :
          boost::make_iterator_range(bf::directory_iterator(path), {})) {
        auto path = entry.path();
        if (bf::is_regular_file(path)) {
          if (!ci::SetDirPermissions(path, permission)) {
            LOG(ERROR) << "Grant permission error" << " path: " << path
                << " permission: " << permission;
            return Status::ERROR; /* temp error, TODO */
          }
        }
      }
      continue;
    }

    if (bf::is_directory(path) && path.filename() == "lib") {
      auto permission = bf::perms::owner_all |
          bf::perms::group_read | bf::perms::group_exe |
          bf::perms::others_read | bf::perms::others_exe;
      if (!ci::SetDirPermissions(path, permission)) {
        LOG(ERROR) << "Grant permission error" << " path: " << path
            << " permission: " << permission;
        return Status::ERROR;
      }
      for (auto& entry :
          boost::make_iterator_range(bf::directory_iterator(path), {})) {
        auto path = entry.path();
        if (bf::is_regular_file(path)) {
          auto permission = bf::perms::owner_read | bf::perms::owner_write |
          bf::perms::group_read | bf::perms::others_read;
          if (!ci::SetDirPermissions(path, permission)) {
            LOG(ERROR) << "Grant permission error" << " path: " << path
                << " permission: " << permission;
            return Status::ERROR;
          }
        }
      }
      continue;
    }

    if (bf::is_directory(path)) {
      auto permission = bf::perms::owner_all |
          bf::perms::group_read | bf::perms::group_exe |
          bf::perms::others_read | bf::perms::others_exe;
      if (!ci::SetDirPermissions(path, permission)) {
        LOG(ERROR) << "Grant permission error" << " path: " << path
            << " permission: " << permission;
        return Status::ERROR;
      }
      continue;
    }

    if (bf::is_regular_file(path)) {
      auto permission = bf::perms::owner_read | bf::perms::owner_write |
          bf::perms::group_read | bf::perms::others_read;
      if (!ci::SetDirPermissions(path, permission)) {
        LOG(ERROR) << "Grant permission error" << " path: " << path
            << " permission: " << permission;
        return Status::ERROR;
      }
      continue;
    }
  }

  return status;
}

}  // namespace filesystem
}  // namespace tpk

