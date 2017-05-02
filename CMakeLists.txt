cmake_minimum_required(VERSION 2.8.12)

project(sweeper)

include(FeatureSummary)
find_package(Qt5 REQUIRED COMPONENTS Core DBus Widgets)
find_package(ECM 1.7.0 NO_MODULE REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(ECMAddAppIcon)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

add_definitions(
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_URL_CAST_FROM_STRING
)

find_package(KF5 5.15.0 REQUIRED COMPONENTS
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

