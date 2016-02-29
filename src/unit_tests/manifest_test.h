// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef UNIT_TESTS_MANIFEST_TEST_H_
#define UNIT_TESTS_MANIFEST_TEST_H_

#include <boost/filesystem/path.hpp>

#include <common/installer_context.h>
#include <common/request.h>
#include <common/step/step_parse_manifest.h>
#include <common/utils/glist_range.h>

#include <glib.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#define ASSERT_CSTR_EQ(STR1, STR2)                                             \
  ASSERT_EQ(strcmp(STR1, STR2), 0)                                             \

#define ASSERT_CSTR_NE(STR1, STR2)                                             \
  ASSERT_NE(strcmp(STR1, STR2), 0)                                             \

#define ASSERT_IN(ITEM, CONTAINER)                                             \
  ASSERT_NE(std::find(CONTAINER.begin(), CONTAINER.end(),  ITEM),              \
            CONTAINER.end());                                                  \

template<typename T>
gint Size(GListRange<T>* range) {
  return std::distance(range->begin(), range->end());
}

class StepParseRunner {
 public:
  explicit StepParseRunner(const std::string& dir_suffix,
                           bool ignore_start_files = true)
      : dir_suffix_(dir_suffix),
        ignore_start_files_(ignore_start_files),
        context_(nullptr) { }

  bool Run();
  manifest_x* GetManifest() const;

 private:
  void PrepareContext();

  std::string dir_suffix_;
  bool ignore_start_files_;
  std::unique_ptr<common_installer::InstallerContext> context_;
};

class ManifestTest : public testing::Test {
 public:
  std::string GetMyName() const;
};

#endif  // UNIT_TESTS_MANIFEST_TEST_H_
