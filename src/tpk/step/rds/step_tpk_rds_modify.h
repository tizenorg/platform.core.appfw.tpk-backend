// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_STEP_RDS_STEP_TPK_RDS_MODIFY_H_
#define TPK_STEP_RDS_STEP_TPK_RDS_MODIFY_H_

#include <boost/filesystem.hpp>

#include <common/step/step.h>
#include <common/installer_context.h>
#include <common/step/rds/step_rds_modify.h>

#include <string>
#include <utility>
#include <vector>

namespace tpk {
namespace rds {

/**
 * \brief Step that apply RDS modification during reinstallation process of tpk
 */
class StepTpkRDSModify : public common_installer::rds::StepRDSModify {
  using StepRDSModify::StepRDSModify;

 protected:
  std::string GetAppPath() override;
};

}  // namespace rds
}  // namespace tpk

#endif  // TPK_STEP_RDS_STEP_TPK_RDS_MODIFY_H_
