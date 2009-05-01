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

#include "privacyfunctions.h"

#include <ktoolinvocation.h>
#include <kconfig.h>
#include <kglobal.h>
#include <kdebug.h>
#include <krecentdocument.h>
#include <kstandarddirs.h>
#include <kbookmarkmanager.h>
#include <klocale.h>
#include <QtDBus/QtDBus>

#include <qstringlist.h>
#include <QFile>
#include <QDir>
#include <kconfiggroup.h>
#include <QProcess>
#include <QLatin1String>

bool ClearThumbnailsAction::action()
{
   // http://freedesktop.org/Standards/Home
   // http://triq.net/~jens/thumbnail-spec/index.html

   QDir thumbnailDir( QDir::homePath() + "/.thumbnails/normal");
   thumbnailDir.setFilter( QDir::Files );
   const QStringList entries = thumbnailDir.entryList();
   for( QStringList::const_iterator it = entries.begin() ; it != entries.end() ; ++it) {
      if(!thumbnailDir.remove(*it)) {
         errMsg = i18n("A thumbnail could not be removed.");
         return false;
      }
   }

   thumbnailDir.setPath(QDir::homePath() + "/.thumbnails/large");
   const QStringList entries2 = thumbnailDir.entryList();
   for( QStringList::const_iterator it = entries2.begin() ; it != entries2.end() ; ++it) {
      if(!thumbnailDir.remove(*it)) {
         errMsg = i18n("A thumbnail could not be removed.");
         return false;
      }
   }

   thumbnailDir.setPath(QDir::homePath() + "/.thumbnails/fail");
   const QStringList entries3 = thumbnailDir.entryList();
   for( QStringList::const_iterator it = entries3.begin() ; it != entries3.end() ; ++it) {
      if(!thumbnailDir.remove(*it)) {
         errMsg = i18n("A thumbnail could not be removed.");
         return false;
      }
   }

   return true;
}

bool ClearRunCommandHistoryAction::action()
{
   QDBusInterface krunner("org.kde.krunner", "/Interface", "org.kde.krunner.Interface");
   QDBusReply<void> reply = krunner.call("clearHistory");
   return reply.isValid();
}

bool ClearAllCookiesAction::action()
{
   QDBusInterface mediamanager("org.kde.kded", "/modules/kcookiejar", "org.kde.KCookieServer");
   QDBusReply<void> reply = mediamanager.call("deleteAllCookies");
   return reply.isValid();
}

bool ClearAllCookiesPoliciesAction::action()
{
   // load the config file and section
   KConfig cfg("kcookiejarrc");
   KConfigGroup group = cfg.group("Cookie Policy");

   kDebug() << "removing all saved cookie policies" ;
   group.deleteEntry("CookieDomainAdvice");
   cfg.sync();

   // inform the cookie jar we pillaged it
   QDBusInterface kcookiejar("org.kde.kded", "/modules/kcookiejar", "org.kde.KCookieServer");
   QDBusReply<void> reply = kcookiejar.call("reloadPolicy");

   return reply.isValid();
}

bool ClearSavedClipboardContentsAction::action()
{
   if(!QDBusConnection::sessionBus().interface()->isServiceRegistered("org.kde.klipper")) {
      KConfig *c = new KConfig("klipperrc", KConfig::NoGlobals);
      KConfigGroup group(c, "General");
      group.deleteEntry("ClipboardData");
      c->sync();

      delete c;
      return true;
   }
   QDBusInterface klipper("org.kde.klipper", "/klipper", "org.kde.klipper.klipper");
   QDBusReply<void> reply = klipper.call("clearClipboardHistory");
   return reply.isValid();
}

bool ClearFormCompletionAction::action()
{
   bool status;

   // try to delete the file, if it exists
   QFile completionFile(KStandardDirs::locateLocal("data", "khtml/formcompletions"));
   (completionFile.exists() ? status = completionFile.remove() : status = true);

   if (!status) {
      errMsg = i18n("The file exists but could not be removed.");
   }

   return status;
}

bool ClearWebCacheAction::action()
{
   QStringList lst;
   lst << "--clear-all";
   return QProcess::startDetached(KStandardDirs::findExe("kio_http_cache_cleaner"),lst);
}

bool ClearRecentDocumentsAction::action()
{
   KRecentDocument::clear();
   return KRecentDocument::recentDocuments().isEmpty();
}

bool ClearWebHistoryAction::action()
{
   // Clear the history from the memory of the running konquerors
   QDBusMessage message = QDBusMessage::createSignal("/KonqHistoryManager", "org.kde.Konqueror.HistoryManager", "notifyClear" );
   (void) QDBusConnection::sessionBus().send(message);

   // Delete the file
   const QString file = KStandardDirs::locateLocal("data", QLatin1String("konqueror/konq_history"));
   QFile::remove(file);
   return true;
}

bool ClearFaviconsAction::action()
{
   QDir favIconDir(KGlobal::dirs()->saveLocation( "cache", "favicons/" ));
   QStringList saveTheseFavicons;
   KBookmarkManager* konqiBookmarkMgr;

   konqiBookmarkMgr =
      KBookmarkManager::managerForFile(KStandardDirs::locateLocal("data",
            QLatin1String("konqueror/bookmarks.xml")), "konqueror");
   kDebug() << "saving the favicons that are in konqueror bookmarks" ;
   kDebug() << "opened konqueror bookmarks at " << konqiBookmarkMgr->path() ;

   // get the entire slew of bookmarks
   KBookmarkGroup konqiBookmarks = konqiBookmarkMgr->root();

   // walk through the bookmarks, if they have a favicon we should keep it
   KBookmark bookmark = konqiBookmarks.first();

   while (!bookmark.isNull()) {
      if ((bookmark.icon()).startsWith(QLatin1String("favicons/"))) {
         // pick out the name, throw .png on the end, and store the filename
         QRegExp regex("favicons/(.*)");
         regex.indexIn(bookmark.icon(), 0);
         kDebug() << "will save " << (regex.cap(1) + ".png") ;
         saveTheseFavicons << (regex.cap(1) + ".png");
      }
      bookmark = konqiBookmarks.next(bookmark);
   }

   favIconDir.setFilter( QDir::Files );

   const QStringList entries = favIconDir.entryList();

   // erase all files in favicon directory...
   for( QStringList::const_iterator it = entries.begin() ; it != entries.end() ; ++it) {
      // ...if we're not supposed to save them, of course
      if (!saveTheseFavicons.contains(*it)) {
         kDebug() << "removing " << *it ;
         if(!favIconDir.remove(*it)) {
            errMsg = i18n("A favicon could not be removed.");
            return false;
         }
      }
   }

   return true;
}

bool ClearRecentApplicationAction::action()
{
    QDBusMessage message =
        QDBusMessage::createSignal("/kickoff/RecentAppDoc", "org.kde.plasma", "cleanRecentDocumentsAndDocuments");
    QDBusConnection::sessionBus().send(message);

    return true;
}


// kate: tab-width 3; indent-mode cstyle; replace-tabs true;
