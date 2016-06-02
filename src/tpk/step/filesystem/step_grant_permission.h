// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.


#ifndef TPK_STEP_FILESYSTEM_TPK_GRANT_PERMISSION_H_
#define TPK_STEP_FILESYSTEM_TPK_GRANT_PERMISSION_H_

#include <manifest_parser/utils/logging.h>

#include <string>

#include "common/installer_context.h"
#include "common/step/step.h"

namespace tpk {
namespace filesystem {

/**
 * \brief Step responsible for granting permissions to /bin, /lib
 */
class StepTpkGrantPermission : public common_installer::Step {
 public:
  using Step::Step;

  Status process() override;
  Status undo() override { return Status::OK; }
  Status clean() override { return Status::OK; }
  Status precheck() override;

  STEP_NAME(GrantPermission)
};

}  // namespace filesystem
}  // namespace tpk

#endif  // TPK_STEP_FILESYSTEM_TPK_GRANT_PERMISSION_H_
