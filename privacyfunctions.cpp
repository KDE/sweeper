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
#include <kconfig.h>
#include <ksimpleconfig.h>
#include <kprocess.h>
#include <kglobal.h>
#include <kapplication.h>
#include <kdebug.h>
#include <krecentdocument.h>
#include <kstandarddirs.h>
#include <kbookmarkmanager.h>
#include <klocale.h>
#include <dbus/qdbus.h>

#include <qstringlist.h>
#include <QFile>
#include <QDir>

#include "privacyfunctions.h"

bool ClearThumbnailsAction::action()
{
   // http://freedesktop.org/Standards/Home
   // http://triq.net/~jens/thumbnail-spec/index.html
   
   QDir thumbnailDir( QDir::homePath() + "/.thumbnails/normal");
   thumbnailDir.setFilter( QDir::Files );
   QStringList entries = thumbnailDir.entryList();
   for( QStringList::Iterator it = entries.begin() ; it != entries.end() ; ++it) {
      if(!thumbnailDir.remove(*it)) {
         errMsg = i18n("A thumbnail could not be removed.");
         return false;
      }
   }
   
   thumbnailDir.setPath(QDir::homePath() + "/.thumbnails/large");
   entries = thumbnailDir.entryList();
   for( QStringList::Iterator it = entries.begin() ; it != entries.end() ; ++it) {
      if(!thumbnailDir.remove(*it)) {
         errMsg = i18n("A thumbnail could not be removed.");
         return false;
      }
   }
   
   thumbnailDir.setPath(QDir::homePath() + "/.thumbnails/fail");
   entries = thumbnailDir.entryList();
   for( QStringList::Iterator it = entries.begin() ; it != entries.end() ; ++it) {
      if(!thumbnailDir.remove(*it)) {
         errMsg = i18n("A thumbnail could not be removed.");
         return false;
      }
   }
   
   return true;
}

bool ClearRunCommandHistoryAction::action()
{
   QDBusInterfacePtr kdesktop("org.kde.kdesktop", "/KDesktop", "org.kde.kdesktop.Desktop");
   QDBusReply<bool> reply = kdesktop->call("clearCommandHistory");
   return reply.isSuccess();
}

bool ClearAllCookiesAction::action()
{
   QDBusInterfacePtr mediamanager("org.kde.kded", "/modules/kcookiejar", "org.kde.KCookieServer");
   QDBusReply<bool> reply = mediamanager->call("deleteAllCookies");
   return reply.isSuccess();
}

bool ClearAllCookiesPoliciesAction::action()
{
   // load the config file and section
   KConfig cfg("kcookiejarrc");
   cfg.setGroup("Cookie Policy");
   
   kDebug() << "removing all saved cookie policies" << endl;
   cfg.deleteEntry("CookieDomainAdvice");
   cfg.sync();
   
   // inform the cookie jar we pillaged it
   QDBusInterfacePtr mediamanager("org.kde.kded", "/modules/kcookiejar", "org.kde.KCookieServer");
   mediamanager->call("reloadPolicy");
   
   return true;
}

bool ClearSavedClipboardContentsAction::action()
{
   if(!kapp->dcopClient()->isApplicationRegistered("klipper")) {
      KConfig *c = new KConfig("klipperrc", false, false);
      
      KConfigGroup group(c, "General");
      group.deleteEntry("ClipboardData");
      c->sync();
      
      delete c;
      return true;
   }
   QDBusInterfacePtr klipper("org.kde.klipper", "/Klipper", "org.kde.klipper.klipper");
   QDBusReply<bool> reply = klipper->call("clearClipboardHistory");
   return reply.isSuccess();
}

bool ClearFormCompletionAction::action()
{
   bool status;
   
   // try to delete the file, if it exists
   QFile completionFile(locateLocal("data", "khtml/formcompletions"));
   (completionFile.exists() ? status = completionFile.remove() : status = true);
   
   if (!status) {
      errMsg = i18n("The file exists but could not be removed.");
   }
   
   return status;
}

bool ClearWebCacheAction::action()
{
   KProcess process;
   process << "kio_http_cache_cleaner" << "--clear-all";
   return process.start(KProcess::DontCare);
}

bool ClearRecentDocumentsAction::action()
{
   KRecentDocument::clear();
   return KRecentDocument::recentDocuments().isEmpty();
}

bool ClearQuickStartMenuAction::action()
{
   return kapp->dcopClient()->send( "kicker", "kicker", "clearQuickStartMenu()", QString ("") );
}

bool ClearWebHistoryAction::action()
{
   QStringList args("--preload");
   
   // preload Konqueror if it is not running
   if(!kapp->dcopClient()->isApplicationRegistered("konqueror")) {
      kDebug() << "couldn't find Konqueror instance, preloading." << endl;
      KToolInvocation::kdeinitExec("konqueror", args, 0,0);
   }
   
   return kapp->dcopClient()->send("konqueror*", "KonqHistoryManager",
                                   "notifyClear(QCString)", QString (""));
}

bool ClearFaviconsAction::action()
{
   QDir favIconDir(KGlobal::dirs()->saveLocation( "cache", "favicons/" ));
   QStringList saveTheseFavicons;
   KBookmarkManager* konqiBookmarkMgr;
   
   konqiBookmarkMgr = 
      KBookmarkManager::managerForFile(locateLocal("data",
            QLatin1String("konqueror/bookmarks.xml")), false);
   kDebug() << "saving the favicons that are in konqueror bookmarks" << endl;
   kDebug() << "opened konqueror bookmarks at " << konqiBookmarkMgr->path() << endl;
   
   // get the entire slew of bookmarks
   KBookmarkGroup konqiBookmarks = konqiBookmarkMgr->root();
   
   // walk through the bookmarks, if they have a favicon we should keep it
   KBookmark bookmark = konqiBookmarks.first();
      
   while (!bookmark.isNull()) {
      if ((bookmark.icon()).startsWith("favicons/")) {
         // pick out the name, throw .png on the end, and store the filename
         QRegExp regex("favicons/(.*)");
         regex.indexIn(bookmark.icon(), 0);
         kDebug() << "will save " << (regex.cap(1) + ".png") << endl;
         saveTheseFavicons << (regex.cap(1) + ".png");
      }
      bookmark = konqiBookmarks.next(bookmark);
   }
   
   favIconDir.setFilter( QDir::Files );
   
   QStringList entries = favIconDir.entryList();
   
   // erase all files in favicon directory...
   for( QStringList::Iterator it = entries.begin() ; it != entries.end() ; ++it) {
      // ...if we're not supposed to save them, of course
      if (!saveTheseFavicons.contains(*it)) {
         kDebug() << "removing " << *it << endl;
         if(!favIconDir.remove(*it)) {
            errMsg = i18n("A favicon could not be removed.");
            return false;
         }
      }
   }
   
   return true;
}

// kate: tab-width 3; indent-mode cstyle; replace-tabs true;
