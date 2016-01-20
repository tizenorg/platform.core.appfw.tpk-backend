// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/step_direct_manifest_check_signature.h"

namespace tpk {
namespace security {

common_installer::Step::Status StepDirectManifestCheckSignature::process() {
  if (context_->request_mode.get() == common_installer::RequestMode::GLOBAL) {
    // set fixed privilege level for global apps installed directly from
    // platform manifest file (preloaded)
    context_->privilege_level.set(common_installer::PrivilegeLevel::PLATFORM);
    return Status::OK;
  } else {
    return StepCheckSignature::process();
  }
}

}  // namespace security
}  // namespace tpk

