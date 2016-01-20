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

#include "common/utils/file_util.h"

namespace {
const char* kPathManifestPrefix = "/usr/share/packages";
}

namespace common_installer {
namespace parse {

namespace bf = boost::filesystem;

Step::Status StepParsePreload::process() {
  const char* package_id = context_->pkgid.get().c_str();
  const char* preload_manifest_val = context_->manifest_data.get()->preload;
  if (!preload_manifest_val) {
    LOG(ERROR) << "Could not find preload attribute in package:" << package_id;
    return Status::PARSE_ERROR;
  }

  bf::path xml_path(getUserManifestPath(context_->uid.get()));
  xml_path /= package_id;
  xml_path += ".xml";

  bool is_preloaded = bf::exists(xml_path) &&
      (strcmp(preload_manifest_val, "true") == 0);

  if (getuid() != 0) {
    if (is_preloaded) {
      LOG(ERROR) << "You're not authorized to install preload app: "
          << package_id;
      return Status::OPERATION_NOT_ALLOWED;
    }
  }

  return Status::OK;
}

}  // namespace parse
}  // namespace common_installer
