// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/filesystem/step_tpk_prepare_package_directory.h"

#include <boost/filesystem/operations.hpp>
#include <boost/system/error_code.hpp>

#include <common/backup_paths.h>
#include <common/utils/file_util.h>

namespace bf = boost::filesystem;
namespace bs = boost::system;
namespace ci = common_installer;

namespace tpk {
namespace filesystem {

ci::Step::Status StepTpkPreparePackageDirectory::PrepareDirectory(
    const std::string& entry) {
  // Due to the fact that boost fails to return correct parent_path() then path
  // with ending '/' is passed, kExtractEntries has not ending slash
  // here, we need to filter only directories and ci::ExtractToTmpDir() has
  // simple string filter so we append slash character here
  std::string filter = entry + '/';
  if (!ci::ExtractToTmpDir(context_->file_path.get().c_str(),
                           context_->pkg_path.get(), entry)) {
    LOG(ERROR) << "Failed to extract from zip files in: " << filter;
    return Status::UNZIP_ERROR;
  }
  return Status::OK;
}

ci::Step::Status StepTpkPreparePackageDirectory::PrepareLink(
    const std::string& entry, const bf::path& mount_point) {
  bs::error_code error;
  bf::path mount_point_entry = mount_point / entry;
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
    bf::create_symlink(mount_point_entry, destination, error);
    if (error) {
      LOG(ERROR) << "Failed to create symlink for entry: " << mount_point_entry;
      return Status::APP_DIR_ERROR;
    }
  }
  return Status::OK;
}


ci::Step::Status StepTpkPreparePackageDirectory::ExtractEntries() {
  LOG(DEBUG) << "Extracting entries from zip package...";
  for (auto& entry : GetExtractEntries()) {
    LOG(DEBUG) << "Extracting: " << entry;
    auto status = PrepareDirectory(entry);
    if (status != Status::OK)
      return status;
  }
  return Status::OK;
}

ci::Step::Status StepTpkPreparePackageDirectory::PrepareLinks() {
  bf::path mount_point = ci::GetMountLocation(context_->pkg_path.get());
  LOG(DEBUG) << "Creating symlinks to zip package...";
  for (auto& link_entry : kSymlinkEntries) {
    LOG(DEBUG) << "Symlink: " << link_entry;
    auto status = PrepareLink(link_entry, mount_point);
    if (status != Status::OK)
      return status;
  }
  return Status::OK;
}

ci::Step::Status StepTpkPreparePackageDirectory::process() {
  auto status = ExtractEntries();
  if (status != Status::OK)
    return status;
  return PrepareLinks();
}

ci::Step::Status StepTpkPreparePackageDirectory::precheck() {
  if (context_->pkg_path.get().empty()) {
    LOG(ERROR) << "Package installation path is not set";
    return Status::INVALID_VALUE;
  }
  return Status::OK;
}

const std::vector<std::string>&
StepTpkPreparePackageDirectory::GetExtractEntries() const {
  return kExtractEntries;
}

}  // namespace filesystem
}  // namespace tpk
