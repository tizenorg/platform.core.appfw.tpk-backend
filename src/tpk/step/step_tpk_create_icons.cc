// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/step_tpk_create_icons.h"

namespace tpk {
namespace filesystem {

boost::filesystem::path StepTpkCreateIcons::GetIconRoot() const {
  return context_->pkg_path.get() / "shared" / "res";
}

std::string StepTpkCreateIcons::GetAppTypeForIcons() const {
  return "capp";
}

}  // namespace filesystem
}  // namespace tpk
