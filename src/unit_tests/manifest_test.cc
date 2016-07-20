// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "unit_tests/manifest_test.h"

#include <cassert>

namespace bf = boost::filesystem;
namespace ci = common_installer;

namespace {

const char kManifestTestcaseData[] =
    "/usr/share/tpk-backend-ut/test_samples/manifest/";

}  // namespace

bool StepParseRunner::Run() {
  PrepareContext();
  ci::configuration::StepParseManifest step(context_.get(),
      ci::configuration::StepParseManifest::ManifestLocation::PACKAGE,
      ci::configuration::StepParseManifest::StoreLocation::NORMAL);
  return step.process() == ci::Step::Status::OK;
}

manifest_x* StepParseRunner::GetManifest() const {
  return context_->manifest_data.get();
}

void StepParseRunner::PrepareContext() {
  context_.reset(new ci::InstallerContext());
  context_->root_application_path.set(ci::GetRootAppPath(false, getuid()));
  context_->unpacked_dir_path.set(
      bf::path(kManifestTestcaseData) / dir_suffix_);

  // Make sure that there is manifest file for testcase
  assert(bf::exists(context_->unpacked_dir_path.get() / "tizen-manifest.xml")
         && "Failed to stat manifest file");
}

std::string ManifestTest::GetMyName() const {
  std::string suite =
      testing::UnitTest::GetInstance()->current_test_info()->test_case_name();
  std::string case_name =
      testing::UnitTest::GetInstance()->current_test_info()->name();
  return suite + '.' + case_name;
}

application_x* GetSingleApp(manifest_x* m) {
  if (!m)
    return nullptr;
  auto apps = GListRange<application_x*>(m->application);
  if (Size(&apps) != 1)
    return nullptr;
  return *apps.begin();
}
