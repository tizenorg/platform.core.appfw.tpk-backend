// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_STEP_FILESYSTEM_STEP_TPK_UPDATE_PACKAGE_DIRECTORY_H_
#define TPK_STEP_FILESYSTEM_STEP_TPK_UPDATE_PACKAGE_DIRECTORY_H_

#include <boost/filesystem/path.hpp>
#include <manifest_parser/utils/logging.h>

#include <string>

#include "tpk/step/filesystem/step_tpk_prepare_package_directory.h"

namespace tpk {
namespace filesystem {

/**
 * \brief Responsible for adjusting package directory after mounting zip package
 *        by StepMountUpdate
 *
 * This step will, additionally to base step actions, ,aintain backup
 * of directories from point 1) for operation rollback scenario.
 */
class StepTpkUpdatePackageDirectory : public StepTpkPreparePackageDirectory {
 public:
  using StepTpkPreparePackageDirectory::StepTpkPreparePackageDirectory;

  Status process() override;
  Status clean() override;
  Status undo() override;

 private:
  Status BackupEntries();
  Status BackupDirectory(const std::string& entry,
                          const boost::filesystem::path& backup_path);
  Status RestoreDirectory(const std::string& entry,
                          const boost::filesystem::path& backup_path);

  STEP_NAME(TpkUpdatePackageDirectory)
};

}  // namespace filesystem
}  // namespace tpk

#endif  // TPK_STEP_FILESYSTEM_STEP_TPK_UPDATE_PACKAGE_DIRECTORY_H_
