// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/filesystem/step_remove_external_storage_directories.h"

#include "tpk/external_dirs.h"

namespace ci = common_installer;

namespace tpk {
namespace filesystem {

ci::Step::Status StepRemoveExternalStorageDirectories::process() {
  if (!DeleteExternalAppdataDirectories(context_->pkgid.get(),
                                        context_->request_mode.get(),
                                        context_->uid.get()))
    return Status::APP_DIR_ERROR;
  return Status::OK;
}

}  // namespace filesystem
}  // namespace tpk
