// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/step_parse_preload.h"

#include <pkgmgr-info.h>
#include <unistd.h>

#include <boost/filesystem.hpp>

#include <cstring>
#include <cstdio>
#include <string>

#include "common/pkgmgr_interface.h"
#include "common/utils/file_util.h"

namespace {
const char kPathManifestPrefix[] = "/usr/share/packages";
}

namespace common_installer {
namespace tpk {

namespace bf = boost::filesystem;

Step::Status StepParsePreload::process() {
  bool is_preload = context_->is_preload_request.get();

  if (is_preload) {
    context_->manifest_data.get()->preload = strdup("true");

    if (getuid() != 0) {
      LOG(ERROR) << "You're not authorized to install preload app: "
          << context_->pkgid.get().c_str();
      return Status::OPERATION_NOT_ALLOWED;
    }
  } else {
    context_->manifest_data.get()->preload = strdup("false");
  }

  return Status::OK;
}

}  // namespace tpk
}  // namespace common_installer
