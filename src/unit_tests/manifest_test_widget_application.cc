// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "unit_tests/manifest_test.h"

namespace bf = boost::filesystem;
namespace ci = common_installer;

TEST_F(ManifestTest, WidgetApplicationElement_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  ASSERT_CSTR_EQ(m->package, "package0id");
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->appid, "package0id.appid");
  ASSERT_CSTR_EQ(app->component_type, "widgetapp");
}

TEST_F(ManifestTest, WidgetApplicationElement_Appid_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, WidgetApplicationElement_Appid_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, WidgetApplicationElement_Label_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  application_x* app = GetSingleApp(runner.GetManifest());
  ASSERT_NE(app, nullptr);
  auto labels = GListRange<label_x*>(app->label);
  ASSERT_EQ(Size(&labels), 0);
}

TEST_F(ManifestTest, WidgetApplicationElement_Label_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  application_x* app = GetSingleApp(runner.GetManifest());
  ASSERT_NE(app, nullptr);
  auto labels = GListRange<label_x*>(app->label);
  ASSERT_EQ(Size(&labels), 1);
  ASSERT_CSTR_EQ((*labels.begin())->text, "label");
  ASSERT_CSTR_EQ((*labels.begin())->lang, DEFAULT_LOCALE);
}

TEST_F(ManifestTest, WidgetApplicationElement_Label_Text_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, WidgetApplicationElement_Label_Lang_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, WidgetApplicationElement_Label_Many) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  application_x* app = GetSingleApp(runner.GetManifest());
  ASSERT_NE(app, nullptr);
  auto labels = GListRange<label_x*>(app->label);
  ASSERT_EQ(Size(&labels), 2);
  ASSERT_CSTR_EQ((*labels.begin())->text, "label");
  ASSERT_CSTR_EQ((*labels.begin())->lang, DEFAULT_LOCALE);
  ASSERT_CSTR_EQ((*++labels.begin())->text, "label_GB");
  ASSERT_CSTR_EQ((*++labels.begin())->lang, "en-GB");
}

TEST_F(ManifestTest, WidgetApplicationElement_Label_Many_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, WidgetApplicationElement_Icon_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  application_x* app = GetSingleApp(runner.GetManifest());
  ASSERT_NE(app, nullptr);
  auto icons = GListRange<icon_x*>(app->icon);
  ASSERT_EQ(Size(&icons), 0);
}

TEST_F(ManifestTest, WidgetApplicationElement_Icon_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, WidgetApplicationElement_Icon_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  application_x* app = GetSingleApp(runner.GetManifest());
  ASSERT_NE(app, nullptr);
  auto icons = GListRange<icon_x*>(app->icon);
  ASSERT_EQ(Size(&icons), 1);
  ASSERT_EQ(bf::path((*icons.begin())->text).filename().string(),
            "test_icon.png");
}

TEST_F(ManifestTest, WidgetApplicationElement_Main_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, WidgetApplicationElement_Main_Invalid_BothMain) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, WidgetApplicationElement_Main_TakeFirst) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  ASSERT_CSTR_EQ(m->package, "package0id");
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 2);
  application_x* app1 = *apps.begin();
  application_x* app2 = *++apps.begin();
  ASSERT_CSTR_EQ(app1->appid, "package0id.appid1");
  ASSERT_CSTR_EQ(app1->mainapp, "true");
  ASSERT_CSTR_EQ(app2->appid, "package0id.appid2");
  ASSERT_CSTR_EQ(app2->mainapp, "false");
  ASSERT_CSTR_EQ(m->mainapp_id, "package0id.appid1");
}

TEST_F(ManifestTest, WidgetApplicationElement_Main_TakeMarked) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  ASSERT_CSTR_EQ(m->package, "package0id");
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 2);
  application_x* app1 = *apps.begin();
  application_x* app2 = *++apps.begin();
  ASSERT_CSTR_EQ(app1->appid, "package0id.appid1");
  ASSERT_CSTR_EQ(app1->mainapp, "false");
  ASSERT_CSTR_EQ(app2->appid, "package0id.appid2");
  ASSERT_CSTR_EQ(app2->mainapp, "true");
  ASSERT_CSTR_EQ(m->mainapp_id, "package0id.appid2");
}

// preview, update-period, support-size are supposed to be parsed by plugin
