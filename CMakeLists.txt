cmake_minimum_required(VERSION 2.8.12)

project(sweeper)

include(FeatureSummary)
find_package(Qt5 REQUIRED COMPONENTS Core DBus Widgets)
find_package(ECM 1.7.0 NO_MODULE REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(ECMAddAppIcon)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

find_package(KF5 REQUIRED COMPONENTS
    I18n
    Crash
    Config
    ConfigWidgets
    CoreAddons
    DocTools
    KDELibs4Support
)
include_directories (${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )

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
   KF5::KIOCore
   KF5::KDELibs4Support)

install(TARGETS sweeper  ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS sweeper.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR})
install( FILES sweeperui.rc  DESTINATION ${DATA_INSTALL_DIR}/sweeper)
install( FILES org.kde.sweeper.xml DESTINATION ${DBUS_INTERFACES_INSTALL_DIR} )

add_subdirectory(doc)
