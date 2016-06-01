// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_TPK_MOUNT_PATH_H_
#define TPK_TPK_MOUNT_PATH_H_

#include <string>
#include <vector>

namespace tpk {

const std::vector<std::string>& GetExtractEntries();
const std::vector<std::string>& GetSymlinkEntries();

}  // namespace tpk

#endif  // TPK_TPK_MOUNT_PATH_H_
