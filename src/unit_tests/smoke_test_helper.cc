// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include <common/pkgmgr_interface.h>
#include <common/step/step.h>
#include <manifest_parser/utils/logging.h>

#include <iostream>

#include "tpk/tpk_app_query_interface.h"
#include "tpk/tpk_installer.h"

namespace ci = common_installer;

namespace {

class StepCrash : public ci::Step {
 public:
  using Step::Step;

  ci::Step::Status process() override {
    raise(SIGSEGV);
    return Status::OK;
  }
  ci::Step::Status clean() override { return ci::Step::Status::OK; }
  ci::Step::Status undo() override { return ci::Step::Status::OK; }
  ci::Step::Status precheck() override { return ci::Step::Status::OK; }
};

}  // namespace

// This version of backend will crash in the end
// it is used for recovery testcase
int main(const int argc, char* argv[]) {
  ci::PkgmgrInstaller pkgmgr_installer;
  tpk::TpkAppQueryInterface interface;
  ci::PkgMgrPtr pkgmgr = ci::PkgMgrInterface::Create(argc, argv,
                                                     &pkgmgr_installer,
                                                     &interface);
  if (!pkgmgr) {
    LOG(ERROR) << "Failed to create pkgmgr interface";
    return -1;
  }

  tpk::TpkInstaller t(pkgmgr);
  t.AddStep<StepCrash>();
  if (t.Run() != ci::AppInstaller::Result::OK) {
    LOG(ERROR) << "TpkInstaller run failure";
    return -1;
  }
  return 0;
}
