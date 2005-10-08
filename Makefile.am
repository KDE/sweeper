AM_CPPFLAGS    = $(all_includes)

bin_PROGRAMS = privacy

privacy_SOURCES = main.cpp privacy.cpp kprivacydialog.ui kprivacymanager.cpp

INCLUDES = $(all_includes)

AM_LDFLAGS   = $(all_libraries) $(KDE_RPATH)
LDADD =  $(LIB_KFILE) $(LIB_KDEPRINT)

privacy_LDFLAGS = $(all_libraries)
privacy_LIBADD = $(LIB_KDEUI) $(LIB_KDECORE) -lkio

METASOURCES = AUTO

noinst_HEADERS = privacy.h

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/privacy.pot

xdg_apps_DATA = privacy.desktop

rcdir = $(kde_datadir)/privacy
rc_DATA = privacyui.rc
