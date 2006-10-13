project(sweeper)



########### next target ###############

set(sweeper_SRCS 
   main.cpp 
   sweeper.cpp 
   privacyfunctions.cpp 
   privacyaction.cpp ) 


qt4_add_dbus_adaptor( sweeper_SRCS org.kde.sweeper.xml sweeper.h Sweeper)
		
kde4_automoc(sweeper ${sweeper_SRCS})

kde4_add_ui_files(sweeper_SRCS sweeperdialog.ui )

kde4_add_executable(sweeper ${sweeper_SRCS})

target_link_libraries(sweeper  ${KDE4_KIO_LIBS} )

install(TARGETS sweeper  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES sweeper.desktop  DESTINATION ${XDG_APPS_DIR})
install( FILES sweeperui.rc  DESTINATION ${DATA_INSTALL_DIR}/sweeper)


