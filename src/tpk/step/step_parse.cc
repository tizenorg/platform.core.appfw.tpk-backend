// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/step_parse.h"

#include <common/app_installer.h>
#include <common/installer_context.h>
#include <common/step/step.h>
#include <common/utils/glist_range.h>

#include <tpk_manifest_handlers/account_handler.h>
#include <tpk_manifest_handlers/application_manifest_constants.h>
#include <tpk_manifest_handlers/author_handler.h>
#include <tpk_manifest_handlers/description_handler.h>
#include <tpk_manifest_handlers/package_handler.h>
#include <tpk_manifest_handlers/privileges_handler.h>
#include <tpk_manifest_handlers/profile_handler.h>
#include <tpk_manifest_handlers/service_application_handler.h>
#include <tpk_manifest_handlers/shortcut_handler.h>
#include <tpk_manifest_handlers/ui_application_handler.h>
#include <tpk_manifest_handlers/widget_application_handler.h>

#include <pkgmgr/pkgmgr_parser.h>
#include <pkgmgr-info.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <set>
#include <type_traits>
#include <string>
#include <vector>


namespace tpk {
namespace parse {

namespace {

namespace app_keys = tpk::application_keys;
namespace bf = boost::filesystem;

const char kManifestFileName[] = "tizen-manifest.xml";

}  // namepsace

common_installer::Step::Status StepParse::precheck() {
  if (context_->unpacked_dir_path.get().empty()) {
    LOG(ERROR) << "unpacked_dir_path attribute is empty";
    return common_installer::Step::Status::INVALID_VALUE;
  }
  if (!boost::filesystem::exists(context_->unpacked_dir_path.get())) {
    LOG(ERROR) << "unpacked_dir_path ("
               << context_->unpacked_dir_path.get()
               << ") path does not exist";
    return common_installer::Step::Status::INVALID_VALUE;
  }

  boost::filesystem::path tmp;
  if (!context_->xml_path.get().empty()) {
    tmp = context_->xml_path.get();
  } else {
    tmp = context_->unpacked_dir_path.get();
    tmp /= kManifestFileName;
  }

  if (!boost::filesystem::exists(tmp)) {
    LOG(ERROR) << "manifest not found from the package";
    return common_installer::Step::Status::MANIFEST_NOT_FOUND;
  }

  return common_installer::Step::Status::OK;
}

// TODO(jungh.yeon) : this function should be re-considered
bf::path StepParse::FindIcon(const std::string& filename) {
  bf::path icon_path;
  bf::path app_path;

  if (filename.length() == 0)
    return icon_path;

  if (index(filename.c_str(), '/'))
    return filename;

  // FIXME: icons for preloaded apps should also be moved to "shared/res"
  icon_path = bf::path(getIconPath(context_->uid.get())) / filename;
  if (access(icon_path.c_str(), F_OK) == 0)
    return icon_path;

  icon_path = bf::path(getIconPath(context_->uid.get()) /
              bf::path("default/small") / filename);
  if (bf::exists(icon_path))
    return icon_path;

  if (context_->uid.get() == GLOBAL_USER) {
    app_path = tzplatform_getenv(TZ_SYS_RW_APP);
  } else {
    tzplatform_set_user(context_->uid.get());
    app_path = tzplatform_getenv(TZ_USER_APP);
    tzplatform_reset_user();
  }

  icon_path =
      context_->unpacked_dir_path.get() / bf::path("shared/res") / filename;
  if (bf::exists(icon_path)) {
    icon_path =
        app_path / context_->pkgid.get() / bf::path("shared/res") / filename;
    return icon_path;
  }

  icon_path =
      context_->unpacked_dir_path.get() / bf::path("res/icons") / filename;
  if (bf::exists(icon_path)) {
    icon_path =
        app_path / context_->pkgid.get() / bf::path("res/icons") / filename;
    return icon_path;
  }

  icon_path = context_->unpacked_dir_path.get() / filename;
  if (bf::exists(icon_path)) {
    icon_path = app_path / context_->pkgid.get() / filename;
    return icon_path;
  }

  icon_path = "";
  return icon_path;
}

bool StepParse::LocateConfigFile() {
  boost::filesystem::path manifest;
  if (!context_->xml_path.get().empty()) {
    manifest = context_->xml_path.get();
  } else {
    manifest = context_->unpacked_dir_path.get();
    manifest /= kManifestFileName;
  }

  LOG(DEBUG) << "manifest path: " << manifest;

  if (!boost::filesystem::exists(manifest))
    return false;

  path_ = manifest;
  return true;
}

bf::path StepParse::LocateConfigFile() const {
  boost::filesystem::path path;
  if (!context_->xml_path.get().empty()) {
    path = context_->xml_path.get();
  } else {
    path = context_->unpacked_dir_path.get();
    path /= kManifestFileName;
  }
  return path;
}

bool StepParse::FillInstallationInfo(manifest_x* manifest) {
  manifest->root_path = strdup(
      (context_->root_application_path.get() / manifest->package).c_str());
  manifest->installed_time =
      strdup(std::to_string(std::chrono::system_clock::to_time_t(
          std::chrono::system_clock::now())).c_str());
  return true;
}

bool StepParse::FillPackageInfo(manifest_x* manifest) {
  std::shared_ptr<const PackageInfo> pkg_info =
      std::static_pointer_cast<const PackageInfo>(
          parser_->GetManifestData(app_keys::kManifestKey));
  if (!pkg_info) {
    LOG(ERROR) << "Package info manifest data has not been found.";
    return false;
  }

  std::shared_ptr<const UIApplicationInfoList> ui_application_list =
      std::static_pointer_cast<const UIApplicationInfoList>(
          parser_->GetManifestData(app_keys::kUIApplicationKey));
  std::shared_ptr<const ServiceApplicationInfoList> service_application_list =
      std::static_pointer_cast<const ServiceApplicationInfoList>(
          parser_->GetManifestData(app_keys::kServiceApplicationKey));
  std::shared_ptr<const WidgetApplicationInfoList> widget_application_list =
      std::static_pointer_cast<const WidgetApplicationInfoList>(
          parser_->GetManifestData(app_keys::kWidgetApplicationKey));

  // mandatory check
  if (!ui_application_list && !service_application_list &&
      !widget_application_list) {
    LOG(ERROR) << "UI Application or Service Application or Widget Application "
                  "are mandatory and has not been found.";
    return false;
  }

  manifest->ns = strdup(pkg_info->xmlns().c_str());
  manifest->package = strdup(pkg_info->package().c_str());
  manifest->nodisplay_setting = strdup(pkg_info->nodisplay_setting().c_str());
  manifest->appsetting = strdup("false");
  manifest->support_disable = strdup("false");
  manifest->version = strdup(pkg_info->version().c_str());
  manifest->installlocation = strdup(pkg_info->install_location().c_str());
  manifest->api_version = strdup(pkg_info->api_version().c_str());
  if (context_->pkg_type.get().compare("rpm") == 0)
    manifest->type = strdup("rpm");
  else
    manifest->type = strdup("tpk");

  for (auto& pair : pkg_info->labels()) {
    label_x* label = reinterpret_cast<label_x*>(calloc(1, sizeof(label_x)));
    if (!pair.first.empty())
      label->lang = strdup(pair.first.c_str());
    else
      label->lang = strdup(DEFAULT_LOCALE);
    label->name = strdup(pair.second.c_str());
    manifest->label = g_list_append(manifest->label, label);
  }

  std::shared_ptr<const ProfileInfo> profile_info =
      std::static_pointer_cast<const ProfileInfo>(
          parser_->GetManifestData(ProfileInfo::Key()));
  if (profile_info) {
    for (auto& profile : profile_info->profiles()) {
      manifest->deviceprofile = g_list_append(manifest->deviceprofile,
                                              strdup(profile.c_str()));
    }
  }

  if (ui_application_list) {
    manifest->mainapp_id =
        strdup(ui_application_list->items[0].ui_info.appid().c_str());
  } else if (service_application_list) {
    manifest->mainapp_id =
        strdup(service_application_list->items[0].sa_info.appid().c_str());
  } else if (widget_application_list) {
    manifest->mainapp_id =
        strdup(widget_application_list->items[0].widget_info.appid().c_str());
  }
  return true;
}

bool StepParse::FillAuthorInfo(manifest_x* manifest) {
  std::shared_ptr<const AuthorInfo> author_info =
      std::static_pointer_cast<const AuthorInfo>(
          parser_->GetManifestData(app_keys::kAuthorKey));

  if (!author_info) {
    LOG(ERROR) << "Author data has not been found.";
    return false;
  }

  author_x* author = reinterpret_cast<author_x*>(calloc(1, sizeof(author_x)));
  author->text = strdup(author_info->name().c_str());
  author->email = strdup(author_info->email().c_str());
  author->href = strdup(author_info->href().c_str());
  author->lang = strdup(DEFAULT_LOCALE);
  manifest->author = g_list_append(manifest->author, author);
  return true;
}

bool StepParse::FillDescription(manifest_x* manifest) {
  std::shared_ptr<const DescriptionInfo> description_info =
      std::static_pointer_cast<const DescriptionInfo>(
          parser_->GetManifestData(app_keys::kDescriptionKey));

  if (!description_info) {
    LOG(ERROR) << "Description data has not been found.";
    return false;
  }

  description_x* description = reinterpret_cast<description_x*>
      (calloc(1, sizeof(description_x)));
  description->text = strdup(description_info->description().c_str());
  description->lang = !description_info->xml_lang().empty() ?
      strdup(description_info->xml_lang().c_str()) : strdup(DEFAULT_LOCALE);
  manifest->description = g_list_append(manifest->description, description);
  return true;
}

bool StepParse::FillPrivileges(manifest_x* manifest) {
  std::shared_ptr<const PrivilegesInfo> perm_info =
      std::static_pointer_cast<const PrivilegesInfo>(parser_->GetManifestData(
          app_keys::kPrivilegesKey));
  if (!perm_info)
    return true;

  std::set<std::string> privileges = perm_info->GetPrivileges();
  for (auto& priv : privileges) {
    manifest->privileges = g_list_append(manifest->privileges,
                                         strdup(priv.c_str()));
  }
  return true;
}

bool StepParse::FillWidgetApplication(manifest_x* manifest) {
  std::shared_ptr<const WidgetApplicationInfoList> widget_application_list =
      std::static_pointer_cast<const WidgetApplicationInfoList>(
          parser_->GetManifestData(app_keys::kWidgetApplicationKey));
  if (!widget_application_list)
    return true;

  for (const auto& application : widget_application_list->items) {
    // if there is no app yet, set this app as mainapp
    bool main_app = manifest->application == nullptr;

    application_x* widget_app =
        static_cast<application_x*>(calloc(1, sizeof(application_x)));
    widget_app->appid = strdup(application.widget_info.appid().c_str());
    widget_app->launch_mode =
        strdup(application.widget_info.launch_mode().c_str());
    widget_app->multiple = strdup(application.widget_info.multiple().c_str());
    widget_app->nodisplay =
        strdup(application.widget_info.nodisplay().c_str());
    widget_app->type = strdup("capp");
    widget_app->component_type = strdup("widgetapp");
    widget_app->hwacceleration =
        strdup(application.widget_info.hwacceleration().c_str());
    widget_app->onboot = strdup("false");
    widget_app->autorestart = strdup("false");
    widget_app->mainapp = main_app ? strdup("true") : strdup("false");
    widget_app->enabled = strdup("true");
    widget_app->screenreader = strdup("use-system-setting");
    widget_app->recentimage = strdup("false");
    widget_app->launchcondition = strdup("false");
    widget_app->guestmode_visibility = strdup("true");
    widget_app->permission_type = strdup("normal");
    widget_app->ambient_support = strdup("false");
    widget_app->package = strdup(manifest->package);
    widget_app->support_disable = strdup(manifest->support_disable);
    manifest->application = g_list_append(manifest->application, widget_app);
    if (strncmp(context_->pkg_type.get().c_str(), "rpm", strlen("rpm")) == 0)
      widget_app->exec = strdup(application.widget_info.exec().c_str());
    else
      widget_app->exec = strdup((context_->root_application_path.get()
                            / manifest->package / "bin"
                            / application.widget_info.exec()).c_str());

    if (!FillApplicationIconPaths(widget_app, application.app_icons))
      return false;
    if (!FillLabel(widget_app, application.label))
      return false;
    if (!FillImage(widget_app, application.app_images))
      return false;
    if (!FillMetadata(widget_app, application.meta_data))
      return false;
  }
  return true;
}

bool StepParse::FillServiceApplication(manifest_x* manifest) {
  std::shared_ptr<const ServiceApplicationInfoList> service_application_list =
      std::static_pointer_cast<const ServiceApplicationInfoList>(
          parser_->GetManifestData(app_keys::kServiceApplicationKey));
  if (!service_application_list)
    return true;

  for (const auto& application : service_application_list->items) {
    // if there is no app yet, set this app as mainapp
    bool main_app = manifest->application == nullptr;

    application_x* service_app =
        static_cast<application_x*>(calloc(1, sizeof(application_x)));
    service_app->appid = strdup(application.sa_info.appid().c_str());
    service_app->autorestart =
        strdup(application.sa_info.auto_restart().c_str());
    service_app->onboot = strdup(application.sa_info.on_boot().c_str());
    service_app->type = strdup(application.sa_info.type().c_str());
    service_app->process_pool =
        strdup(application.sa_info.process_pool().c_str());
    service_app->component_type = strdup("svcapp");
    service_app->mainapp = main_app ? strdup("true") : strdup("false");
    service_app->enabled = strdup("true");
    service_app->hwacceleration = strdup("default");
    service_app->screenreader = strdup("use-system-setting");
    service_app->recentimage = strdup("false");
    service_app->launchcondition = strdup("false");
    service_app->indicatordisplay = strdup("true");
    service_app->effectimage_type = strdup("image");
    service_app->guestmode_visibility = strdup("true");
    service_app->permission_type = strdup("normal");
    service_app->submode = strdup("false");
    service_app->process_pool = strdup("false");
    service_app->ambient_support = strdup("false");
    service_app->package = strdup(manifest->package);
    service_app->support_disable = strdup(manifest->support_disable);
    manifest->application = g_list_append(manifest->application, service_app);
    if (strncmp(context_->pkg_type.get().c_str(), "rpm", strlen("rpm")) == 0)
      service_app->exec = strdup(application.sa_info.exec().c_str());
    else
      service_app->exec = strdup((context_->root_application_path.get()
                            / manifest->package / "bin"
                            / application.sa_info.exec()).c_str());

    if (!FillAppControl(service_app,  application.app_control))
      return false;
    if (!FillDataControl(service_app, application.data_control))
      return false;
    if (!FillApplicationIconPaths(service_app, application.app_icons))
      return false;
    if (!FillLabel(service_app, application.label))
      return false;
    if (!FillMetadata(service_app, application.meta_data))
      return false;
    if (!FillBackgroundCategoryInfo(service_app,
        application.background_category))
      return false;
  }
  return true;
}

bool StepParse::FillUIApplication(manifest_x* manifest) {
  std::shared_ptr<const UIApplicationInfoList> ui_application_list =
      std::static_pointer_cast<const UIApplicationInfoList>(
          parser_->GetManifestData(app_keys::kUIApplicationKey));
  if (!ui_application_list)
    return true;

  for (const auto& application : ui_application_list->items) {
    // if there is no app yet, set this app as mainapp
    bool main_app = manifest->application == nullptr;

    application_x* ui_app =
        static_cast<application_x*>(calloc(1, sizeof(application_x)));
    ui_app->appid = strdup(application.ui_info.appid().c_str());
    ui_app->launch_mode = strdup(application.ui_info.launch_mode().c_str());
    ui_app->multiple = strdup(application.ui_info.multiple().c_str());
    ui_app->nodisplay = strdup(application.ui_info.nodisplay().c_str());
    ui_app->taskmanage = strdup(application.ui_info.taskmanage().c_str());
    ui_app->type = strdup(application.ui_info.type().c_str());
    ui_app->component_type = strdup("uiapp");
    ui_app->ui_gadget = strdup(application.ui_info.uigadget().c_str());
    ui_app->process_pool = strdup(application.ui_info.process_pool().c_str());
    ui_app->submode = strdup(application.ui_info.submode().c_str());
    ui_app->indicatordisplay =
        strdup(application.ui_info.indicator_display().c_str());
    ui_app->effectimage_type =
        strdup(application.ui_info.effectimage_type().c_str());
    ui_app->portraitimg =
        strdup(application.ui_info.portrait_image().c_str());
    ui_app->landscapeimg =
        strdup(application.ui_info.landscape_image().c_str());
    ui_app->submode_mainid =
        strdup(application.ui_info.submode_mainid().c_str());
    ui_app->hwacceleration =
        strdup(application.ui_info.hwacceleration().c_str());
    ui_app->onboot = strdup("false");
    ui_app->autorestart = strdup("false");
    ui_app->component_type = strdup("uiapp");
    ui_app->mainapp = main_app ? strdup("true") : strdup("false");
    ui_app->enabled = strdup("true");
    ui_app->screenreader = strdup("use-system-setting");
    ui_app->recentimage = strdup("false");
    ui_app->launchcondition = strdup("false");
    ui_app->guestmode_visibility = strdup("true");
    ui_app->permission_type = strdup("normal");
    ui_app->ambient_support = strdup("false");
    ui_app->package = strdup(manifest->package);
    ui_app->support_disable = strdup(manifest->support_disable);
    manifest->application = g_list_append(manifest->application, ui_app);
    if (strncmp(context_->pkg_type.get().c_str(), "rpm", strlen("rpm")) == 0)
      ui_app->exec = strdup(application.ui_info.exec().c_str());
    else
      ui_app->exec = strdup((context_->root_application_path.get()
                            / manifest->package / "bin"
                            / application.ui_info.exec()).c_str());


    if (!FillAppControl(ui_app, application.app_control))
      return false;
    if (!FillDataControl(ui_app, application.data_control))
      return false;
    if (!FillApplicationIconPaths(ui_app, application.app_icons))
      return false;
    if (!FillLabel(ui_app, application.label))
      return false;
    if (!FillImage(ui_app, application.app_images))
      return false;
    if (!FillMetadata(ui_app, application.meta_data))
      return false;
    if (!FillBackgroundCategoryInfo(ui_app, application.background_category))
      return false;
  }
  return true;
}

template <typename T>
bool StepParse::FillAppControl(application_x* app, const T& app_control_list) {
  if (app_control_list.empty())
    return true;

  for (const auto& control : app_control_list) {
    appcontrol_x* app_control =
          static_cast<appcontrol_x*>(calloc(1, sizeof(appcontrol_x)));
    app_control->operation = strdup(control.operation().c_str());
    if (!control.mime().empty())
      app_control->mime = strdup(control.mime().c_str());
    if (!control.uri().empty())
      app_control->uri = strdup(control.uri().c_str());
    app->appcontrol = g_list_append(app->appcontrol, app_control);
  }
  return true;
}

template <typename T>
bool StepParse::FillDataControl(application_x* app,
                                const T& data_control_list) {
  if (data_control_list.empty())
    return true;

  for (const auto& control : data_control_list) {
    datacontrol_x* data_control =
          static_cast<datacontrol_x*>(calloc(1, sizeof(datacontrol_x)));
    data_control->access = strdup(control.access().c_str());
    data_control->providerid = strdup(control.providerid().c_str());
    data_control->type = strdup(control.type().c_str());
    app->datacontrol = g_list_append(app->datacontrol, data_control);
  }
  return true;
}

template <typename T>
bool StepParse::FillApplicationIconPaths(application_x* app,
                                         const T& icons_info) {
  for (auto& application_icon : icons_info.icons()) {
    icon_x* icon = reinterpret_cast<icon_x*> (calloc(1, sizeof(icon_x)));
    // NOTE: name is an attribute, but the xml writer uses it as text.
    // This must be fixed in whole app-installer modules, including wgt.
    // Current implementation is just for compatibility.
    icon->text = strdup(FindIcon(application_icon.path()).c_str());
    icon->name = strdup(application_icon.path().c_str());
    icon->lang = strdup(DEFAULT_LOCALE);
    app->icon = g_list_append(app->icon, icon);
  }
  return true;
}

template <typename T>
bool StepParse::FillLabel(application_x* app, const T& label_list) {
  if (label_list.empty())
    return true;

  for (const auto& control : label_list) {
    label_x* label =
          static_cast<label_x*>(calloc(1, sizeof(label_x)));
    // NOTE: name is an attribute, but the xml writer uses it as text.
    // This must be fixed in whole app-installer modules, including wgt.
    // Current implementation is just for compatibility.
    label->text = strdup(control.text().c_str());
    label->name = strdup(control.name().c_str());
    label->lang = !control.xml_lang().empty() ?
        strdup(control.xml_lang().c_str()) : strdup(DEFAULT_LOCALE);
    app->label = g_list_append(app->label, label);
  }
  return true;
}

template <typename T>
bool StepParse::FillMetadata(application_x* app, const T& meta_data_list) {
  if (meta_data_list.empty())
    return true;

  for (auto& meta : meta_data_list) {
    metadata_x* metadata =
        static_cast<metadata_x*>(calloc(1, sizeof(metadata_x)));
    metadata->key = strdup(meta.key().c_str());
    metadata->value = strdup(meta.val().c_str());
    app->metadata = g_list_append(app->metadata, metadata);
  }
  return true;
}

bool StepParse::FillImage(application_x* app,
                          const tpk::parse::ApplicationImagesInfo& image_list) {
  for (auto& app_image : image_list.images) {
    image_x* image =
        static_cast<image_x*>(calloc(1, sizeof(image_x)));
    image->name = strdup(app_image.name().c_str());
    const std::string& lang = app_image.lang();
    if (!lang.empty())
      image->lang = strdup(lang.c_str());
    else
      image->lang = strdup(DEFAULT_LOCALE);
    if (!app_image.section().empty())
      image->section = strdup(app_image.section().c_str());
    app->image = g_list_append(app->image, image);
  }
  return true;
}

bool StepParse::FillAccounts() {
  std::shared_ptr<const AccountInfo> account_info =
      std::static_pointer_cast<const AccountInfo>(parser_->GetManifestData(
          app_keys::kAccountKey));
  if (!account_info)
    return true;

  common_installer::AccountInfo info;
  for (auto& account : account_info->accounts()) {
    common_installer::SingleAccountInfo single_info;
    single_info.capabilities = account.capabilities;
    single_info.icon_paths = account.icon_paths;
    single_info.multiple_account_support = account.multiple_account_support;
    single_info.names = account.labels;
    // appid has the same value as package
    single_info.appid =  account.app_id;
    single_info.providerid = account.provider_id;
    info.set_account(single_info);
  }
  context_->manifest_plugins_data.get().account_info.set(info);
  return true;
}

bool StepParse::FillShortcuts() {
  std::shared_ptr<const ShortcutListInfo> shortcut_info =
      std::static_pointer_cast<const ShortcutListInfo>(parser_->GetManifestData(
          app_keys::kShortcutListKey));
  if (!shortcut_info)
    return true;

  common_installer::ShortcutListInfo list;
  for (auto& shortcut : shortcut_info->shortcuts) {
    common_installer::ShortcutInfo single_info;
    single_info.app_id = shortcut.app_id;
    single_info.extra_data = shortcut.extra_data;
    single_info.extra_key = shortcut.extra_key;
    single_info.icon = shortcut.icon;
    single_info.labels =  shortcut.labels;
    list.push_back(single_info);
  }
  context_->manifest_plugins_data.get().shortcut_info.set(list);
  return true;
}

template <typename T>
bool StepParse::FillBackgroundCategoryInfo(application_x* app,
    const T& background_category_data_list) {
  for (const auto& background_category : background_category_data_list) {
    app->background_category = g_list_append(
        app->background_category, strdup(background_category.value().c_str()));
  }

  return true;
}

bool StepParse::FillManifestX(manifest_x* manifest) {
  if (!FillPackageInfo(manifest))
    return false;
  if (!FillInstallationInfo(manifest))
    return false;
  if (!FillUIApplication(manifest))
    return false;
  if (!FillServiceApplication(manifest))
    return false;
  if (!FillWidgetApplication(manifest))
    return false;
  if (!FillPrivileges(manifest))
    return false;
  if (!FillAccounts())
    return false;
  if (!FillShortcuts())
    return false;
  return true;
}

common_installer::Step::Status StepParse::process() {
  if (!LocateConfigFile()) {
    LOG(ERROR) << "No manifest file exists";
    return common_installer::Step::Status::MANIFEST_NOT_FOUND;
  }
  parser_.reset(new tpk::parse::TPKConfigParser());
  if (!parser_->ParseManifest(path_)) {
    LOG(ERROR) << "[Parse] Parse failed. " <<  parser_->GetErrorMessage();
    return common_installer::Step::Status::PARSE_ERROR;
  }

  // Copy data from ManifestData to InstallerContext
  std::shared_ptr<const PackageInfo> info =
      std::static_pointer_cast<const PackageInfo>(
          parser_->GetManifestData(
              app_keys::kManifestKey));

  context_->pkgid.set(info->package());

  manifest_x* manifest =
      static_cast<manifest_x*>(calloc(1, sizeof(manifest_x)));

  if (!FillManifestX(const_cast<manifest_x*>(manifest))) {
    LOG(ERROR) << "[Parse] Storing manifest_x failed. "
               <<  parser_->GetErrorMessage();
    return common_installer::Step::Status::PARSE_ERROR;
  }

  if (!context_->tep_path.get().empty())
    manifest->tep_name = context_->tep_path.get().c_str();

  // write pkgid for recovery file
  if (context_->recovery_info.get().recovery_file) {
    context_->recovery_info.get().recovery_file->set_pkgid(manifest->package);
    context_->recovery_info.get().recovery_file->WriteAndCommitFileContent();
  }

  std::shared_ptr<const PrivilegesInfo> perm_info =
      std::static_pointer_cast<const PrivilegesInfo>(
          parser_->GetManifestData(
              application_keys::kPrivilegesKey));
  parser::PrivilegesSet privileges;
  if (perm_info)
    privileges = perm_info->GetPrivileges();

  std::shared_ptr<const UIApplicationInfoList> ui_application_list =
      std::static_pointer_cast<const UIApplicationInfoList>(
          parser_->GetManifestData(app_keys::kUIApplicationKey));

  LOG(DEBUG) << " Read data -[ ";
  LOG(DEBUG) << "App package: " << info->package();
  LOG(DEBUG) << "  aplication version     = " <<  info->version();
  LOG(DEBUG) << "  api_version = " <<  info->api_version();
  if (ui_application_list) {
    LOG(DEBUG) << "  launch_modes -[";
    for (const auto& application : ui_application_list->items) {
      LOG(DEBUG) << "    launch_mode[" << application.ui_info.appid() << "] = "
        <<  application.ui_info.launch_mode();
    }
  }
  LOG(DEBUG) << "  ]-";
  LOG(DEBUG) << "  privileges -[";
  for (const auto& p : privileges) {
    LOG(DEBUG) << "    " << p;
  }
  LOG(DEBUG) << "  ]-";
  LOG(DEBUG) << "]-";

  context_->manifest_data.set(manifest);
  return common_installer::Step::Status::OK;
}

}  // namespace parse
}  // namespace tpk
