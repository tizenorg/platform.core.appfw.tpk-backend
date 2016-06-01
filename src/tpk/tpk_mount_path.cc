// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk/tpk_mount_path.h"

#include <unistd.h>
#include <sys/types.h>

namespace {

const std::vector<std::string> kExtractEntries = {
  "bin",
  "lib",
  "shared/res"
};

const std::vector<std::string> kSymlinkEntries = {
  "res",
  "tizen-manifest.xml",
  "author-signature.xml",
  "signature1.xml",
  "signature2.xml"
};

}  // namespace

namespace tpk {

const std::vector<std::string>& GetExtractEntries() {
  return kExtractEntries;
}

const std::vector<std::string>& GetSymlinkEntries() {
  return kSymlinkEntries;
}

}  // namespace tpk
