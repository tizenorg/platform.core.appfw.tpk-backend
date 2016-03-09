// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/filesystem/step_check_pkg_directory_path.h"

#include <boost/filesystem.hpp>
#include <string>

namespace tpk {
namespace filesystem {

namespace bf = boost::filesystem;
namespace bs = boost::system;

common_installer::Step::Status StepCheckPkgDirPath::process() {
  bf::path pkg_path = context_->package_storage->path();
  if (!bf::exists(pkg_path)) {
    LOG(INFO) << "Create pkg_path("
              << pkg_path
              << ") for package("
              << context_->pkgid.get() << ")";
    bs::error_code error;
    bf::create_directories(pkg_path, error);
    if (error) {
      LOG(ERROR) << "Cannot create directory: " << pkg_path;
      return Step::Status::APP_DIR_ERROR;
    }
  }

  return Status::OK;
}

}  // namespace filesystem
}  // namespace tpk
