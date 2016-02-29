// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "unit_tests/manifest_test.h"

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

#include <vector>

namespace bf = boost::filesystem;
namespace ci = common_installer;

TEST_F(ManifestTest, UIApplicationElement_Valid) {
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

TEST_F(ManifestTest, UIApplicationElement_MissingNoApp) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_MissingOtherApp) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_NE(app->component_type, "uiapp");
}

TEST_F(ManifestTest, UIApplicationElement_Appid_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_Appid_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_LaunchMode_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->launch_mode, "single");
}

TEST_F(ManifestTest, UIApplicationElement_LaunchMode_Single) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->launch_mode, "single");
}

TEST_F(ManifestTest, UIApplicationElement_LaunchMode_Group) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->launch_mode, "group");
}

TEST_F(ManifestTest, UIApplicationElement_LaunchMode_Caller) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->launch_mode, "caller");
}

TEST_F(ManifestTest, UIApplicationElement_LaunchMode_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_Multiple_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->multiple, "false");
}

TEST_F(ManifestTest, UIApplicationElement_Multiple_False) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->multiple, "false");
}

TEST_F(ManifestTest, UIApplicationElement_Multiple_True) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->multiple, "true");
}

TEST_F(ManifestTest, UIApplicationElement_Multiple_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_Nodisplay_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->nodisplay, "false");
}

TEST_F(ManifestTest, UIApplicationElement_Nodisplay_False) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->nodisplay, "false");
}

TEST_F(ManifestTest, UIApplicationElement_Nodisplay_True) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->nodisplay, "true");
}

TEST_F(ManifestTest, UIApplicationElement_Nodisplay_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_Taskmanage_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->taskmanage, "true");
}

TEST_F(ManifestTest, UIApplicationElement_Taskmanage_False) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->taskmanage, "false");
}

TEST_F(ManifestTest, UIApplicationElement_Taskmanage_True) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->taskmanage, "true");
}

TEST_F(ManifestTest, UIApplicationElement_Taskmanage_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_HwAcceleration_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->hwacceleration, "default");
}

TEST_F(ManifestTest, UIApplicationElement_HwAcceleration_Default) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->hwacceleration, "default");
}

TEST_F(ManifestTest, UIApplicationElement_HwAcceleration_Off) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->hwacceleration, "off");
}

TEST_F(ManifestTest, UIApplicationElement_HwAcceleration_On) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->hwacceleration, "on");
}

TEST_F(ManifestTest, UIApplicationElement_Type_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_Type_Capp) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->type, "capp");
}

TEST_F(ManifestTest, UIApplicationElement_Type_Webapp) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  ASSERT_CSTR_EQ(app->type, "webapp");
}

TEST_F(ManifestTest, UIApplicationElement_Label_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto labels = GListRange<label_x*>(app->label);
  ASSERT_EQ(Size(&labels), 0);
}

TEST_F(ManifestTest, UIApplicationElement_Label_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto labels = GListRange<label_x*>(app->label);
  ASSERT_EQ(Size(&labels), 1);
  ASSERT_CSTR_EQ((*labels.begin())->text, "label");
  ASSERT_CSTR_EQ((*labels.begin())->lang, DEFAULT_LOCALE);
}

TEST_F(ManifestTest, UIApplicationElement_Label_Text_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_Label_Lang_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_Label_Many) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto labels = GListRange<label_x*>(app->label);
  ASSERT_EQ(Size(&labels), 2);
  ASSERT_CSTR_EQ((*labels.begin())->text, "label");
  ASSERT_CSTR_EQ((*labels.begin())->lang, DEFAULT_LOCALE);
  ASSERT_CSTR_EQ((*++labels.begin())->text, "label_GB");
  ASSERT_CSTR_EQ((*++labels.begin())->lang, "en-GB");
}

TEST_F(ManifestTest, UIApplicationElement_Label_Many_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_Metadata_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto labels = GListRange<label_x*>(app->metadata);
  ASSERT_EQ(Size(&labels), 0);
}

TEST_F(ManifestTest, UIApplicationElement_Metadata_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto metadata = GListRange<metadata_x*>(app->metadata);
  ASSERT_EQ(Size(&metadata), 1);
  ASSERT_CSTR_EQ((*metadata.begin())->key, "key");
  ASSERT_CSTR_EQ((*metadata.begin())->value, "value");
}

TEST_F(ManifestTest, UIApplicationElement_Metadata_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_Metadata_Many) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto metadata = GListRange<metadata_x*>(app->metadata);
  ASSERT_EQ(Size(&metadata), 2);
  ASSERT_CSTR_EQ((*metadata.begin())->key, "key1");
  ASSERT_CSTR_EQ((*metadata.begin())->value, "value1");
  ASSERT_CSTR_EQ((*++metadata.begin())->key, "key2");
  ASSERT_CSTR_EQ((*++metadata.begin())->value, "");
}

TEST_F(ManifestTest, UIApplicationElement_BackgroundCategory_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto bg_categories = GListRange<char*>(app->background_category);
  ASSERT_EQ(Size(&bg_categories), 0);
}

TEST_F(ManifestTest, UIApplicationElement_BackgroundCategory_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto bg_categories = GListRange<char*>(app->background_category);
  ASSERT_EQ(Size(&bg_categories), 1);
  ASSERT_CSTR_EQ(*bg_categories.begin(), "bgcategory");
}

TEST_F(ManifestTest, UIApplicationElement_BackgroundCategory_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_BackgroundCategory_Many) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto bg_categories = GListRange<char*>(app->background_category);
  ASSERT_EQ(Size(&bg_categories), 2);
  ASSERT_CSTR_EQ(*bg_categories.begin(), "bgcategory1");
  ASSERT_CSTR_EQ(*++bg_categories.begin(), "bgcategory2");
}

TEST_F(ManifestTest, UIApplicationElement_DataControl_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto datacontrols = GListRange<datacontrol_x*>(app->datacontrol);
  ASSERT_EQ(Size(&datacontrols), 0);
}

TEST_F(ManifestTest, UIApplicationElement_DataControl_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto datacontrols = GListRange<datacontrol_x*>(app->datacontrol);
  ASSERT_EQ(Size(&datacontrols), 1);
  ASSERT_CSTR_EQ((*datacontrols.begin())->providerid, "providerid");
  ASSERT_CSTR_EQ((*datacontrols.begin())->access, "access");
  ASSERT_CSTR_EQ((*datacontrols.begin())->type, "type");
}

TEST_F(ManifestTest, UIApplicationElement_DataControl_Many) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto datacontrols = GListRange<datacontrol_x*>(app->datacontrol);
  ASSERT_EQ(Size(&datacontrols), 2);
  ASSERT_CSTR_EQ((*datacontrols.begin())->providerid, "providerid1");
  ASSERT_CSTR_EQ((*datacontrols.begin())->access, "access1");
  ASSERT_CSTR_EQ((*datacontrols.begin())->type, "type1");
  ASSERT_CSTR_EQ((*++datacontrols.begin())->providerid, "providerid2");
  ASSERT_CSTR_EQ((*++datacontrols.begin())->access, "access2");
  ASSERT_CSTR_EQ((*++datacontrols.begin())->type, "type2");
}

TEST_F(ManifestTest, UIApplicationElement_AppControl_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto appcontrols = GListRange<appcontrol_x*>(app->appcontrol);
  ASSERT_EQ(Size(&appcontrols), 0);
}

TEST_F(ManifestTest, UIApplicationElement_AppControl_Operation_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto appcontrols = GListRange<appcontrol_x*>(app->appcontrol);
  ASSERT_EQ(Size(&appcontrols), 0);
}

TEST_F(ManifestTest, UIApplicationElement_AppControl_Operation_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_AppControl_Operation_Invalid_NotURI) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_AppControl_Mime_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_AppControl_Uri_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_AppControl_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto appcontrols = GListRange<appcontrol_x*>(app->appcontrol);
  ASSERT_EQ(Size(&appcontrols), 1);
  ASSERT_CSTR_EQ((*appcontrols.begin())->operation,
                 "http://tizen.org/appcontrol/operation/compose");
  ASSERT_CSTR_EQ((*appcontrols.begin())->uri, "scheme://");
  ASSERT_CSTR_EQ((*appcontrols.begin())->mime, "application/pdf");
}

TEST_F(ManifestTest, UIApplicationElement_AppControl_Many) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto appcontrols = GListRange<appcontrol_x*>(app->appcontrol);
  ASSERT_EQ(Size(&appcontrols), 2);
  ASSERT_CSTR_EQ((*appcontrols.begin())->operation,
                 "http://tizen.org/appcontrol/operation/compose");
  ASSERT_CSTR_EQ((*appcontrols.begin())->uri, "scheme://");
  ASSERT_CSTR_EQ((*appcontrols.begin())->mime, "application/pdf");
  ASSERT_CSTR_EQ((*++appcontrols.begin())->operation,
                 "http://tizen.org/appcontrol/operation/compose2");
  ASSERT_CSTR_EQ((*++appcontrols.begin())->uri, "scheme2://");
  ASSERT_CSTR_EQ((*++appcontrols.begin())->mime, "application/text");
}

TEST_F(ManifestTest, UIApplicationElement_AppControl_SkipUri) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto appcontrols = GListRange<appcontrol_x*>(app->appcontrol);
  ASSERT_EQ(Size(&appcontrols), 1);
  ASSERT_CSTR_EQ((*appcontrols.begin())->operation,
                 "http://tizen.org/appcontrol/operation/compose");
  ASSERT_EQ((*appcontrols.begin())->uri, nullptr);
  ASSERT_CSTR_EQ((*appcontrols.begin())->mime, "application/pdf");
}

TEST_F(ManifestTest, UIApplicationElement_AppControl_SkipMime) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto appcontrols = GListRange<appcontrol_x*>(app->appcontrol);
  ASSERT_EQ(Size(&appcontrols), 1);
  ASSERT_CSTR_EQ((*appcontrols.begin())->operation,
                 "http://tizen.org/appcontrol/operation/compose");
  ASSERT_CSTR_EQ((*appcontrols.begin())->uri, "scheme://");
  ASSERT_EQ((*appcontrols.begin())->mime, nullptr);
}

TEST_F(ManifestTest, UIApplicationElement_AppControl_CartesianProduct) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto appcontrols = GListRange<appcontrol_x*>(app->appcontrol);
  ASSERT_EQ(Size(&appcontrols), 8);

  std::vector<boost::tuple<std::string, std::string, std::string>>
      found_app_controls;
  for (auto& item : appcontrols) {
    found_app_controls.emplace_back(item->operation, item->uri, item->mime);
  }

  for (auto operation : {"http://tizen.org/appcontrol/operation/compose1",
                         "http://tizen.org/appcontrol/operation/compose2"}) {
    for (auto uri : {"scheme1://", "scheme2://"}) {
      for (auto mime : {"application/text", "application/pdf"}) {
        ASSERT_IN(boost::make_tuple(operation, uri, mime), found_app_controls);
      }
    }
  }
}

TEST_F(ManifestTest,
       UIApplicationElement_AppControl_CartesianProduct_SkipMime) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto appcontrols = GListRange<appcontrol_x*>(app->appcontrol);
  ASSERT_EQ(Size(&appcontrols), 4);

  std::vector<boost::tuple<std::string, std::string, std::string>>
      found_app_controls;
  for (auto& item : appcontrols) {
    ASSERT_EQ(item->mime, nullptr);
    found_app_controls.emplace_back(item->operation, item->uri, "");
  }

  for (auto operation : {"http://tizen.org/appcontrol/operation/compose1",
                         "http://tizen.org/appcontrol/operation/compose2"}) {
    for (auto uri : {"scheme1://", "scheme2://"}) {
      ASSERT_IN(boost::make_tuple(operation, uri, ""), found_app_controls);
    }
  }
}

TEST_F(ManifestTest, UIApplicationElement_AppControl_CartesianProduct_SkipUri) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto appcontrols = GListRange<appcontrol_x*>(app->appcontrol);
  ASSERT_EQ(Size(&appcontrols), 4);

  std::vector<boost::tuple<std::string, std::string, std::string>>
      found_app_controls;
  for (auto& item : appcontrols) {
    ASSERT_EQ(item->uri, nullptr);
    found_app_controls.emplace_back(item->operation, "", item->mime);
  }

  for (auto operation : {"http://tizen.org/appcontrol/operation/compose1",
                         "http://tizen.org/appcontrol/operation/compose2"}) {
    for (auto mime : {"application/text", "application/pdf"}) {
      ASSERT_IN(boost::make_tuple(operation, "", mime), found_app_controls);
    }
  }
}

TEST_F(ManifestTest, UIApplicationElement_Icon_Missing) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto icons = GListRange<icon_x*>(app->icon);
  ASSERT_EQ(Size(&icons), 0);
}

TEST_F(ManifestTest, UIApplicationElement_Icon_Invalid) {
  StepParseRunner runner(GetMyName());
  ASSERT_FALSE(runner.Run());
}

TEST_F(ManifestTest, UIApplicationElement_Icon_Valid) {
  StepParseRunner runner(GetMyName());
  ASSERT_TRUE(runner.Run());
  manifest_x* m = runner.GetManifest();
  ASSERT_NE(m, nullptr);
  auto apps = GListRange<application_x*>(m->application);
  ASSERT_EQ(Size(&apps), 1);
  application_x* app = *apps.begin();
  auto icons = GListRange<icon_x*>(app->icon);
  ASSERT_EQ(Size(&icons), 1);
  ASSERT_EQ(bf::path((*icons.begin())->text).filename().string(),
            "test_icon.png");
}

// TODO(t.iwanek): there should be more tests for ui application icon but
// implementation differs much from the content on tizen.org so I have
// implemented only basic correct scenario test for icon
