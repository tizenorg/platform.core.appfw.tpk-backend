// Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
// Use of this source code is governed by an apache-2.0 license that can be
// found in the LICENSE file.

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/system/error_code.hpp>

#include <common/backup_paths.h>
#include <common/pkgmgr_interface.h>
#include <common/pkgmgr_registration.h>
#include <common/request.h>
#include <common/step/configuration/step_fail.h>
#include <common/utils/subprocess.h>

#include <gtest/gtest.h>
#include <gtest/gtest-death-test.h>
#include <pkgmgr-info.h>
#include <signal.h>
#include <unistd.h>
#include <tzplatform_config.h>

#include <array>
#include <cstdio>
#include <cstdlib>

#include "tpk/tpk_app_query_interface.h"
#include "tpk/tpk_installer.h"

#define SIZEOFARRAY(ARR)                                                       \
  sizeof(ARR) / sizeof(ARR[0])                                                 \

namespace bf = boost::filesystem;
namespace bs = boost::system;
namespace ci = common_installer;

namespace {

const bf::path kSmokePackagesDirectory =
    "/usr/share/tpk-backend-ut/test_samples/smoke/";

const char kApplicationDir[] = ".applications";
const char kApplicationDirBackup[] = ".applications.bck";
const char KUserAppsDir[] = "apps_rw";
const char KUserAppsDirBackup[] = "apps_rw.bck";

enum class RequestResult {
  NORMAL,
  FAIL
};

class TestPkgmgrInstaller : public ci::PkgmgrInstallerInterface {
 public:
  bool CreatePkgMgrInstaller(pkgmgr_installer** installer,
                             ci::InstallationMode* mode) {
    *installer = pkgmgr_installer_offline_new();
    if (!*installer)
      return false;
    *mode = ci::InstallationMode::ONLINE;
    return true;
  }

  bool ShouldCreateSignal() const {
    return false;
  }
};

void RemoveAllRecoveryFiles() {
  bf::path root_path = ci::GetRootAppPath(false);
  if (!bf::exists(root_path))
    return;
  for (auto& dir_entry : boost::make_iterator_range(
         bf::directory_iterator(root_path), bf::directory_iterator())) {
    if (bf::is_regular_file(dir_entry)) {
      if (dir_entry.path().string().find("/recovery") != std::string::npos) {
        bs::error_code error;
        bf::remove(dir_entry.path(), error);
      }
    }
  }
}

bf::path FindRecoveryFile() {
  bf::path root_path = ci::GetRootAppPath(false);
  for (auto& dir_entry : boost::make_iterator_range(
         bf::directory_iterator(root_path), bf::directory_iterator())) {
    if (bf::is_regular_file(dir_entry)) {
      if (dir_entry.path().string().find("/recovery") != std::string::npos) {
        return dir_entry.path();
      }
    }
  }
  return {};
}

bool ValidateFileContentInPackage(const std::string& pkgid,
                                  const std::string& relative,
                                  const std::string& expected) {
  bf::path root_path = ci::GetRootAppPath(false);
  bf::path file_path = root_path / pkgid / relative;
  if (!bf::exists(file_path)) {
    LOG(ERROR) << file_path << " doesn't exist";
    return false;
  }
  FILE* handle = fopen(file_path.c_str(), "r");
  if (!handle) {
    LOG(ERROR) << file_path << " cannot  be open";
    return false;
  }
  std::string content;
  std::array<char, 200> buffer;
  while (fgets(buffer.data(), buffer.size(), handle)) {
    content += buffer.data();
  }
  fclose(handle);
  return content == expected;
}

void ValidatePackageFS(const std::string& pkgid, const std::string& appid) {
  bf::path root_path = ci::GetRootAppPath(false);
  bf::path package_path = root_path / pkgid;
  bf::path binary_path = package_path / "bin" / appid;
  bf::path data_path = package_path / "data";
  bf::path shared_path = package_path / "shared";
  bf::path cache_path = package_path / "cache";
  ASSERT_TRUE(bf::exists(root_path));
  ASSERT_TRUE(bf::exists(package_path));
  ASSERT_TRUE(bf::exists(binary_path));
  ASSERT_TRUE(bf::exists(data_path));
  ASSERT_TRUE(bf::exists(shared_path));
  ASSERT_TRUE(bf::exists(cache_path));

  bf::path manifest_path =
      bf::path(getUserManifestPath(getuid(), false)) / (pkgid + ".xml");
  bf::path icon_path = bf::path(getIconPath(getuid(), false)) / (appid + ".png");
  ASSERT_TRUE(bf::exists(manifest_path));
  ASSERT_TRUE(bf::exists(icon_path));

  // backups should not exist
  bf::path package_backup = ci::GetBackupPathForPackagePath(package_path);
  bf::path manifest_backup = ci::GetBackupPathForManifestFile(manifest_path);
  bf::path icon_backup = ci::GetBackupPathForIconFile(icon_path);
  ASSERT_FALSE(bf::exists(package_backup));
  ASSERT_FALSE(bf::exists(manifest_backup));
  ASSERT_FALSE(bf::exists(icon_backup));
}

void PackageCheckCleanup(const std::string& pkgid, const std::string& appid) {
  bf::path root_path = ci::GetRootAppPath(false);
  bf::path package_path = root_path / pkgid;
  ASSERT_FALSE(bf::exists(package_path));

  bf::path manifest_path =
      bf::path(getUserManifestPath(getuid(), false)) / (pkgid + ".xml");
  bf::path icon_path = bf::path(getIconPath(getuid(), false)) / (appid + ".png");
  ASSERT_FALSE(bf::exists(manifest_path));
  ASSERT_FALSE(bf::exists(icon_path));

  // backups should not exist
  bf::path package_backup = ci::GetBackupPathForPackagePath(package_path);
  bf::path manifest_backup = ci::GetBackupPathForManifestFile(manifest_path);
  bf::path icon_backup = ci::GetBackupPathForIconFile(icon_path);
  ASSERT_FALSE(bf::exists(package_backup));
  ASSERT_FALSE(bf::exists(manifest_backup));
  ASSERT_FALSE(bf::exists(icon_backup));
}

void ValidatePackage(const std::string& pkgid, const std::string& appid) {
  ASSERT_TRUE(ci::IsPackageInstalled(pkgid, ci::GetRequestMode()));
  ValidatePackageFS(pkgid, appid);
}

void CheckPackageNonExistance(const std::string& pkgid,
                              const std::string& appid) {
  ASSERT_FALSE(ci::IsPackageInstalled(pkgid, ci::GetRequestMode()));
  PackageCheckCleanup(pkgid, appid);
}

std::unique_ptr<ci::AppQueryInterface> CreateQueryInterface() {
  std::unique_ptr<ci::AppQueryInterface> query_interface(
      new tpk::TpkAppQueryInterface());
  return query_interface;
}

std::unique_ptr<ci::AppInstaller> CreateInstaller(ci::PkgMgrPtr pkgmgr) {
  std::unique_ptr<ci::AppInstaller> installer(new tpk::TpkInstaller(pkgmgr));
  return installer;
}

ci::AppInstaller::Result RunInstallerWithPkgrmgr(ci::PkgMgrPtr pkgmgr,
                                                 RequestResult mode) {
  std::unique_ptr<ci::AppInstaller> installer = CreateInstaller(pkgmgr);
  switch (mode) {
  case RequestResult::FAIL:
    installer->AddStep<ci::configuration::StepFail>();
    break;
  default:
    break;
  }
  return installer->Run();
}

ci::AppInstaller::Result Install(const bf::path& path,
                                 RequestResult mode = RequestResult::NORMAL) {
  const char* argv[] = {"", "-i", path.c_str()};
  TestPkgmgrInstaller pkgmgr_installer;
  std::unique_ptr<ci::AppQueryInterface> query_interface =
      CreateQueryInterface();
  auto pkgmgr =
      ci::PkgMgrInterface::Create(SIZEOFARRAY(argv), const_cast<char**>(argv),
                                  &pkgmgr_installer,
                                  query_interface.get());
  if (!pkgmgr) {
    LOG(ERROR) << "Failed to initialize pkgmgr interface";
    return ci::AppInstaller::Result::UNKNOWN;
  }
  return RunInstallerWithPkgrmgr(pkgmgr, mode);
}

ci::AppInstaller::Result Update(const bf::path& path_old,
                                const bf::path& path_new,
                                RequestResult mode = RequestResult::NORMAL) {
  if (Install(path_old) != ci::AppInstaller::Result::OK) {
    LOG(ERROR) << "Failed to install application. Cannot update";
    return ci::AppInstaller::Result::UNKNOWN;
  }
  return Install(path_new, mode);
}

ci::AppInstaller::Result Uninstall(const std::string& pkgid,
                                   RequestResult mode = RequestResult::NORMAL) {
  const char* argv[] = {"", "-d", pkgid.c_str()};
  TestPkgmgrInstaller pkgmgr_installer;
  std::unique_ptr<ci::AppQueryInterface> query_interface =
      CreateQueryInterface();
  auto pkgmgr =
      ci::PkgMgrInterface::Create(SIZEOFARRAY(argv), const_cast<char**>(argv),
                                  &pkgmgr_installer, query_interface.get());
  if (!pkgmgr) {
    LOG(ERROR) << "Failed to initialize pkgmgr interface";
    return ci::AppInstaller::Result::UNKNOWN;
  }
  return RunInstallerWithPkgrmgr(pkgmgr, mode);
}

ci::AppInstaller::Result Reinstall(const bf::path& path,
                                   const bf::path& delta_dir,
                                   RequestResult mode = RequestResult::NORMAL) {
  if (Install(path) != ci::AppInstaller::Result::OK) {
    LOG(ERROR) << "Failed to install application. Cannot perform RDS";
    return ci::AppInstaller::Result::UNKNOWN;
  }
  const char* argv[] = {"", "-r", delta_dir.c_str()};
  TestPkgmgrInstaller pkgmgr_installer;
  std::unique_ptr<ci::AppQueryInterface> query_interface =
      CreateQueryInterface();
  auto pkgmgr =
      ci::PkgMgrInterface::Create(SIZEOFARRAY(argv), const_cast<char**>(argv),
                                  &pkgmgr_installer, query_interface.get());
  if (!pkgmgr) {
    LOG(ERROR) << "Failed to initialize pkgmgr interface";
    return ci::AppInstaller::Result::UNKNOWN;
  }
  return RunInstallerWithPkgrmgr(pkgmgr, mode);
}

ci::AppInstaller::Result DeltaInstall(const bf::path& path,
    const bf::path& delta_package) {
  if (Install(path) != ci::AppInstaller::Result::OK) {
    LOG(ERROR) << "Failed to install application. Cannot perform RDS";
    return ci::AppInstaller::Result::UNKNOWN;
  }
  return Install(delta_package);
}

ci::AppInstaller::Result Recover(const bf::path& recovery_file,
                                 RequestResult mode = RequestResult::NORMAL) {
  const char* argv[] = {"", "-b", recovery_file.c_str()};
  TestPkgmgrInstaller pkgmgr_installer;
  std::unique_ptr<ci::AppQueryInterface> query_interface =
      CreateQueryInterface();
  auto pkgmgr =
      ci::PkgMgrInterface::Create(SIZEOFARRAY(argv), const_cast<char**>(argv),
                                  &pkgmgr_installer, query_interface.get());
  if (!pkgmgr) {
    LOG(ERROR) << "Failed to initialize pkgmgr interface";
    return ci::AppInstaller::Result::UNKNOWN;
  }
  return RunInstallerWithPkgrmgr(pkgmgr, mode);
}

}  // namespace

namespace common_installer {

class SmokeEnvironment : public testing::Environment {
 public:
  explicit SmokeEnvironment(const bf::path& home) : home_(home) {
  }
  void SetUp() override {
    bs::error_code error;
    bf::remove_all(home_ / kApplicationDirBackup, error);
    bf::remove_all(home_ / KUserAppsDirBackup, error);
    if (bf::exists(home_ / KUserAppsDir)) {
      bf::rename(home_ / KUserAppsDir, home_ / KUserAppsDirBackup, error);
      if (error)
        LOG(ERROR) << "Failed to setup test environment. Does some previous"
                   << " test crashed? Directory: "
                   << (home_ / KUserAppsDirBackup) << " should not exist.";
      assert(!error);
    }
    if (bf::exists(home_ / kApplicationDir)) {
      bf::rename(home_ / kApplicationDir, home_ / kApplicationDirBackup, error);
      if (error)
        LOG(ERROR) << "Failed to setup test environment. Does some previous"
                   << " test crashed? Directory: "
                   << (home_ / kApplicationDirBackup) << " should not exist.";
      assert(!error);
    }
  }
  void TearDown() override {
    bs::error_code error;
    bf::remove_all(home_ / kApplicationDir, error);
    bf::remove_all(home_ / KUserAppsDir, error);
    if (bf::exists(home_ / KUserAppsDirBackup))
      bf::rename(home_ / KUserAppsDirBackup, home_ / KUserAppsDir, error);
    if (bf::exists(home_ / kApplicationDirBackup))
      bf::rename(home_ / kApplicationDirBackup, home_ / kApplicationDir, error);
  }

 private:
  bf::path home_;
};

class SmokeTest : public testing::Test {
};

TEST_F(SmokeTest, DeltaMode_Tpk) {
  bf::path path = kSmokePackagesDirectory / "DeltaMode_Tpk.tpk";
  bf::path delta_package = kSmokePackagesDirectory / "DeltaMode_Tpk.delta";
  std::string pkgid = "smokeapp18";
  std::string appid = "smokeapp18.DeltaModeTpk";
  ASSERT_EQ(DeltaInstall(path, delta_package),
            ci::AppInstaller::Result::OK);
  ValidatePackage(pkgid, appid);

  // Check delta modifications
  bf::path root_path = ci::GetRootAppPath(false);
  ASSERT_FALSE(bf::exists(root_path / pkgid / "DELETED"));
  ASSERT_TRUE(bf::exists(root_path / pkgid / "ADDED"));
  ASSERT_TRUE(bf::exists(root_path / pkgid / "bin" / "native"));
  ASSERT_TRUE(bf::exists(root_path / pkgid / "shared" / "res" / "native.png"));
  ValidateFileContentInPackage(pkgid, "MODIFIED", "version 2\n");
}

TEST_F(SmokeTest, ReinstallMode_Tpk) {
  bf::path path = kSmokePackagesDirectory / "ReinstallMode_Tpk.tpk";
  bf::path rds_directory = kSmokePackagesDirectory / "delta_dir";
  std::string pkgid = "smokeapp25";
  std::string appid = "smokeapp25.ReinstallModeTpk";
  ASSERT_EQ(Reinstall(path, rds_directory), ci::AppInstaller::Result::OK);
  ValidatePackage(pkgid, appid);

  // Check delta modifications
  bf::path root_path = ci::GetRootAppPath(false);
  ASSERT_FALSE(bf::exists(root_path / pkgid / "DELETED"));
  ASSERT_TRUE(bf::exists(root_path / pkgid / "ADDED"));
  ASSERT_TRUE(bf::exists(root_path / pkgid / "bin" / "native"));
  ASSERT_TRUE(bf::exists(root_path / pkgid / "shared" / "res" / "native.png"));
  ValidateFileContentInPackage(pkgid, "MODIFIED", "version 2\n");
}

TEST_F(SmokeTest, InstallationMode_Tpk) {
  bf::path path = kSmokePackagesDirectory / "InstallationMode_Tpk.tpk";
  std::string pkgid = "smokeapp12";
  std::string appid = "smokeapp12.InstallationModeTpk";
  ASSERT_EQ(Install(path), ci::AppInstaller::Result::OK);
  ValidatePackage(pkgid, appid);
}

TEST_F(SmokeTest, UpdateMode_Tpk) {
  bf::path path_old = kSmokePackagesDirectory / "UpdateMode_Tpk.tpk";
  bf::path path_new = kSmokePackagesDirectory / "UpdateMode_Tpk_2.tpk";
  std::string pkgid = "smokeapp13";
  std::string appid = "smokeapp13.UpdateModeTpk";
  ASSERT_EQ(Update(path_old, path_new), ci::AppInstaller::Result::OK);
  ValidatePackage(pkgid, appid);

  ASSERT_TRUE(ValidateFileContentInPackage(pkgid, "VERSION", "2\n"));
}

TEST_F(SmokeTest, DeinstallationMode_Tpk) {
  bf::path path = kSmokePackagesDirectory / "DeinstallationMode_Tpk.tpk";
  std::string pkgid = "smokeapp14";
  std::string appid = "smokeapp14.DeinstallationModeTpk";
  ASSERT_EQ(Install(path), ci::AppInstaller::Result::OK);
  ASSERT_EQ(Uninstall(pkgid), ci::AppInstaller::Result::OK);
  CheckPackageNonExistance(pkgid, appid);
}

TEST_F(SmokeTest, RecoveryMode_Tpk_Installation) {
  bf::path path = kSmokePackagesDirectory / "RecoveryMode_Tpk_Installation.tpk";
  RemoveAllRecoveryFiles();
  ci::Subprocess backend_crash("/usr/bin/tpk-backend-ut/smoke-test-helper");
  backend_crash.Run("-i", path.string());
  ASSERT_NE(backend_crash.Wait(), 0);

  std::string pkgid = "smokeapp15";
  std::string appid = "smokeapp15.RecoveryModeTpkInstallation";
  bf::path recovery_file = FindRecoveryFile();
  ASSERT_FALSE(recovery_file.empty());
  ASSERT_EQ(Recover(recovery_file), ci::AppInstaller::Result::OK);
  CheckPackageNonExistance(pkgid, appid);
}

TEST_F(SmokeTest, RecoveryMode_Tpk_Update) {
  bf::path path_old = kSmokePackagesDirectory / "RecoveryMode_Tpk_Update.tpk";
  bf::path path_new = kSmokePackagesDirectory / "RecoveryMode_Tpk_Update_2.tpk";
  RemoveAllRecoveryFiles();
  ci::Subprocess backend("/usr/bin/tpk-backend");
  backend.Run("-i", path_old.string());
  ASSERT_EQ(backend.Wait(), 0);
  ci::Subprocess backend_crash("/usr/bin/tpk-backend-ut/smoke-test-helper");
  backend_crash.Run("-i", path_new.string());
  ASSERT_NE(backend_crash.Wait(), 0);

  std::string pkgid = "smokeapp16";
  std::string appid = "smokeapp16.RecoveryModeTpkUpdate";
  bf::path recovery_file = FindRecoveryFile();
  ASSERT_FALSE(recovery_file.empty());
  ASSERT_EQ(Recover(recovery_file), ci::AppInstaller::Result::OK);
  ValidatePackage(pkgid, appid);

  ASSERT_TRUE(ValidateFileContentInPackage(pkgid, "VERSION", "1\n"));
}

}  // namespace common_installer

int main(int argc,  char** argv) {
  testing::InitGoogleTest(&argc, argv);
  const char* directory = getenv("HOME");
  if (!directory) {
    LOG(ERROR) << "Cannot get $HOME value";
    return 1;
  }
  testing::AddGlobalTestEnvironment(
      new common_installer::SmokeEnvironment(directory));
  return RUN_ALL_TESTS();
}
