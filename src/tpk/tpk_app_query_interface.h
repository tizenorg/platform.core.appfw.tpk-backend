// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_TPK_APP_QUERY_INTERFACE_H_
#define TPK_TPK_APP_QUERY_INTERFACE_H_

#ifndef TPK_BACKEND_EXPORT_API
#define TPK_BACKEND_EXPORT_API
#endif

#include <common/app_query_interface.h>

namespace tpk {

class TPK_BACKEND_EXPORT_API TpkAppQueryInterface : public common_installer::AppQueryInterface {
 public:
  bool IsAppInstalledByArgv(int argc, char** argv) override;
};

}  // namespace tpk

#endif  // TPK_TPK_APP_QUERY_INTERFACE_H_
