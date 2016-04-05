// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_STEP_PKGMGR_STEP_CONVERT_XML_H_
#define TPK_STEP_PKGMGR_STEP_CONVERT_XML_H_

#ifndef TPK_BACKEND_EXPORT_API
#define TPK_BACKEND_EXPORT_API
#endif

#include <manifest_parser/utils/logging.h>

#include <boost/filesystem/path.hpp>
#include <libxml/tree.h>

#include <common/step/step.h>

namespace tpk {
namespace pkgmgr {

class TPK_BACKEND_EXPORT_API StepConvertXml : public common_installer::Step {
 public:
  using Step::Step;

  Status process() override;
  Status clean() override;
  Status undo() override;
  Status precheck() override;

 private:
  boost::filesystem::path xml_path_;
  bool ConvertXml(xmlDocPtr doc);

  STEP_NAME(ConvertXml)
};

}  // namespace pkgmgr
}  // namespace tpk

#endif  // TPK_STEP_PKGMGR_STEP_CONVERT_XML_H_
