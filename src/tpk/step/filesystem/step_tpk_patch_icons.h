// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_STEP_FILESYSTEM_STEP_TPK_PATCH_ICONS_H_
#define TPK_STEP_FILESYSTEM_STEP_TPK_PATCH_ICONS_H_

#ifndef TPK_BACKEND_EXPORT_API
#define TPK_BACKEND_EXPORT_API
#endif

#include <boost/filesystem/path.hpp>

#include <common/step/step.h>
#include <manifest_parser/utils/logging.h>

namespace tpk {
namespace filesystem {

/**
 * @brief The StepTpkPatchIcons class
 *        Fixes location of icons for tpk apps if icons are not located in
 *        "shared/res/" directory.
 */
class TPK_BACKEND_EXPORT_API StepTpkPatchIcons : public common_installer::Step {
 public:
  using Step::Step;

  Status process() override;
  Status clean() override { return Status::OK; }
  Status undo() override { return Status::OK; }
  Status precheck() override { return Status::OK; }

 private:
  Status ProcessIconOutsidePackage(
      const boost::filesystem::path& common_icon_location,
      const boost::filesystem::path& icon_text,
      application_x* app, icon_x* icon);
  Status FixIconLocation(const boost::filesystem::path& icon_text);

  SCOPE_LOG_TAG(TpkPatchIcons)
};

}  // namespace filesystem
}  // namespace tpk

#endif  // TPK_STEP_FILESYSTEM_STEP_TPK_PATCH_ICONS_H_
