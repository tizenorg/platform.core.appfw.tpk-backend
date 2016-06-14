// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_STEP_FILESYSTEM_STEP_UPDATE_EXTERNAL_STORAGE_DIRECTORIES_H_
#define TPK_STEP_FILESYSTEM_STEP_UPDATE_EXTERNAL_STORAGE_DIRECTORIES_H_

#include <common/installer_context.h>
#include <common/step/step.h>
#include <manifest_parser/utils/logging.h>

namespace tpk {
namespace filesystem {

class StepUpdateExternalStorageDirectories : public common_installer::Step {
 public:
  using Step::Step;

  Status process() override;
  Status clean() override { return Status::OK; }
  Status undo() override  { return Status::OK; }
  Status precheck() override;

  STEP_NAME(UpdateExternalStorageDirectories)
};

}  // namespace filesystem
}  // namespace tpk

#endif  // TPK_STEP_FILESYSTEM_STEP_UPDATE_EXTERNAL_STORAGE_DIRECTORIES_H_
