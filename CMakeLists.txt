project(sweeper)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    find_package(KDE4)
    include( KDE4Defaults )
    include_directories(${KDE4_INCLUDES})

    add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
    add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

    # If definitions like -D_GNU_SOURCE are needed for these checks they
    # should be added to _KDE4_PLATFORM_DEFINITIONS when it is originally
    # defined outside this file.  Here we include these definitions in
    # CMAKE_REQUIRED_DEFINITIONS so they will be included in the build of
    # checks below.
    set( CMAKE_REQUIRED_DEFINITIONS ${_KDE4_PLATFORM_DEFINITIONS} )
endif()

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
