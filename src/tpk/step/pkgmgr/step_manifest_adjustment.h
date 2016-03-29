// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_STEP_PKGMGR_STEP_MANIFEST_ADJUSTMENT_H_
#define TPK_STEP_PKGMGR_STEP_MANIFEST_ADJUSTMENT_H_

#include <manifest_parser/utils/logging.h>

#include <boost/filesystem/path.hpp>

#include <common/step/step.h>

namespace tpk {
namespace pkgmgr {

class __attribute__ ((visibility ("default"))) StepManifestAdjustment : public common_installer::Step {
 public:
  using Step::Step;

  Status process() override;
  Status clean() override;
  Status undo() override;
  Status precheck() override;

 private:
  boost::filesystem::path xml_path_;

  SCOPE_LOG_TAG(StepManifestAdjustment)
};

}  // namespace pkgmgr
}  // namespace tpk

#endif  // TPK_STEP_PKGMGR_STEP_MANIFEST_ADJUSTMENT_H_
