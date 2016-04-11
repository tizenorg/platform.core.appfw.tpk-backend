// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/filesystem/step_tpk_prepare_package_directory.h"

#include <boost/filesystem/operations.hpp>
#include <boost/system/error_code.hpp>

#include <common/backup_paths.h>
#include <common/utils/file_util.h>

#include <vector>

namespace bf = boost::filesystem;
namespace bs = boost::system;
namespace ci = common_installer;

namespace {

const std::vector<std::string> kExtractEntries = {
  "bin/",
  "lib/",
  "shared/res/"
};

const std::vector<std::string> kSymlinkEntries = {
  "res/",
  "tizen-manifest.xml",
  "author-signature.xml",
  "signature1.xml",
  "signature2.xml"
};

// TODO(t.iwanek): fix tep support when mounting

bool ReplacePaths(const bf::path& source, const bf::path& destination) {
  if (!bf::exists(destination.parent_path())) {
    bs::error_code error;
    bf::create_directories(destination.parent_path(), error);
    if (error) {
      LOG(ERROR) << "Failed to create destination directory for directory "
                 << "backup";
      return false;
    }
  }
  if (!ci::MoveDir(source, destination)) {
    LOG(ERROR) << "Failed to move " << source << " to " << destination;
    return false;
  }
  return true;
}

}  // namespace

namespace tpk {
namespace filesystem {

ci::Step::Status StepTpkPreparePackageDirectory::PrepareDirectory(
    const std::string& entry, const bf::path& backup_path) {
  // make backup if directory exists
  if (bf::exists(context_->pkg_path.get() / entry)) {
    bf::path source = context_->pkg_path.get() / entry;
    bf::path destination = backup_path / entry;
    if (!ReplacePaths(source, destination))
      return Status::APP_DIR_ERROR;
  }

  // extract given entry
  if (!ci::ExtractToTmpDir(context_->file_path.get().c_str(),
                           context_->pkg_path.get(), entry)) {
    LOG(ERROR) << "Failed to extract from zip files in: " << entry;
    return Status::UNZIP_ERROR;
  }
  return Status::OK;
}

ci::Step::Status StepTpkPreparePackageDirectory::RestoreDirectory(
    const std::string& entry, const boost::filesystem::path& backup_path) {
  // restore backup if directory exists
  if (bf::exists(backup_path / entry)) {
    bf::path source = backup_path / entry;
    bf::path destination = context_->pkg_path.get() / entry;
    if (!ReplacePaths(source, destination))
      return Status::APP_DIR_ERROR;
  }
  return Status::OK;
}

ci::Step::Status StepTpkPreparePackageDirectory::PrepareLink(
    const std::string& entry) {
  bs::error_code error;
  bf::path mount_point_entry =
      ci::GetMountLocation(context_->pkg_path.get()) / entry;
  if (bf::exists(mount_point_entry)) {
    bf::path destination = context_->pkg_path.get() / entry;
    if (bf::exists(destination)) {
      if (!bf::is_symlink(destination)) {
        LOG(ERROR) << "Cannot proceed. "
                   << "Location of link is used by another file";
        return Status::APP_DIR_ERROR;
      }
      bf::remove(destination, error);
      if (error) {
        LOG(ERROR) << "Failed to remove previous symlink";
        return Status::APP_DIR_ERROR;
      }
    }
    bf::create_symlink(mount_point_entry,
                       destination, error);
    if (error) {
      LOG(ERROR) << "Failed to create symlink for entry: " << mount_point_entry;
      return Status::APP_DIR_ERROR;
    }
  }
  return Status::OK;
}

ci::Step::Status StepTpkPreparePackageDirectory::PreparePackageDirectory() {
  bf::path backup_path =
      ci::GetBackupPathForPackagePath(context_->pkg_path.get());

  for (auto& entry : kExtractEntries) {
    auto status = PrepareDirectory(entry, backup_path);
    if (status != Status::OK)
      return status;
  }

  for (auto& link_entry : kSymlinkEntries) {
    auto status = PrepareLink(link_entry);
    if (status != Status::OK)
      return status;
  }

  return Status::OK;
}

ci::Step::Status StepTpkPreparePackageDirectory::process() {
  return PreparePackageDirectory();
}

ci::Step::Status StepTpkPreparePackageDirectory::clean() {
  bf::path backup_path =
      ci::GetBackupPathForPackagePath(context_->pkg_path.get());
  if (bf::exists(backup_path)) {
    bs::error_code error;
    bf::remove(backup_path, error);
    if (error) {
      LOG(ERROR) << "Failed to remove backup path: " << backup_path;
      return Status::APP_DIR_ERROR;
    }
  }
  return Status::OK;
}

ci::Step::Status StepTpkPreparePackageDirectory::undo() {
  bf::path backup_path =
      ci::GetBackupPathForPackagePath(context_->pkg_path.get());

  for (auto& entry : kExtractEntries) {
    RestoreDirectory(entry, backup_path);
  }
  return Status::OK;
}

ci::Step::Status StepTpkPreparePackageDirectory::precheck() {
  if (context_->pkg_path.get().empty()) {
    LOG(ERROR) << "Package installation path is not set";
    return Status::INVALID_VALUE;
  }
  return Status::OK;
}

}  // namespace filesystem
}  // namespace tpk
