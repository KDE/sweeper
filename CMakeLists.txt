cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# KDE Application Version, managed by release script
set (RELEASE_SERVICE_VERSION_MAJOR "25")
set (RELEASE_SERVICE_VERSION_MINOR "04")
set (RELEASE_SERVICE_VERSION_MICRO "3")
set (RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(sweeper VERSION ${RELEASE_SERVICE_VERSION})

set(QT_REQUIRED_VERSION "6.7.0")
set(KF_MIN_VERSION "6.3.0")

find_package(ECM ${KF_MIN_VERSION} NO_MODULE REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMAddAppIcon)
include(ECMQtDeclareLoggingCategory)
include(FeatureSummary)
include(ECMDeprecationSettings)
include(KDEGitCommitHooks)
include(KDEClangFormat)
include(ECMInstallIcons)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.c)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

find_package(Qt6 ${QT_REQUIRED_VERSION} REQUIRED COMPONENTS Core DBus Widgets)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    Bookmarks
    Crash
    Config
    ConfigWidgets
    CoreAddons
    I18n
    TextWidgets
    XmlGui
)
find_package(KF6DocTools ${KF_MIN_VERSION})
set_package_properties(KF6DocTools PROPERTIES DESCRIPTION
    "Tools to generate documentation"
    TYPE OPTIONAL
)

find_package(PlasmaActivitiesStats REQUIRED)

ecm_set_disabled_deprecation_versions(QT 6.8.0 KF 6.12.0)
add_definitions(-DQT_NO_CONTEXTLESS_CONNECT)
include_directories (${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )

configure_file(config-sweeper.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config-sweeper.h
)

add_executable(sweeper)
target_sources(sweeper PRIVATE
   main.cpp 
   sweeper.cpp 
   privacyfunctions.cpp 
   privacyaction.cpp
   privacyaction.h
   sweeper.h
   privacyfunctions.h
   sweeper.qrc
   )
ecm_qt_declare_logging_category(sweeper HEADER sweeper_debug.h IDENTIFIER SWEEPER_LOG CATEGORY_NAME org.kde.sweeper DESCRIPTION "sweeper" EXPORT SWEEPER)


qt6_add_dbus_adaptor( sweeper_SRCS org.kde.sweeper.xml sweeper.h Sweeper)
target_sources(sweeper PRIVATE ${sweeper_SRCS})
		
ki18n_wrap_ui(sweeper sweeperdialog.ui )

target_link_libraries(sweeper
   Qt6::Core
   Qt6::DBus
   Qt6::Widgets
   Plasma::ActivitiesStats
   KF6::Bookmarks
   KF6::CoreAddons
   KF6::Crash
   KF6::I18n
   KF6::TextWidgets
   KF6::XmlGui
)

install(TARGETS sweeper  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS org.kde.sweeper.desktop  DESTINATION ${KDE_INSTALL_APPDIR})
install( FILES org.kde.sweeper.xml DESTINATION ${KDE_INSTALL_DBUSINTERFACEDIR} )
install( FILES org.kde.sweeper.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

ecm_install_icons(
    ICONS sc-apps-sweeper.svg
    DESTINATION ${KDE_INSTALL_ICONDIR}
)

ki18n_install(po)
if(KF6DocTools_FOUND)
    kdoctools_install(po)
    add_subdirectory(doc)
endif()
ecm_qt_install_logging_categories(
        EXPORT SWEEPER
        FILE sweeper.categories
        DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
        )
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
