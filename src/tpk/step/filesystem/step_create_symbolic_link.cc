/* Copyright 2015 Samsung Electronics, license APACHE-2.0, see LICENSE file */

#include "tpk/step/filesystem/step_create_symbolic_link.h"

#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>

#include <common/app_installer.h>
#include <common/step/step.h>
#include <common/installer_context.h>
#include <common/utils/file_util.h>
#include <common/utils/glist_range.h>

#include <pkgmgr-info.h>

#include <iostream>

namespace tpk {
namespace filesystem {

namespace bf = boost::filesystem;
namespace ci = common_installer;

using common_installer::InstallerContext;
typedef common_installer::Step::Status Status;

bool CreateSymLink(application_x* app, InstallerContext* context) {
  boost::system::error_code boost_error;
  bf::path bindir = context->package_storage->path() /
      bf::path("bin");
  LOG(DEBUG) << "Creating dir: " << bindir;
  if (!common_installer::CreateDir(bindir)) {
    LOG(ERROR) << "Directory creation failure: " << bindir;
    return false;
  }

  if (app->ui_gadget && strcmp(app->ui_gadget, "true") == 0) {
    // Ug-client path
    // Make a symlink with the name of appid, pointing /usr/bin/ug-client
    bf::path app_exec_path(app->exec);
    if (!bf::exists(app_exec_path)) {
      bf::path ug_client_path(tzplatform_mkpath(TZ_SYS_BIN, "ug-client"));
      LOG(INFO) << "Createing symlink " << app_exec_path << " pointing " <<
        ug_client_path;
      bf::create_symlink(ug_client_path, app_exec_path, boost_error);
      if (boost_error) {
        LOG(ERROR) << "Symlink creation failure: " << app_exec_path;
        return false;
      }
    }
  } else {
    if (context->request_type.get() == ci::RequestType::ManifestDirectInstall ||
        context->request_type.get() == ci::RequestType::ManifestDirectUpdate) {
      return true;
    }

    // Exec path
    // Make a symlink with the name of appid, pointing exec file
    bf::path symlink_path = bindir / bf::path(app->appid);
    bf::path app_exec_path(app->exec);
    if (!bf::exists(symlink_path) && bf::exists(app_exec_path)) {
      LOG(DEBUG) << "Creating symlink " << symlink_path << " pointing " <<
          app_exec_path;
      bf::create_symlink(app_exec_path, symlink_path, boost_error);
      if (boost_error) {
        LOG(ERROR) << "Symlink creation failure: " << symlink_path;
        return false;
      }
    }
  }

  return true;
}

bool SetExecPermission(application_x* app) {
  boost::system::error_code boost_error;
  // Give an execution permission to the original executable
  LOG(DEBUG) << "Giving exec permission to " << app->exec;
  bf::path app_exec_path(app->exec);
  if (bf::exists(app_exec_path)) {
    bf::permissions(app_exec_path, bf::owner_all |
        bf::group_read | bf::group_exe |
        bf::others_read | bf::others_exe, boost_error);
    if (boost_error) {
      LOG(ERROR) << "Permission change failure";
      return false;
    }
  } else {
    LOG(WARNING) << "file does not exist";
  }

  return true;
}

Status StepCreateSymbolicLink::precheck() {
  manifest_x *m = context_->manifest_data.get();
  if (!m) {
    LOG(ERROR) << "manifest_data attribute is empty";
    return Step::Status::INVALID_VALUE;
  }
  if (!m->application) {
    LOG(ERROR) << "No application exists";
    return Step::Status::ERROR;
  }

  return Step::Status::OK;
}

Status StepCreateSymbolicLink::process() {
  manifest_x* m = context_->manifest_data.get();
  for (application_x* app : GListRange<application_x*>(m->application)) {
    // filter out non-tpk apps as this step is run for hybrid backend too
    if (strcmp("capp", app->type) != 0 && strcmp("jsapp", app->type) != 0)
      continue;
    if (!CreateSymLink(app, context_))
      return Status::ERROR;
    if (!SetExecPermission(app))
      return Status::ERROR;
  }
  return Status::OK;
}

}  // namespace filesystem
}  // namespace tpk
