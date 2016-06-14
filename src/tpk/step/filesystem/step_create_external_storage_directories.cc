// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/filesystem/step_create_external_storage_directories.h"

#include <common/privileges.h>
#include <common/shared_dirs.h>
#include <common/utils/glist_range.h>

#include <string>
#include <vector>

#include "tpk/external_dirs.h"

namespace ci = common_installer;

namespace tpk {
namespace filesystem {

ci::Step::Status StepCreateExternalStorageDirectories::process() {
  if (!HasExternalAppdataPrivilege(context_->manifest_data.get())) {
    LOG(DEBUG) << "External storage privilege not found, skipping.";
    return Status::OK;
  } else {
    if (!CreateExternalAppdataDirectories(context_->pkgid.get(),
                                          context_->request_mode.get(),
                                          context_->uid.get()))
      return Status::APP_DIR_ERROR;
  }
  return Status::OK;
}

ci::Step::Status StepCreateExternalStorageDirectories::precheck() {
  if (!context_->manifest_data.get()) {
    LOG(ERROR) << "Manifest data not set";
    return Status::INVALID_VALUE;
  }
  return Status::OK;
}

}  // namespace filesystem
}  // namespace tpk
