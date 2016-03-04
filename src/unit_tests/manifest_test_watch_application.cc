// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "unit_tests/manifest_test.h"

namespace bf = boost::filesystem;
namespace ci = common_installer;

TEST_F(ManifestTest, WatchApplicationElement_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  ASSERT_CSTR_EQ(m->package, "package0id");
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->appid, "package0id.appid");
  ASSERT_CSTR_EQ(app->component_type, "watchapp");
}

TEST_F(ManifestTest, WatchApplicationElement_Appid_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, WatchApplicationElement_Appid_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, WatchApplicationElement_Label_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  application_x* app = GetSingleApp(runner.GetManifest());
  ASSERT_NE(app, nullptr);
  auto labels = GListRange<label_x*>(app->label);
  ASSERT_EQ(Size(&labels), 0);
}

TEST_F(ManifestTest, WatchApplicationElement_Label_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  application_x* app = GetSingleApp(runner.GetManifest());
  ASSERT_NE(app, nullptr);
  auto labels = GListRange<label_x*>(app->label);
  ASSERT_EQ(Size(&labels), 1);
  ASSERT_CSTR_EQ((*labels.begin())->text, "label");
  ASSERT_CSTR_EQ((*labels.begin())->lang, DEFAULT_LOCALE);
}

TEST_F(ManifestTest, WatchApplicationElement_Label_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, WatchApplicationElement_Icon_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  application_x* app = GetSingleApp(runner.GetManifest());
  ASSERT_NE(app, nullptr);
  auto icons = GListRange<icon_x*>(app->icon);
  ASSERT_EQ(Size(&icons), 0);
}

TEST_F(ManifestTest, WatchApplicationElement_Icon_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, WatchApplicationElement_Icon_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  application_x* app = GetSingleApp(runner.GetManifest());
  ASSERT_NE(app, nullptr);
  auto icons = GListRange<icon_x*>(app->icon);
  ASSERT_EQ(Size(&icons), 1);
  ASSERT_EQ(bf::path((*icons.begin())->text).filename().string(),
            "test_icon.png");
}

TEST_F(ManifestTest, WatchApplicationElement_AmbientSupport_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  application_x* app = GetSingleApp(runner.GetManifest());
  ASSERT_NE(app, nullptr);
  ASSERT_CSTR_EQ(app->ambient_support, "false");
}

TEST_F(ManifestTest, WatchApplicationElement_AmbientSupport_False) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  application_x* app = GetSingleApp(runner.GetManifest());
  ASSERT_NE(app, nullptr);
  ASSERT_CSTR_EQ(app->ambient_support, "false");
}


TEST_F(ManifestTest, WatchApplicationElement_AmbientSupport_True) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  application_x* app = GetSingleApp(runner.GetManifest());
  ASSERT_NE(app, nullptr);
  ASSERT_CSTR_EQ(app->ambient_support, "true");
}

TEST_F(ManifestTest, WatchApplicationElement_AmbientSupport_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, WatchApplicationElement_SetManifestXDefaults) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  ASSERT_CSTR_EQ(m->package, "package0id");
  ASSERT_CSTR_EQ(m->version, "1.0.0");
  ASSERT_CSTR_EQ(m->installlocation, "auto");
  ASSERT_CSTR_EQ(m->appsetting, "false");
  ASSERT_CSTR_EQ(m->type, "tpk");
  ASSERT_NE(m->installed_time, nullptr);
  ASSERT_CSTR_EQ(m->mainapp_id, "package0id.appid");
  ASSERT_CSTR_EQ(m->nodisplay_setting, "false");
  ASSERT_CSTR_EQ(m->support_disable, "false");
  ASSERT_CSTR_EQ(m->api_version, "2.3");
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->appid, "package0id.appid");
  ASSERT_NE(app->exec, nullptr);
  ASSERT_CSTR_EQ(app->nodisplay, "false");
  ASSERT_CSTR_EQ(app->multiple, "false");
  ASSERT_CSTR_EQ(app->taskmanage, "false");
  ASSERT_CSTR_EQ(app->enabled, "true");
  ASSERT_CSTR_EQ(app->hwacceleration, "default");
  ASSERT_CSTR_EQ(app->screenreader, "use-system-setting");
  ASSERT_CSTR_EQ(app->component_type, "watchapp");
  ASSERT_CSTR_EQ(app->mainapp, "true");
  ASSERT_CSTR_EQ(app->recentimage, "false");
  ASSERT_CSTR_EQ(app->launchcondition, "false");
  ASSERT_CSTR_EQ(app->indicatordisplay, "true");
  ASSERT_CSTR_EQ(app->effectimage_type, "image");
  ASSERT_CSTR_EQ(app->guestmode_visibility, "true");
  ASSERT_CSTR_EQ(app->permission_type, "normal");
  ASSERT_CSTR_EQ(app->submode, "false");
  ASSERT_CSTR_EQ(app->process_pool, "false");
}
