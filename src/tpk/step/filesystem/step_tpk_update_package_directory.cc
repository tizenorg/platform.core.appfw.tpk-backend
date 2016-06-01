// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/filesystem/step_tpk_update_package_directory.h"

#include <boost/filesystem/operations.hpp>
#include <boost/system/error_code.hpp>

#include <common/backup_paths.h>
#include <common/utils/file_util.h>

#include <vector>

#include "tpk/tpk_mount_path.h"

namespace bf = boost::filesystem;
namespace bs = boost::system;
namespace ci = common_installer;

namespace {

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

ci::Step::Status StepTpkUpdatePackageDirectory::BackupDirectory(
    const std::string& entry, const boost::filesystem::path& backup_path) {
  bf::path source = context_->pkg_path.get() / entry;
  if (bf::exists(source)) {
    bf::path destination = backup_path / entry;
    if (!ReplacePaths(source, destination))
      return Status::APP_DIR_ERROR;
  }
  return Status::OK;
}

ci::Step::Status StepTpkUpdatePackageDirectory::RestoreDirectory(
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

ci::Step::Status StepTpkUpdatePackageDirectory::BackupEntries() {
  bf::path backup_path =
      ci::GetBackupPathForPackagePath(context_->pkg_path.get());
  for (auto& entry : tpk::GetExtractEntries()) {
    auto status = BackupDirectory(entry, backup_path);
    if (status != Status::OK)
      return status;
  }
  return Status::OK;
}

ci::Step::Status StepTpkUpdatePackageDirectory::process() {
  auto status = BackupEntries();
  if (status != Status::OK)
    return status;
  status = ExtractEntries();
  if (status != Status::OK)
    return status;
  return PrepareLinks();
}

ci::Step::Status StepTpkUpdatePackageDirectory::clean() {
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

ci::Step::Status StepTpkUpdatePackageDirectory::undo() {
  bf::path backup_path =
      ci::GetBackupPathForPackagePath(context_->pkg_path.get());

  for (auto& entry : tpk::GetExtractEntries()) {
    auto status = RestoreDirectory(entry, backup_path);
    if (status != Status::OK)
      return status;
  }
  return Status::OK;
}

}  // namespace filesystem
}  // namespace tpk
