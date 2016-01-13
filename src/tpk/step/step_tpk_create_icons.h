// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_STEP_STEP_TPK_CREATE_ICONS_H_
#define TPK_STEP_STEP_TPK_CREATE_ICONS_H_

#include <manifest_parser/utils/logging.h>

#include <common/step/step_create_icons.h>

namespace tpk {
namespace filesystem {

/**
 * \brief This step return path to widget icon
 */
class StepTpkCreateIcons
    : public common_installer::filesystem::StepCreateIcons {
 public:
  using StepCreateIcons::StepCreateIcons;

  /**
   * \brief Return path to widget icon
   *
   * \return path to widget icon
   */
  boost::filesystem::path GetIconRoot() const override;

  /**
   * \brief Return type of application for which icons should be created
   *
   * \return application type
   */
  std::string GetAppTypeForIcons() const override;

  SCOPE_LOG_TAG(TpkCreateIcons)
};

}  // namespace filesystem
}  // namespace tpk

#endif  // TPK_STEP_STEP_TPK_CREATE_ICONS_H_
