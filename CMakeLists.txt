project(sweeper)

find_package(KDE4)
include( KDE4Defaults )
include_directories(${KDE4_INCLUDES})

########### next target ###############

set(sweeper_SRCS 
   main.cpp 
   sweeper.cpp 
   privacyfunctions.cpp 
   privacyaction.cpp ) 


qt4_add_dbus_adaptor( sweeper_SRCS org.kde.sweeper.xml sweeper.h Sweeper)
		
kde4_add_app_icon(sweeper_SRCS "${KDE4_INSTALL_DIR}/share/icons/oxygen/*/actions/trash-empty.png")

kde4_add_ui_files(sweeper_SRCS sweeperdialog.ui )

kde4_add_executable(sweeper ${sweeper_SRCS})

target_link_libraries(sweeper ${KDE4_KIO_LIBS})

install(TARGETS sweeper  ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS sweeper.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR})
install( FILES sweeperui.rc  DESTINATION ${DATA_INSTALL_DIR}/sweeper)
install( FILES org.kde.sweeper.xml DESTINATION ${DBUS_INTERFACES_INSTALL_DIR} )

add_subdirectory(doc)
