/* Copyright 2015 Samsung Electronics, license APACHE-2.0, see LICENSE file */

#ifndef TPK_STEP_FILESYSTEM_STEP_CREATE_SYMBOLIC_LINK_H_
#define TPK_STEP_FILESYSTEM_STEP_CREATE_SYMBOLIC_LINK_H_

#include <manifest_parser/utils/logging.h>

#include <common/app_installer.h>

namespace tpk {
namespace filesystem {

class StepCreateSymbolicLink : public common_installer::Step {
 public:
  using Step::Step;
  Status process() override;
  Status clean() override { return Status::OK; }
  Status undo() override { return Status::OK; }
  Status precheck() override;

  SCOPE_LOG_TAG(SymbolicLink)
};

}  // namespace filesystem
}  // namespace tpk

#endif  // TPK_STEP_FILESYSTEM_STEP_CREATE_SYMBOLIC_LINK_H_
