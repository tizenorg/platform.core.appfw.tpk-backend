// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "unit_tests/manifest_test.h"

namespace bf = boost::filesystem;
namespace ci = common_installer;

TEST_F(ManifestTest, ManifestElement_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  ASSERT_CSTR_EQ(m->package, "package0id");
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->appid, "package0id.appid");
}

TEST_F(ManifestTest, ManifestElement_InvalidRootElement) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, ManifestElement_DoubleRootElement) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, ManifestElement_InstallLocation_Auto) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  ASSERT_CSTR_EQ(m->installlocation, "auto");
}

TEST_F(ManifestTest, ManifestElement_InstallLocation_InternalOnly) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  ASSERT_CSTR_EQ(m->installlocation, "internal-only");
}

TEST_F(ManifestTest, ManifestElement_InstallLocation_PreferExternal) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  ASSERT_CSTR_EQ(m->installlocation, "prefer-external");
}

TEST_F(ManifestTest, ManifestElement_InstallLocation_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  ASSERT_CSTR_EQ(m->installlocation, "auto");
}

TEST_F(ManifestTest, ManifestElement_InstallLocation_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, ManifestElement_Package_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, ManifestElement_Package_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, ManifestElement_Type_Tpk) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  ASSERT_CSTR_EQ(m->type, "tpk");
}

TEST_F(ManifestTest, ManifestElement_Type_Rpm) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  ASSERT_CSTR_EQ(m->type, "rpm");
}

TEST_F(ManifestTest, ManifestElement_Type_Wgt) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  ASSERT_CSTR_EQ(m->type, "wgt");
}

TEST_F(ManifestTest, ManifestElement_Type_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, ManifestElement_Type_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  ASSERT_CSTR_EQ(m->type, "tpk");
}

TEST_F(ManifestTest, ManifestElement_Version_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  ASSERT_CSTR_EQ(m->version, "1.2.3");
}

TEST_F(ManifestTest, ManifestElement_Version_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, ManifestElement_Version_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, ManifestElement_ApiVersion_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  ASSERT_CSTR_EQ(m->api_version, "3.0");
}

TEST_F(ManifestTest, ManifestElement_ApiVersion_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, ManifestElement_ApiVersion_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, AuthorElement_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto authors = GListRange<author_x*>(m->author);
  ASSERT_EQ(Size(&authors), 0);
}

TEST_F(ManifestTest, AuthorElement_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto authors = GListRange<author_x*>(m->author);
  ASSERT_EQ(Size(&authors), 1);
  author_x* author = *authors.begin();
  ASSERT_CSTR_EQ(author->email, "valid@email.com");
  ASSERT_CSTR_EQ(author->href, "http://site.domain.com");
}

TEST_F(ManifestTest, AuthorElement_Email_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, AuthorElement_Email_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, AuthorElement_Href_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, AuthorElement_Many) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, DescriptionElement_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto descriptions = GListRange<description_x*>(m->description);
  ASSERT_EQ(Size(&descriptions), 0);
}

TEST_F(ManifestTest, DescriptionElement_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto descriptions = GListRange<description_x*>(m->description);
  ASSERT_EQ(Size(&descriptions), 1);
  description_x* description = *descriptions.begin();
  ASSERT_CSTR_EQ(description->text, "text");
  ASSERT_CSTR_EQ(description->lang, "en-GB");
}

TEST_F(ManifestTest, DescriptionElement_Many) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto descriptions = GListRange<description_x*>(m->description);
  ASSERT_EQ(Size(&descriptions), 2);
  description_x* description1 = *descriptions.begin();
  description_x* description2 = *++descriptions.begin();
  ASSERT_CSTR_EQ(description1->text, "text");
  ASSERT_CSTR_EQ(description1->lang, "en-GB");
  ASSERT_CSTR_EQ(description2->text, "text2");
  ASSERT_CSTR_EQ(description2->lang, DEFAULT_LOCALE);
}

TEST_F(ManifestTest, DescriptionElement_Lang_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, PrivilegesElement_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto privileges = GListRange<const char*>(m->privileges);
  ASSERT_EQ(Size(&privileges), 0);
}

TEST_F(ManifestTest, PrivilegesElement_None) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto privileges = GListRange<char*>(m->privileges);
  ASSERT_EQ(Size(&privileges), 0);
}

TEST_F(ManifestTest, PrivilegesElement_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto privileges = GListRange<char*>(m->privileges);
  ASSERT_EQ(Size(&privileges), 1);
  ASSERT_CSTR_EQ(*(privileges.begin()),
                 "http://tizen.org/privilege/application.admin");
}

TEST_F(ManifestTest, PrivilegesElement_Many) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto privileges = GListRange<char*>(m->privileges);
  ASSERT_EQ(Size(&privileges), 3);
  ASSERT_CSTR_EQ(*(privileges.begin()),
                 "http://tizen.org/privilege/account.read");
  ASSERT_CSTR_EQ(*(++privileges.begin()),
                 "http://tizen.org/privilege/application.admin");
  ASSERT_CSTR_EQ(*(++++privileges.begin()),
                 "http://tizen.org/privilege/appmanager.launch");
}

TEST_F(ManifestTest, ProfileElement_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto profiles = GListRange<char*>(m->deviceprofile);
  ASSERT_EQ(Size(&profiles), 0);
}

TEST_F(ManifestTest, ProfileElement_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto profiles = GListRange<char*>(m->deviceprofile);
  ASSERT_EQ(Size(&profiles), 1);
  const char* profile = *profiles.begin();
  ASSERT_CSTR_EQ(profile, "mobile");
}

TEST_F(ManifestTest, ProfileElement_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, ProfileElement_Many) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto profiles = GListRange<char*>(m->deviceprofile);
  ASSERT_EQ(Size(&profiles), 2);
  const char* profile1 = *profiles.begin();
  const char* profile2 = *++profiles.begin();
  ASSERT_CSTR_EQ(profile1, "mobile");
  ASSERT_CSTR_EQ(profile2, "wearable");
}
