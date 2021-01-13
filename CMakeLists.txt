cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# KDE Application Version, managed by release script
set (RELEASE_SERVICE_VERSION_MAJOR "21")
set (RELEASE_SERVICE_VERSION_MINOR "03")
set (RELEASE_SERVICE_VERSION_MICRO "70")
set (RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(sweeper VERSION ${RELEASE_SERVICE_VERSION})

set(REQUIRED_KF5_MIN_VERSION "5.68.0")
set(REQUIRED_QT_VERSION "5.12.0")

include(FeatureSummary)
find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Core DBus Widgets)
find_package(ECM ${REQUIRED_KF5_MIN_VERSION} NO_MODULE REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(ECMAddAppIcon)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)

add_definitions(
	-DQT_DISABLE_DEPRECATED_BEFORE=0x050C00
    -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT
    -DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x054400
)

find_package(KF5 ${REQUIRED_KF5_MIN_VERSION} REQUIRED COMPONENTS
    ActivitiesStats
    Bookmarks
    Crash
    Config
    ConfigWidgets
    CoreAddons
    DocTools
    I18n
    KIO
    TextWidgets
    XmlGui
)

if (KF5Config_VERSION VERSION_GREATER "5.56.0")
        add_definitions(-DQT_NO_FOREACH)
        MESSAGE(STATUS "compile without foreach")
endif()


include_directories (${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )

configure_file(config-sweeper.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/config-sweeper.h
)

set(sweeper_SRCS 
   main.cpp 
   sweeper.cpp 
   privacyfunctions.cpp 
   privacyaction.cpp ) 


qt5_add_dbus_adaptor( sweeper_SRCS org.kde.sweeper.xml sweeper.h Sweeper)
		
# file(GLOB ICONS_SRCS "${KDE4_INSTALL_DIR}/share/icons/oxygen/*/actions/trash-empty.png")
# ecm_add_app_icon(sweeper_SRCS ICONS ${ICONS_SRCS})

ki18n_wrap_ui(sweeper_SRCS sweeperdialog.ui )

add_executable(sweeper ${sweeper_SRCS})

target_link_libraries(sweeper
   Qt5::Core
   Qt5::DBus
   Qt5::Widgets
   KF5::ActivitiesStats
   KF5::Bookmarks
   KF5::Crash
   KF5::I18n
   KF5::KIOCore
   KF5::TextWidgets
   KF5::XmlGui
)

install(TARGETS sweeper  ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS org.kde.sweeper.desktop  DESTINATION ${KDE_INSTALL_APPDIR})
install( FILES sweeperui.rc  DESTINATION ${KXMLGUI_INSTALL_DIR}/sweeper)
install( FILES org.kde.sweeper.xml DESTINATION ${KDE_INSTALL_DBUSINTERFACEDIR} )
install( FILES org.kde.sweeper.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

add_subdirectory(doc)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

