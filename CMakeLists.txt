project(sweeper)



########### next target ###############

set(sweeper_SRCS 
   main.cpp 
   sweeper.cpp 
   privacyfunctions.cpp 
   privacyaction.cpp ) 


qt4_add_dbus_adaptor( sweeper_SRCS org.kde.sweeper.xml sweeper.h Sweeper)
		
kde4_automoc(${sweeper_SRCS})

kde4_add_ui3_files(sweeper_SRCS sweeperdialog.ui )

kde4_add_executable(sweeper ${sweeper_SRCS})

target_link_libraries(sweeper  ${KDE4_KIO_LIBS} )

install(TARGETS sweeper  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES sweeper.desktop  DESTINATION ${XDG_APPS_DIR})
install( FILES sweeperui.rc  DESTINATION ${DATA_INSTALL_DIR}/sweeper)




#original Makefile.am contents follow:

#AM_CPPFLAGS    = $(all_includes)
#
#bin_PROGRAMS = sweeper
#
#sweeper_SOURCES = main.cpp sweeper.cpp sweeperdialog.ui privacyfunctions.cpp \
#		  privacyaction.cpp dcopinterface.h dcopinterface.skel
#
#INCLUDES = $(all_includes)
#
#AM_LDFLAGS   = $(all_libraries) $(KDE_RPATH)
#LDADD =  $(LIB_KFILE) $(LIB_KDEPRINT)
#
#sweeper_LDFLAGS = $(all_libraries)
#sweeper_LIBADD = $(LIB_KDEUI) $(LIB_KDECORE) -lkio
#
#METASOURCES = AUTO
#
#noinst_HEADERS = sweeper.h
#
#messages: rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/sweeper.pot
#
#xdg_apps_DATA = sweeper.desktop
#
#rcdir = $(kde_datadir)/sweeper
#rc_DATA = sweeperui.rc
