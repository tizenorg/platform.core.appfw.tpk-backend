// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/step_check_pkg_directory_path.h"

#include <boost/filesystem.hpp>
#include <string>

namespace tpk {
namespace filesystem {

namespace bf = boost::filesystem;
namespace bs = boost::system;

common_installer::Step::Status StepCheckPkgDirPath::process() {
  if (!bf::exists(context_->pkg_path.get())) {
    LOG(INFO) << "Create pkg_path("
              << context_->pkg_path.get()
              << ") for package("
              << context_->pkgid.get() << ")";
    bs::error_code error;
    bf::create_directories(context_->pkg_path.get(), error);
    if (error) {
      LOG(ERROR) << "Cannot create directory: "
                 << context_->pkg_path.get().string();
      return Step::Status::APP_DIR_ERROR;
    }
  }

  return Status::OK;
}

}  // namespace filesystem
}  // namespace tpk
