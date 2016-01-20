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


bool IsRunningInOfflineMode() {
  pkgmgr_installer* pi = pkgmgr_installer_new();
  if (!pi) return false;
  pkgmgr_installer_free(pi);
  return true;
}

Step::Status StepParsePreload::process() {
  const char* package_id = context_->pkgid.get().c_str();
  const char* preload_manifest_val = context_->manifest_data.get()->preload;

  if (!preload_manifest_val) {
    LOG(ERROR) << "Could not find preload attribute in package:" << package_id;
    return Status::PARSE_ERROR;
  }

  bool is_preload = false;

  if (IsRunningInOfflineMode()) {
    if (strcmp(preload_manifest_val, "false") == 0) {
      context_->manifest_data.get()->preload = strdup("true");
      is_preload = true;
    }
  } else {

    if (context_->request_type.get() == RequestType::ManifestDirectInstall ||
        context_->request_type.get() == RequestType::ManifestDirectUpdate) {
      if (strcmp(preload_manifest_val, "") == 0) {
        context_->manifest_data.get()->preload = strdup("true");
        is_preload = true;
      }
    }
  }

  if (is_preload) {
    LOG(ERROR) << "You're not authorized to install preload app: "
        << package_id;
    return Status::OPERATION_NOT_ALLOWED;
  }

  return Status::OK;
}

}  // namespace tpk
}  // namespace common_installer
