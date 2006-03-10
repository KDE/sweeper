kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(sweeper_SRCS 
   main.cpp 
   sweeper.cpp 
   privacyfunctions.cpp 
   privacyaction.cpp 
   dcopinterface.h )

kde4_automoc(${sweeper_SRCS})

kde4_add_ui_files(sweeper_SRCS sweeperdialog.ui )

kde4_add_dcop_skels(sweeper_SRCS dcopinterface.h )

kde4_add_executable(sweeper ${sweeper_SRCS})

target_link_libraries(sweeper  ${QT_AND_KDECORE_LIBS} kdeui kio )

install_targets(/bin sweeper )


########### install files ###############

install_files( ${XDG_APPS_DIR} FILES sweeper.desktop )
install_files( ${DATA_INSTALL_DIR}/sweeper FILES sweeperui.rc )

kde4_footer()



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
