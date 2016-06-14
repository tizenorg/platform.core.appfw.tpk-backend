// Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#ifndef TPK_EXTERNAL_DIRS_H_
#define TPK_EXTERNAL_DIRS_H_

#include <common/request.h>
#include <pkgmgrinfo_basic.h>

#include <string>

namespace tpk {

/**
 * @brief HasExternalAppdataPrivilege
 *        Defines conditions upon which package has privilege to create external
 *        appdata directories
 *
 * @return true if package has privilege to directories
 */
bool HasExternalAppdataPrivilege(manifest_x* manifest);

/**
 * @brief CreateExternalAppdataDirectories
 *        Creates external appdata directories according to installation
 *        context's uid and request mode
 *
 * @param pkgid package id
 * @param request_mode request mode - user or global
 * @param uid user id
 *
 * @return true if operation succeeded
 */
bool CreateExternalAppdataDirectories(const std::string& pkgid,
    common_installer::RequestMode request_mode, uid_t uid);

/**
 * @brief RemoveExternalAppdataDirectories
 *        Removes external appdata directories according to installation
 *        context's uid and request mode
 *
 * @param pkgid package id
 * @param request_mode request mode - user or global
 * @param uid user id
 *
 * @return true if operation succeeded
 */
bool RemoveExternalAppdataDirectories(const std::string& pkgid,
    common_installer::RequestMode request_mode, uid_t uid);

}  // namespace tpk

#endif  // TPK_EXTERNAL_DIRS_H_
