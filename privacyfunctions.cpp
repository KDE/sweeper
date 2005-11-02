/**
 * kprivacymanager.cpp
 *
 * Copyright (c) 2003 Ralf Hoelzer <ralf@well.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation; either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <ktoolinvocation.h>
#include <dcopclient.h>
#include <kconfig.h>
#include <ksimpleconfig.h>
#include <kprocess.h>
#include <kglobal.h>
#include <kapplication.h>
#include <kdebug.h>
#include <krecentdocument.h>
#include <kstandarddirs.h>
#include <kbookmarkmanager.h>

#include <qstringlist.h>
#include <qfile.h>
#include <qdir.h>

#include "privacyfunctions.h"

namespace PrivacyFunctions
{

bool clearThumbnails()
{
  // http://freedesktop.org/Standards/Home
  // http://triq.net/~jens/thumbnail-spec/index.html
  
  bool error = false;

  QDir thumbnailDir( QDir::homePath() + "/.thumbnails/normal");
  thumbnailDir.setFilter( QDir::Files );
  QStringList entries = thumbnailDir.entryList();
  for( QStringList::Iterator it = entries.begin() ; it != entries.end() ; ++it)
    if(!thumbnailDir.remove(*it)) error = true;
  if(error) return error;

  thumbnailDir.setPath(QDir::homePath() + "/.thumbnails/large");
  entries = thumbnailDir.entryList();
  for( QStringList::Iterator it = entries.begin() ; it != entries.end() ; ++it)
    if(!thumbnailDir.remove(*it)) error = true;
  if(error) return error;

  thumbnailDir.setPath(QDir::homePath() + "/.thumbnails/fail");
  entries = thumbnailDir.entryList();
  for( QStringList::Iterator it = entries.begin() ; it != entries.end() ; ++it)
    if(!thumbnailDir.remove(*it)) error = true;
  
  return error;
}

bool clearRunCommandHistory()
{
  return kapp->dcopClient()->send( "kdesktop", "KDesktopIface", "clearCommandHistory()", QString("") );
}

bool clearAllCookies()
{
  return kapp->dcopClient()->send( "kded", "kcookiejar", "deleteAllCookies()", QString("") );
}

bool clearAllCookiePolicies()
{
  // load the config file and section
  KConfig cfg("kcookiejarrc");
  cfg.setGroup("Cookie Policy");
  
  kdDebug() << "removing all saved cookie policies" << endl;
  cfg.deleteEntry("CookieDomainAdvice");
  cfg.sync();
  
  // inform the cookie jar we pillaged it
  kapp->dcopClient()->send("kded", "kcookiejar", "reloadPolicy()", QString(""));
  
  return true;
}

bool clearSavedClipboardContents()
{
  if(!PrivacyFunctions::isApplicationRegistered("klipper"))
  {
    KConfig *c = new KConfig("klipperrc", false, false);

    {
      KConfigGroup group(c, "General");
      group.deleteEntry("ClipboardData");
      c->sync();
    }
    delete c;
    return true;
  }

  return kapp->dcopClient()->send( "klipper", "klipper", "clearClipboardHistory()", QString ("") );
}

bool clearFormCompletion()
{
  QFile completionFile(locateLocal("data", "khtml/formcompletions"));

  return completionFile.remove();
}

bool clearWebCache()
{
    KProcess process;
    process << "kio_http_cache_cleaner" << "--clear-all";
    return process.start(KProcess::DontCare);
}

bool clearRecentDocuments()
{
  KRecentDocument::clear();
  return KRecentDocument::recentDocuments().isEmpty();
}

bool clearQuickStartMenu()
{
  return kapp->dcopClient()->send( "kicker", "kicker", "clearQuickStartMenu()", QString ("") );
}

bool clearWebHistory()
{
  QStringList args("--preload");

  // preload Konqueror if it is not running
  if(!PrivacyFunctions::isApplicationRegistered("konqueror"))
  {
    kdDebug() << "couldn't find Konqueror instance, preloading." << endl;
    KToolInvocation::kdeinitExec("konqueror", args, 0,0);
  }

  return kapp->dcopClient()->send( "konqueror*", "KonqHistoryManager",
                                   "notifyClear(QCString)", QString ("") );
}

bool clearFavIcons()
{
  bool error = false;
  
  QDir favIconDir(KGlobal::dirs()->saveLocation( "cache", "favicons/" ));
  QStringList saveTheseFavicons;
  KBookmarkManager* konqiBookmarkMgr;
  
  konqiBookmarkMgr = 
    KBookmarkManager::managerForFile(locateLocal("data",
          QLatin1String("konqueror/bookmarks.xml")), false);
  kdDebug() << "saving the favicons that are in konqueror bookmarks" << endl;
  kdDebug() << "opened konqueror bookmarks at " << konqiBookmarkMgr->path() << endl;
  
  // get the entire slew of bookmarks
  KBookmarkGroup konqiBookmarks = konqiBookmarkMgr->root();
  
  // walk through the bookmarks, if they have a favicon we should keep it
  KBookmark bookmark = konqiBookmarks.first();
     
  while (!bookmark.isNull()) {
    if ((bookmark.icon()).startsWith("favicons/")) {
      // pick out the name, throw .png on the end, and store the filename
      QRegExp regex("favicons/(.*)");
      regex.indexIn(bookmark.icon(), 0);
      kdDebug() << "will save " << (regex.cap(1) + ".png") << endl;
      saveTheseFavicons << (regex.cap(1) + ".png");
    }
    bookmark = konqiBookmarks.next(bookmark);
  }
  
  favIconDir.setFilter( QDir::Files );
  
  QStringList entries = favIconDir.entryList();

  // erase all files in favicon directory...
  for( QStringList::Iterator it = entries.begin() ; it != entries.end() ; ++it)
    // ...if we're not supposed to save them, of course
    if (!saveTheseFavicons.contains(*it)) {
      kdDebug() << "removing " << *it << endl;
      if(!favIconDir.remove(*it)) error = true;
    }
  
  return error;
}


bool isApplicationRegistered(const QString &appName)
{
  DCOPCStringList regApps = kapp->dcopClient()->registeredApplications();

  for ( DCOPCStringList::Iterator it = regApps.begin(); it != regApps.end(); ++it )
    if((*it).find(appName.latin1()) != -1) return true;

  return false;
}

}
