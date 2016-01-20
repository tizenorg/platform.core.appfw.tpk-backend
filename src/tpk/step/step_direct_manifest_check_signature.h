// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_STEP_STEP_DIRECT_MANIFEST_CHECK_SIGNATURE_H_
#define TPK_STEP_STEP_DIRECT_MANIFEST_CHECK_SIGNATURE_H_

#include <common/step/step_check_signature.h>
#include <manifest_parser/utils/logging.h>

namespace tpk {
namespace security {

/**
 * @brief The StepDirectManifestCheckSignature class
 *        This step is used in direct manifest installation to conditionally
 *        skip checking signature for preloaded packages.
 *
 * Preloaded packages must be globally installed.
 */
class StepDirectManifestCheckSignature
    : public common_installer::security::StepCheckSignature {
 public:
  using StepCheckSignature::StepCheckSignature;

  Status process() override;

  SCOPE_LOG_TAG(DirectManifestCheckSignature)
};

}  // namespace security
}  // namespace tpk

#endif  // TPK_STEP_STEP_DIRECT_MANIFEST_CHECK_SIGNATURE_H_
