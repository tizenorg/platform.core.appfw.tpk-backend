// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk/external_dirs.h"

#include <common/privileges.h>
#include <common/pkgmgr_registration.h>
#include <common/shared_dirs.h>
#include <common/utils/glist_range.h>
#include <manifest_parser/utils/logging.h>

#include <algorithm>

namespace ci = common_installer;

namespace tpk {

bool HasExternalAppdataPrivilege(manifest_x* manifest) {
  auto privileges = GListRange<char*>(manifest->privileges);
  return std::find(privileges.begin(), privileges.end(),
      std::string(common::privileges::kPrivForExternalAppData))
          != privileges.end();
}

bool CreateExternalAppdataDirectories(const std::string& pkgid,
                                      ci::RequestMode request_mode, uid_t uid) {
  // TODO(t.iwanek): trusted in this context means that we have signature
  // this argument is not longer needed as all package must be signed
  // so that trusted directory may be labeled correctly by security-manager in
  // all cases. This parameter and its propagation should be removed.
  bool trusted = true;

  switch (request_mode) {
    case ci::RequestMode::GLOBAL: {
      LOG(DEBUG) << "Creating external directories for all users";
      ci::PerformExternalDirectoryCreationForAllUsers(pkgid, trusted);
      break;
    }
    case ci::RequestMode::USER: {
      LOG(DEBUG) << "Creating external directories for user: " << uid;
      ci::PerformExternalDirectoryCreationForUser(uid, pkgid, trusted);
      break;
    }
  }
  return true;
}

bool RemoveExternalAppdataDirectories(const std::string& pkgid,
                                      ci::RequestMode request_mode, uid_t uid) {
  switch (request_mode) {
    case ci::RequestMode::GLOBAL: {
      LOG(DEBUG) << "Removing external directories for all users";
      ci::PerformExternalDirectoryDeletionForAllUsers(pkgid);
      break;
    }
    case ci::RequestMode::USER: {
      // if package is globally installed, leave directories
      if (ci::IsPackageInstalled(pkgid, GLOBAL_USER))
        return true;

      LOG(DEBUG) << "Removing external directories for user: " << uid;
      ci::PerformExternalDirectoryDeletionForUser(uid, pkgid);
      break;
    }
  }
  return true;
}

}  // namespace tpk
