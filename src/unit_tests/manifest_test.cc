// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "unit_tests/manifest_test.h"

namespace bf = boost::filesystem;
namespace ci = common_installer;

namespace {

const char kManifestTestcaseData[] =
    "/usr/share/tpk-backend-ut/test_samples/manifest/";

}  // namespace

bool StepParseRunner::Run() {
  PrepareContext();
  ci::parse::StepParseManifest step(context_.get(),
      ci::parse::StepParseManifest::ManifestLocation::PACKAGE,
      ci::parse::StepParseManifest::StoreLocation::NORMAL);
  return step.process() == ci::Step::Status::OK;
}

manifest_x* StepParseRunner::GetManifest() const {
  return context_->manifest_data.get();
}

void StepParseRunner::PrepareContext() {
  context_.reset(new ci::InstallerContext());
  context_->root_application_path.set(ci::GetRootAppPath(false));
  context_->unpacked_dir_path.set(
      bf::path(kManifestTestcaseData) / dir_suffix_);
}

std::string ManifestTest::GetMyName() const {
  std::string suite =
      testing::UnitTest::GetInstance()->current_test_info()->test_case_name();
  std::string case_name =
      testing::UnitTest::GetInstance()->current_test_info()->name();
  return suite + '.' + case_name;
}
