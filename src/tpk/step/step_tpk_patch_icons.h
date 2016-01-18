// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_STEP_STEP_TPK_PATCH_ICONS_H_
#define TPK_STEP_STEP_TPK_PATCH_ICONS_H_

#include <common/step/step.h>
#include <manifest_parser/utils/logging.h>

namespace tpk {
namespace filesystem {

/**
 * @brief The StepTpkPatchIcons class
 *        Fixes location of icons for tpk apps if icons are not located in
 *        "shared/res/" directory.
 */
class StepTpkPatchIcons : public common_installer::Step {
 public:
  using Step::Step;

  Status process() override;
  Status clean() override { return Status::OK; }
  Status undo() override { return Status::OK; }
  Status precheck() override { return Status::OK; }

  SCOPE_LOG_TAG(TpkPatchIcons)
};

}  // namespace filesystem
}  // namespace tpk

#endif  // TPK_STEP_STEP_TPK_PATCH_ICONS_H_
