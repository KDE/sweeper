AM_CPPFLAGS    = $(all_includes)

bin_PROGRAMS = sweeper

sweeper_SOURCES = main.cpp sweeper.cpp sweeperdialog.ui privacyfunctions.cpp \
		  privacyaction.cpp dcopinterface.h dcopinterface.skel

INCLUDES = $(all_includes)

AM_LDFLAGS   = $(all_libraries) $(KDE_RPATH)
LDADD =  $(LIB_KFILE) $(LIB_KDEPRINT)

sweeper_LDFLAGS = $(all_libraries)
sweeper_LIBADD = $(LIB_KDEUI) $(LIB_KDECORE) -lkio

METASOURCES = AUTO

noinst_HEADERS = sweeper.h

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/sweeper.pot

xdg_apps_DATA = sweeper.desktop

rcdir = $(kde_datadir)/sweeper
rc_DATA = sweeperui.rc
