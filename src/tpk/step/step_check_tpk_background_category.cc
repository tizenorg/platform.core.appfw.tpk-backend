// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/step_check_tpk_background_category.h"

#include <common/installer_context.h>

namespace {
namespace ci_sec = common_installer::security;
}  // namespace

namespace tpk {
namespace security {

StepCheckTpkBackgroundCategory::StepCheckTpkBackgroundCategory(
    common_installer::InstallerContext* context) :
        ci_sec::StepCheckBackgroundCategory(context) {}

bool StepCheckTpkBackgroundCategory::GetBackgroundSupport() {
  return true;
}

}  // namespace security
}  // namespace tpk
