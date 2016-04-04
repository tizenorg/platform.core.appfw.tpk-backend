// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/rds/step_tpk_rds_modify.h"

namespace tpk {
namespace rds {

std::string StepTpkRDSModify::GetAppPath() {
  return context_->pkg_path.get();
}

}  // namespace rds
}  // namespace tpk
