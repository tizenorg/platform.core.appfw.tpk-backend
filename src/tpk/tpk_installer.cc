/* Copyright 2015 Samsung Electronics, license APACHE-2.0, see LICENSE file */
#include "tpk/tpk_installer.h"

#include <common/app_installer.h>
#include <common/step/backup/step_backup_icons.h>
#include <common/step/backup/step_backup_manifest.h>
#include <common/step/backup/step_copy_backup.h>
#include <common/step/configuration/step_configure.h>
#include <common/step/configuration/step_fail.h>
#include <common/step/configuration/step_parse_manifest.h>
#include <common/step/filesystem/step_clear_data.h>
#include <common/step/filesystem/step_copy.h>
#include <common/step/filesystem/step_copy_storage_directories.h>
#include <common/step/filesystem/step_copy_tep.h>
#include <common/step/filesystem/step_create_icons.h>
#include <common/step/filesystem/step_create_per_user_storage_directories.h>
#include <common/step/filesystem/step_create_storage_directories.h>
#include <common/step/filesystem/step_delta_patch.h>
#include <common/step/filesystem/step_recover_files.h>
#include <common/step/filesystem/step_recover_icons.h>
#include <common/step/filesystem/step_recover_manifest.h>
#include <common/step/filesystem/step_recover_storage_directories.h>
#include <common/step/filesystem/step_remove_files.h>
#include <common/step/filesystem/step_remove_icons.h>
#include <common/step/filesystem/step_remove_per_user_storage_directories.h>
#include <common/step/filesystem/step_remove_temporary_directory.h>
#include <common/step/filesystem/step_unzip.h>
#include <common/step/mount/step_mount_unpacked.h>
#include <common/step/mount/step_mount_install.h>
#include <common/step/mount/step_mount_update.h>
#include <common/step/pkgmgr/step_check_blacklist.h>
#include <common/step/pkgmgr/step_check_removable.h>
#include <common/step/pkgmgr/step_kill_apps.h>
#include <common/step/pkgmgr/step_recover_application.h>
#include <common/step/pkgmgr/step_register_app.h>
#include <common/step/pkgmgr/step_remove_manifest.h>
#include <common/step/pkgmgr/step_run_parser_plugins.h>
#include <common/step/pkgmgr/step_unregister_app.h>
#include <common/step/pkgmgr/step_update_app.h>
#include <common/step/pkgmgr/step_update_tep.h>
#include <common/step/rds/step_rds_parse.h>
#include <common/step/recovery/step_open_recovery_file.h>
#include <common/step/security/step_check_old_certificate.h>
#include <common/step/security/step_check_signature.h>
#include <common/step/security/step_privilege_compatibility.h>
#include <common/step/security/step_recover_security.h>
#include <common/step/security/step_register_security.h>
#include <common/step/security/step_revoke_security.h>
#include <common/step/security/step_rollback_deinstallation_security.h>
#include <common/step/security/step_rollback_installation_security.h>
#include <common/step/security/step_update_security.h>

#include "tpk/step/configuration/step_parse_preload.h"
#include "tpk/step/filesystem/step_create_symbolic_link.h"
#include "tpk/step/filesystem/step_tpk_patch_icons.h"
#include "tpk/step/filesystem/step_tpk_prepare_package_directory.h"
#include "tpk/step/filesystem/step_check_pkg_directory_path.h"
#include "tpk/step/pkgmgr/step_convert_xml.h"
#include "tpk/step/pkgmgr/step_manifest_adjustment.h"
#include "tpk/step/rds/step_tpk_rds_modify.h"
#include "tpk/step/security/step_check_tpk_background_category.h"

namespace ci = common_installer;

namespace {

const char kPkgType[] = "tpk";

}  // namespace

namespace tpk {

TpkInstaller::TpkInstaller(common_installer::PkgMgrPtr pkgmgr)
    : AppInstaller(kPkgType, pkgmgr) {
  Prepare();
}

TpkInstaller::~TpkInstaller() {
}

void TpkInstaller::Prepare() {
  switch (pkgmgr_->GetRequestType()) {
    case ci::RequestType::Install:
      InstallSteps();
      break;
    case ci::RequestType::Update:
      UpdateSteps();
      break;
    case ci::RequestType::Uninstall:
      UninstallSteps();
      break;
    case ci::RequestType::Reinstall:
      ReinstallSteps();
      break;
    case ci::RequestType::Delta:
      DeltaSteps();
      break;
    case ci::RequestType::Recovery:
      RecoverySteps();
      break;
    case ci::RequestType::MountInstall:
      MountInstallSteps();
      break;
    case ci::RequestType::MountUpdate:
      MountUpdateSteps();
      break;
    case ci::RequestType::ManifestDirectInstall:
      ManifestDirectInstallSteps();
      break;
    case ci::RequestType::ManifestDirectUpdate:
      ManifestDirectUpdateSteps();
      break;
    case ci::RequestType::Clear:
      ClearSteps();
      break;
    default:
      AddStep<ci::configuration::StepFail>();
      break;
  }
}

void TpkInstaller::InstallSteps() {
  AddStep<ci::configuration::StepConfigure>(pkgmgr_);
  AddStep<ci::filesystem::StepUnzip>();
  AddStep<ci::configuration::StepParseManifest>(
      ci::configuration::StepParseManifest::ManifestLocation::PACKAGE,
      ci::configuration::StepParseManifest::StoreLocation::NORMAL);
  AddStep<tpk::configuration::StepParsePreload>();
  AddStep<ci::pkgmgr::StepCheckBlacklist>();
  AddStep<ci::security::StepCheckSignature>();
  AddStep<ci::security::StepPrivilegeCompatibility>();
  AddStep<tpk::security::StepCheckTpkBackgroundCategory>();
  AddStep<ci::security::StepRollbackInstallationSecurity>();
  AddStep<ci::filesystem::StepCopy>();
  AddStep<ci::filesystem::StepCopyTep>();
  AddStep<ci::filesystem::StepCreateStorageDirectories>();
  AddStep<tpk::filesystem::StepCreateSymbolicLink>();
  AddStep<tpk::filesystem::StepTpkPatchIcons>();
  AddStep<ci::filesystem::StepCreateIcons>();
  AddStep<ci::security::StepRegisterSecurity>();
  AddStep<tpk::pkgmgr::StepConvertXml>();
  AddStep<tpk::pkgmgr::StepManifestAdjustment>();
  AddStep<ci::pkgmgr::StepRegisterApplication>();
  AddStep<ci::pkgmgr::StepRunParserPlugin>(
      ci::Plugin::ActionType::Install);
  AddStep<ci::filesystem::StepCreatePerUserStorageDirectories>();
}

void TpkInstaller::UpdateSteps() {
  AddStep<ci::configuration::StepConfigure>(pkgmgr_);
  AddStep<ci::filesystem::StepUnzip>();
  AddStep<ci::configuration::StepParseManifest>(
      ci::configuration::StepParseManifest::ManifestLocation::PACKAGE,
      ci::configuration::StepParseManifest::StoreLocation::NORMAL);
  AddStep<tpk::configuration::StepParsePreload>();
  AddStep<ci::pkgmgr::StepCheckBlacklist>();
  AddStep<ci::security::StepCheckSignature>();
  AddStep<ci::security::StepPrivilegeCompatibility>();
  AddStep<tpk::security::StepCheckTpkBackgroundCategory>();
  AddStep<ci::security::StepCheckOldCertificate>();
  AddStep<ci::configuration::StepParseManifest>(
      ci::configuration::StepParseManifest::ManifestLocation::INSTALLED,
      ci::configuration::StepParseManifest::StoreLocation::BACKUP);
  AddStep<ci::pkgmgr::StepKillApps>();
  AddStep<ci::backup::StepBackupManifest>();
  AddStep<ci::backup::StepBackupIcons>();
  AddStep<ci::backup::StepCopyBackup>();
  AddStep<ci::filesystem::StepCopyTep>();
  AddStep<ci::pkgmgr::StepUpdateTep>();
  AddStep<ci::filesystem::StepCopyStorageDirectories>();
  AddStep<tpk::filesystem::StepCreateSymbolicLink>();
  AddStep<tpk::filesystem::StepTpkPatchIcons>();
  AddStep<ci::filesystem::StepCreateIcons>();
  AddStep<ci::security::StepUpdateSecurity>();
  AddStep<tpk::pkgmgr::StepConvertXml>();
  AddStep<tpk::pkgmgr::StepManifestAdjustment>();
  AddStep<ci::pkgmgr::StepUpdateApplication>();
  AddStep<ci::pkgmgr::StepRunParserPlugin>(ci::Plugin::ActionType::Upgrade);
}

void TpkInstaller::UninstallSteps() {
  AddStep<ci::configuration::StepConfigure>(pkgmgr_);
  AddStep<ci::pkgmgr::StepCheckRemovable>();
  AddStep<ci::configuration::StepParseManifest>(
      ci::configuration::StepParseManifest::ManifestLocation::INSTALLED,
      ci::configuration::StepParseManifest::StoreLocation::NORMAL);
  AddStep<ci::pkgmgr::StepRunParserPlugin>(
      ci::Plugin::ActionType::Uninstall);
  AddStep<ci::pkgmgr::StepKillApps>();
  AddStep<ci::filesystem::StepRemovePerUserStorageDirectories>();
  AddStep<ci::pkgmgr::StepUnregisterApplication>();
  AddStep<ci::security::StepRollbackDeinstallationSecurity>();
  AddStep<ci::filesystem::StepRemoveFiles>();
  AddStep<ci::filesystem::StepRemoveIcons>();
  AddStep<ci::security::StepRevokeSecurity>();
  AddStep<ci::pkgmgr::StepRemoveManifest>();
}

void TpkInstaller::ReinstallSteps() {
  AddStep<ci::configuration::StepConfigure>(pkgmgr_);
  AddStep<ci::configuration::StepParseManifest>(
     ci::configuration::StepParseManifest::ManifestLocation::PACKAGE,
     ci::configuration::StepParseManifest::StoreLocation::NORMAL);
  AddStep<tpk::configuration::StepParsePreload>();
  AddStep<ci::pkgmgr::StepCheckBlacklist>();
  // TODO(t.iwanek): add StepCheckSignature which is missing
  // this step is temporary removed because of validation problems as files
  // not exising in signature but existing is fs will cause error (data files)
  AddStep<ci::security::StepPrivilegeCompatibility>();
  AddStep<tpk::security::StepCheckTpkBackgroundCategory>();
  // TODO(t.iwanek): add StepCheckOldCertificate which is missing
  AddStep<ci::configuration::StepParseManifest>(
     ci::configuration::StepParseManifest::ManifestLocation::INSTALLED,
     ci::configuration::StepParseManifest::StoreLocation::BACKUP);
  AddStep<ci::pkgmgr::StepKillApps>();
  AddStep<ci::backup::StepBackupManifest>();
  AddStep<ci::backup::StepBackupIcons>();
  AddStep<ci::rds::StepRDSParse>();
  AddStep<tpk::rds::StepTpkRDSModify>();
  AddStep<tpk::filesystem::StepCreateSymbolicLink>();
  AddStep<tpk::filesystem::StepTpkPatchIcons>();
  AddStep<ci::filesystem::StepCreateIcons>();
  AddStep<ci::security::StepUpdateSecurity>();
  AddStep<tpk::pkgmgr::StepConvertXml>();
  AddStep<tpk::pkgmgr::StepManifestAdjustment>();
  AddStep<ci::pkgmgr::StepUpdateApplication>();
  AddStep<ci::pkgmgr::StepRunParserPlugin>(ci::Plugin::ActionType::Upgrade);
}

void TpkInstaller::DeltaSteps() {
  AddStep<ci::configuration::StepConfigure>(pkgmgr_);
  AddStep<ci::filesystem::StepUnzip>();
  AddStep<ci::configuration::StepParseManifest>(
      ci::configuration::StepParseManifest::ManifestLocation::PACKAGE,
      ci::configuration::StepParseManifest::StoreLocation::NORMAL);
  AddStep<ci::filesystem::StepDeltaPatch>();
  AddStep<ci::pkgmgr::StepCheckBlacklist>();
  AddStep<ci::security::StepCheckSignature>();
  AddStep<ci::security::StepPrivilegeCompatibility>();
  AddStep<tpk::security::StepCheckTpkBackgroundCategory>();
  AddStep<ci::security::StepCheckOldCertificate>();
  AddStep<ci::configuration::StepParseManifest>(
      ci::configuration::StepParseManifest::ManifestLocation::INSTALLED,
      ci::configuration::StepParseManifest::StoreLocation::BACKUP);
  AddStep<ci::pkgmgr::StepKillApps>();
  AddStep<ci::backup::StepBackupManifest>();
  AddStep<ci::backup::StepBackupIcons>();
  AddStep<ci::backup::StepCopyBackup>();
  AddStep<ci::filesystem::StepCopyStorageDirectories>();
  AddStep<tpk::filesystem::StepCreateSymbolicLink>();
  AddStep<tpk::filesystem::StepTpkPatchIcons>();
  AddStep<ci::filesystem::StepCreateIcons>();
  AddStep<ci::security::StepUpdateSecurity>();
  AddStep<tpk::pkgmgr::StepConvertXml>();
  AddStep<ci::pkgmgr::StepUpdateApplication>();
  AddStep<ci::pkgmgr::StepRunParserPlugin>(ci::Plugin::ActionType::Upgrade);
}

void TpkInstaller::RecoverySteps() {
  AddStep<ci::configuration::StepConfigure>(pkgmgr_);
  AddStep<ci::recovery::StepOpenRecoveryFile>();
  AddStep<ci::configuration::StepParseManifest>(
      ci::configuration::StepParseManifest::ManifestLocation::RECOVERY,
      ci::configuration::StepParseManifest::StoreLocation::NORMAL);
  AddStep<ci::pkgmgr::StepRecoverApplication>();
  AddStep<ci::filesystem::StepRemoveTemporaryDirectory>();
  AddStep<ci::filesystem::StepRecoverIcons>();
  AddStep<ci::filesystem::StepRecoverManifest>();
  AddStep<ci::filesystem::StepRecoverStorageDirectories>();
  AddStep<ci::filesystem::StepRecoverFiles>();
  AddStep<ci::security::StepRecoverSecurity>();
}

void TpkInstaller::MountInstallSteps() {
  AddStep<ci::configuration::StepConfigure>(pkgmgr_);
  AddStep<ci::mount::StepMountUnpacked>();
  AddStep<ci::configuration::StepParseManifest>(
      ci::configuration::StepParseManifest::ManifestLocation::PACKAGE,
      ci::configuration::StepParseManifest::StoreLocation::NORMAL);
  AddStep<tpk::configuration::StepParsePreload>();
  AddStep<ci::pkgmgr::StepCheckBlacklist>();
  AddStep<ci::security::StepCheckSignature>();
  AddStep<ci::security::StepPrivilegeCompatibility>();
  AddStep<tpk::security::StepCheckTpkBackgroundCategory>();
  AddStep<ci::security::StepRollbackInstallationSecurity>();
  AddStep<ci::mount::StepMountInstall>();
  AddStep<tpk::filesystem::StepTpkPreparePackageDirectory>();
  AddStep<ci::filesystem::StepCopyTep>();
  AddStep<ci::filesystem::StepCreateStorageDirectories>();
  AddStep<tpk::filesystem::StepCreateSymbolicLink>();
  AddStep<tpk::filesystem::StepTpkPatchIcons>();
  AddStep<ci::filesystem::StepCreateIcons>();
  AddStep<ci::security::StepRegisterSecurity>();
  AddStep<tpk::pkgmgr::StepConvertXml>();
  AddStep<tpk::pkgmgr::StepManifestAdjustment>();
  AddStep<ci::pkgmgr::StepRegisterApplication>();
  AddStep<ci::pkgmgr::StepRunParserPlugin>(
      ci::Plugin::ActionType::Install);
  AddStep<ci::filesystem::StepCreatePerUserStorageDirectories>();
}

void TpkInstaller::MountUpdateSteps() {
  AddStep<ci::configuration::StepConfigure>(pkgmgr_);
  AddStep<ci::mount::StepMountUnpacked>();
  AddStep<ci::configuration::StepParseManifest>(
      ci::configuration::StepParseManifest::ManifestLocation::PACKAGE,
      ci::configuration::StepParseManifest::StoreLocation::NORMAL);
  AddStep<tpk::configuration::StepParsePreload>();
  AddStep<ci::pkgmgr::StepCheckBlacklist>();
  AddStep<ci::security::StepCheckSignature>();
  AddStep<ci::security::StepPrivilegeCompatibility>();
  AddStep<tpk::security::StepCheckTpkBackgroundCategory>();
  AddStep<ci::security::StepCheckOldCertificate>();
  AddStep<ci::configuration::StepParseManifest>(
      ci::configuration::StepParseManifest::ManifestLocation::INSTALLED,
      ci::configuration::StepParseManifest::StoreLocation::BACKUP);
  AddStep<ci::pkgmgr::StepKillApps>();
  AddStep<ci::backup::StepBackupManifest>();
  AddStep<ci::backup::StepBackupIcons>();
  AddStep<ci::mount::StepMountUpdate>();
  AddStep<tpk::filesystem::StepTpkPreparePackageDirectory>();
  AddStep<ci::filesystem::StepCopyTep>();
  AddStep<ci::pkgmgr::StepUpdateTep>();
  AddStep<ci::filesystem::StepCopyStorageDirectories>();
  AddStep<tpk::filesystem::StepCreateSymbolicLink>();
  AddStep<tpk::filesystem::StepTpkPatchIcons>();
  AddStep<ci::filesystem::StepCreateIcons>();
  AddStep<ci::security::StepUpdateSecurity>();
  AddStep<tpk::pkgmgr::StepConvertXml>();
  AddStep<tpk::pkgmgr::StepManifestAdjustment>();
  AddStep<ci::pkgmgr::StepUpdateApplication>();
  AddStep<ci::pkgmgr::StepRunParserPlugin>(ci::Plugin::ActionType::Upgrade);
}

void TpkInstaller::ManifestDirectInstallSteps() {
  AddStep<ci::configuration::StepConfigure>(pkgmgr_);
  AddStep<tpk::filesystem::StepCheckPkgDirPath>();
  AddStep<ci::configuration::StepParseManifest>(
      ci::configuration::StepParseManifest::ManifestLocation::INSTALLED,
      ci::configuration::StepParseManifest::StoreLocation::NORMAL);
  AddStep<tpk::configuration::StepParsePreload>();
  AddStep<tpk::pkgmgr::StepManifestAdjustment>();
  AddStep<ci::security::StepCheckSignature>();
  AddStep<ci::security::StepPrivilegeCompatibility>();
  AddStep<tpk::security::StepCheckTpkBackgroundCategory>();
  AddStep<tpk::filesystem::StepCreateSymbolicLink>();
  AddStep<tpk::filesystem::StepTpkPatchIcons>();
  AddStep<ci::security::StepRollbackInstallationSecurity>();
  AddStep<ci::security::StepRegisterSecurity>();
  AddStep<ci::pkgmgr::StepRegisterApplication>();
  AddStep<ci::pkgmgr::StepRunParserPlugin>(ci::Plugin::ActionType::Install);
  AddStep<ci::filesystem::StepCreatePerUserStorageDirectories>();
}

void TpkInstaller::ManifestDirectUpdateSteps() {
  AddStep<ci::configuration::StepConfigure>(pkgmgr_);
  AddStep<ci::configuration::StepParseManifest>(
      ci::configuration::StepParseManifest::ManifestLocation::INSTALLED,
      ci::configuration::StepParseManifest::StoreLocation::NORMAL);
  AddStep<tpk::configuration::StepParsePreload>();
  AddStep<tpk::pkgmgr::StepManifestAdjustment>();
  AddStep<ci::security::StepCheckSignature>();
  AddStep<ci::security::StepPrivilegeCompatibility>();
  AddStep<tpk::security::StepCheckTpkBackgroundCategory>();
  AddStep<tpk::filesystem::StepCreateSymbolicLink>();
  AddStep<tpk::filesystem::StepTpkPatchIcons>();
  AddStep<ci::pkgmgr::StepKillApps>();
  AddStep<ci::security::StepRollbackInstallationSecurity>();
  AddStep<ci::security::StepRegisterSecurity>();
  AddStep<ci::pkgmgr::StepUpdateApplication>();
  AddStep<ci::pkgmgr::StepRunParserPlugin>(ci::Plugin::ActionType::Upgrade);
}

void TpkInstaller::ClearSteps() {
  AddStep<ci::configuration::StepConfigure>(pkgmgr_);
  AddStep<ci::filesystem::StepClearData>();
}

}  // namespace tpk
