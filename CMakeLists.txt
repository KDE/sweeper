project(sweeper)


add_definitions (-DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)

########### next target ###############

set(sweeper_SRCS 
   main.cpp 
   sweeper.cpp 
   privacyfunctions.cpp 
   privacyaction.cpp ) 


qt4_add_dbus_adaptor( sweeper_SRCS org.kde.sweeper.xml sweeper.h Sweeper)
		

kde4_add_ui_files(sweeper_SRCS sweeperdialog.ui )

kde4_add_executable(sweeper ${sweeper_SRCS})

target_link_libraries(sweeper ${KDE4_KFILE_LIBS})

install(TARGETS sweeper  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES sweeper.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR})
install( FILES sweeperui.rc  DESTINATION ${DATA_INSTALL_DIR}/sweeper)
install( FILES org.kde.sweeper.xml DESTINATION ${DBUS_INTERFACES_INSTALL_DIR} )

