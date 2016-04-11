// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_STEP_FILESYSTEM_STEP_TPK_PREPARE_PACKAGE_DIRECTORY_H_
#define TPK_STEP_FILESYSTEM_STEP_TPK_PREPARE_PACKAGE_DIRECTORY_H_

#include <manifest_parser/utils/logging.h>

#include <boost/filesystem/path.hpp>
#include <pkgmgrinfo_basic.h>

#include <string>

#include "common/installer_context.h"
#include "common/step/step.h"

namespace tpk {
namespace filesystem {

/**
 * \brief Responsible for adjusting package directory after mounting zip package
 *        by StepMountInstall or StepMountUpdate
 *
 * This step will:
 *
 * 1) Extract partially content of zip package binaries must be extracted from:
 *    - bin/
 *    - lib/
 *    - shared/res/
 *
 * 2) Create symlinks for following files and directories to point them to
 *    mount path:
 *    - tizen-manifest.xml
 *    - author-signature.xml
 *    - signature*.xml
 *    - res/
 */
class StepTpkPreparePackageDirectory : public common_installer::Step {
 public:
  using Step::Step;

  Status process() override;
  Status clean() override;
  Status undo() override;
  Status precheck() override;

 private:
  Status PreparePackageDirectory();
  Status PrepareDirectory(const std::string& entry,
                          const boost::filesystem::path& backup_path);
  Status RestoreDirectory(const std::string& entry,
                          const boost::filesystem::path& backup_path);
  Status PrepareLink(const std::string& entry);
};

}  // namespace filesystem
}  // namespace tpk

#endif  // TPK_STEP_FILESYSTEM_STEP_TPK_PREPARE_PACKAGE_DIRECTORY_H_
