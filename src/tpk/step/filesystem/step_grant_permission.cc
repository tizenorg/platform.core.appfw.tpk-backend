// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/filesystem/step_grant_permission.h"

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

    if (bf::is_directory(path) && path.filename() == "bin") {
      for (auto& entry :
          boost::make_iterator_range(bf::directory_iterator(path), {})) {
        if (bf::is_regular_file(path)) {
          auto permission = bf::perms::owner_all |
              bf::perms::group_read | bf::perms::group_exe |
              bf::perms::others_read | bf::perms::others_exe;
          if (!ci::SetDirPermissions(path, permission)) {
            LOG(ERROR) << "Grand permission error" << " path: " << path
                << " permission: " << permission;
            return Status::GRANT_PERMISSION_ERROR;
          }
        }
      }
      continue;
    }

    if (bf::is_directory(path) && path.filename() == "lib") {
      for (auto& entry :
          boost::make_iterator_range(bf::directory_iterator(path), {})) {
        if (bf::is_regular_file(path)) {
          auto permission = bf::perms::owner_read | bf::perms::owner_write |
              bf::perms::group_read | bf::perms::others_read;
          if (!ci::SetDirPermissions(path, permission)) {
            LOG(ERROR) << "Grand permission error" << " path: " << path
                << " permission: " << permission;
            return Status::GRANT_PERMISSION_ERROR;
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
        LOG(ERROR) << "Grand permission error" << " path: " << path
            << " permission: " << permission;
        return Status::GRANT_PERMISSION_ERROR;
      }
      continue;
    }

    if (bf::is_regular_file(path)) {
      auto permission = bf::perms::owner_read | bf::perms::owner_write |
          bf::perms::group_read | bf::perms::others_read;
      if (!ci::SetDirPermissions(path, permission)) {
        LOG(ERROR) << "Grand permission error" << " path: " << path
            << " permission: " << permission;
        return Status::GRANT_PERMISSION_ERROR;
      }
      continue;
    }
  }

  return status;
}

}  // namespace filesystem
}  // namespace tpk

