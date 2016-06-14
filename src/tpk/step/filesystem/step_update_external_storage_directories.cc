// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/filesystem/step_update_external_storage_directories.h"

#include "tpk/external_dirs.h"

namespace ci = common_installer;

namespace tpk {
namespace filesystem {

ci::Step::Status StepUpdateExternalStorageDirectories::process() {
  if (!HasExternalAppdataPrivilege(context_->manifest_data.get())) {
    if (!RemoveExternalAppdataDirectories(context_->pkgid.get(),
                                          context_->request_mode.get(),
                                          context_->uid.get()))
      return Status::APP_DIR_ERROR;
  } else {
    if (!CreateExternalAppdataDirectories(context_->pkgid.get(),
                                          context_->request_mode.get(),
                                          context_->uid.get()))
      return Status::APP_DIR_ERROR;
  }
  return Status::OK;
}

ci::Step::Status StepUpdateExternalStorageDirectories::precheck() {
  if (!context_->manifest_data.get()) {
    LOG(ERROR) << "Manifest data not set";
    return Status::INVALID_VALUE;
  }
  return Status::OK;
}

}  // namespace filesystem
}  // namespace tpk
