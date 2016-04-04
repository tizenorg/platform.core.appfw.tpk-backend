SET(SRCS
  step/configuration/step_parse_preload.cc
  step/filesystem/step_check_pkg_directory_path.cc
  step/filesystem/step_create_symbolic_link.cc
  step/filesystem/step_tpk_patch_icons.cc
  step/pkgmgr/step_convert_xml.cc
  step/pkgmgr/step_manifest_adjustment.cc
  step/rds/step_tpk_rds_modify.cc
  step/security/step_check_tpk_background_category.cc
  tpk_app_query_interface.cc
  tpk_installer.cc
)
ADD_LIBRARY(${TARGET_LIBNAME_TPK} SHARED ${SRCS})
ADD_EXECUTABLE(${TARGET_TPK_BACKEND} "tpk_backend.cc")

TARGET_INCLUDE_DIRECTORIES(${TARGET_LIBNAME_TPK} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../")
TARGET_INCLUDE_DIRECTORIES(${TARGET_TPK_BACKEND} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../")

APPLY_PKG_CONFIG(${TARGET_LIBNAME_TPK} PUBLIC
  APP_INSTALLERS_DEPS
  PKGMGR_DEPS
  MANIFEST_PARSER_DEPS
  TPK_MANIFEST_HANDLERS_DEPS
  Boost
)

TARGET_LINK_LIBRARIES(${TARGET_TPK_BACKEND} PRIVATE ${TARGET_LIBNAME_TPK})

INSTALL(TARGETS ${TARGET_LIBNAME_TPK} DESTINATION ${LIB_INSTALL_DIR})
INSTALL(TARGETS ${TARGET_TPK_BACKEND} DESTINATION ${BINDIR})

# Export devel package
CONFIGURE_FILE(tpk-installer.pc.in tpk-installer.pc @ONLY)
INSTALL(FILES tpk-installer.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
INSTALL(DIRECTORY ./ DESTINATION ${INCLUDEDIR}/app-installers/tpk/
        FILES_MATCHING PATTERN "*.h")
