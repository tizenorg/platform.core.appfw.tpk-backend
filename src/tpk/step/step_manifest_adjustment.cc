// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by a apache 2.0 license that can be
// found in the LICENSE file.

#include "tpk/step/step_manifest_adjustment.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <string>

namespace bs = boost::system;
namespace bf = boost::filesystem;

using libxml_char = const unsigned char *;

namespace tpk {
namespace pkgmgr {

common_installer::Step::Status StepManifestAdjustment::precheck() {
  bf::path xml_path = context_->xml_path.get();

  if (!bf::exists(xml_path)) {
    LOG(ERROR) << "Cannot find manifest file at " << xml_path;
    return Step::Status::ERROR;
  }

  xml_path_ = xml_path;

  return Step::Status::OK;
}

common_installer::Step::Status StepManifestAdjustment::process() {
  xmlDocPtr doc = xmlParseFile(xml_path_.c_str());

  if (doc == nullptr) {
    LOG(ERROR) << "Could not parse file";
    return Step::Status::ERROR;
  }

  xmlNodePtr node = xmlDocGetRootElement(doc);

  std::string pkgtype_attrib = "type";
  auto attrib = xmlSetProp(node,
      reinterpret_cast<libxml_char>(pkgtype_attrib.c_str()),
      reinterpret_cast<libxml_char>(context_->manifest_data.get()->type));

  if (attrib == nullptr) {
    LOG(ERROR) << "Failed to set attribute pkgtype";
    return Step::Status::ERROR;
  }

  std::string preload_attrib = "preload";
  attrib = xmlSetProp(node,
      reinterpret_cast<libxml_char>(preload_attrib.c_str()),
      reinterpret_cast<libxml_char>(context_->manifest_data.get()->preload));

  if (attrib == nullptr) {
    LOG(ERROR) << "Failed to set attribute preload";
    return Step::Status::ERROR;
  }

  if (xmlSaveFile(xml_path_.c_str(), doc) == -1) {
    LOG(ERROR) << "Failed to modify xml file";
    return Step::Status::ERROR;
  }

  xmlFreeDoc(doc);

  return Step::Status::OK;
}

common_installer::Step::Status StepManifestAdjustment::clean() {
  return Step::Status::OK;
}

common_installer::Step::Status StepManifestAdjustment::undo() {
  return Step::Status::OK;
}

}  // namespace pkgmgr
}  // namespace tpk
