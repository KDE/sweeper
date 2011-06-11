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

   QDir thumbnailDir( QDir::homePath() + QLatin1String( "/.thumbnails/normal" ));
   thumbnailDir.setFilter( QDir::Files );
   const QStringList entries = thumbnailDir.entryList();
   for( QStringList::const_iterator it = entries.begin() ; it != entries.end() ; ++it) {
      if(!thumbnailDir.remove(*it)) {
         errMsg = i18n("A thumbnail could not be removed.");
         return false;
      }
   }

   thumbnailDir.setPath(QDir::homePath() + QLatin1String( "/.thumbnails/large" ));
   const QStringList entries2 = thumbnailDir.entryList();
   for( QStringList::const_iterator it = entries2.begin() ; it != entries2.end() ; ++it) {
      if(!thumbnailDir.remove(*it)) {
         errMsg = i18n("A thumbnail could not be removed.");
         return false;
      }
   }

   thumbnailDir.setPath(QDir::homePath() + QLatin1String( "/.thumbnails/fail" ));
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
   QDBusInterface krunner(QLatin1String( "org.kde.krunner" ), QLatin1String( "/App" ), QLatin1String( "org.kde.krunner.App" ));
   QDBusReply<void> reply = krunner.call(QLatin1String( "clearHistory" ));
   return reply.isValid();
}

bool ClearAllCookiesAction::action()
{
   QDBusInterface mediamanager(QLatin1String( "org.kde.kded" ), QLatin1String( "/modules/kcookiejar" ), QLatin1String( "org.kde.KCookieServer" ));
   QDBusReply<void> reply = mediamanager.call(QLatin1String( "deleteAllCookies" ));
   return reply.isValid();
}

bool ClearAllCookiesPoliciesAction::action()
{
   // load the config file and section
   KConfig cfg(QLatin1String( "kcookiejarrc" ));
   KConfigGroup group = cfg.group("Cookie Policy");

   kDebug() << "removing all saved cookie policies" ;
   group.deleteEntry("CookieDomainAdvice");
   cfg.sync();

   // inform the cookie jar we pillaged it
   QDBusInterface kcookiejar(QLatin1String( "org.kde.kded" ), QLatin1String( "/modules/kcookiejar" ), QLatin1String( "org.kde.KCookieServer" ));
   QDBusReply<void> reply = kcookiejar.call(QLatin1String( "reloadPolicy" ));

   return reply.isValid();
}

bool ClearSavedClipboardContentsAction::action()
{
   if(!QDBusConnection::sessionBus().interface()->isServiceRegistered(QLatin1String( "org.kde.klipper" ))) {
      KConfig *c = new KConfig(QLatin1String( "klipperrc" ), KConfig::NoGlobals);
      KConfigGroup group(c, "General");
      group.deleteEntry("ClipboardData");
      c->sync();

      delete c;
      return true;
   }
   QDBusInterface klipper(QLatin1String( "org.kde.klipper" ), QLatin1String( "/klipper" ), QLatin1String( "org.kde.klipper.klipper" ));
   QDBusReply<void> reply = klipper.call(QLatin1String( "clearClipboardHistory" ));
   return reply.isValid();
}

bool ClearFormCompletionAction::action()
{
   bool status;

   // try to delete the file, if it exists
   QFile completionFile(KStandardDirs::locateLocal("data", QLatin1String( "khtml/formcompletions" )));
   (completionFile.exists() ? status = completionFile.remove() : status = true);

   if (!status) {
      errMsg = i18n("The file exists but could not be removed.");
   }

   return status;
}

bool ClearWebCacheAction::action()
{
   QStringList lst;
   lst << QLatin1String( "--clear-all" );
   return QProcess::startDetached(KStandardDirs::findExe(QLatin1String( "kio_http_cache_cleaner" )),lst);
}

bool ClearRecentDocumentsAction::action()
{
   KRecentDocument::clear();
   return KRecentDocument::recentDocuments().isEmpty();
}

bool ClearWebHistoryAction::action()
{
   // Clear the history from the memory of the running konquerors
   QDBusMessage message = QDBusMessage::createSignal(QLatin1String( "/KonqHistoryManager" ), QLatin1String( "org.kde.Konqueror.HistoryManager" ), QLatin1String( "notifyClear" ) );
   (void) QDBusConnection::sessionBus().send(message);

   // Delete the file
   const QString file = KStandardDirs::locateLocal("data", QLatin1String("konqueror/konq_history"));
   QFile::remove(file);

   const QDBusMessage message2 = QDBusMessage::createSignal(QLatin1String( "/KonqUndoManager" ), QLatin1String( "org.kde.Konqueror.UndoManager" ), QLatin1String( "notifyRemove" ) );
   (void) QDBusConnection::sessionBus().send(message2);

   // Delete the file
   const QString file2 = KStandardDirs::locateLocal("data", QLatin1String("konqueror/closeditems_saved"));
   QFile::remove(file2);
   return true;
}

bool ClearFaviconsAction::action()
{
   QDir favIconDir(KGlobal::dirs()->saveLocation( "cache", QLatin1String( "favicons/" ) ));
   QStringList saveTheseFavicons;
   KBookmarkManager* konqiBookmarkMgr;

   konqiBookmarkMgr =
      KBookmarkManager::managerForFile(KStandardDirs::locateLocal("data",
            QLatin1String("konqueror/bookmarks.xml")), QLatin1String( "konqueror" ));
   kDebug() << "saving the favicons that are in konqueror bookmarks" ;
   kDebug() << "opened konqueror bookmarks at " << konqiBookmarkMgr->path() ;

   // get the entire slew of bookmarks
   KBookmarkGroup konqiBookmarks = konqiBookmarkMgr->root();

   // walk through the bookmarks, if they have a favicon we should keep it
   KBookmark bookmark = konqiBookmarks.first();

   while (!bookmark.isNull()) {
      if ((bookmark.icon()).startsWith(QLatin1String("favicons/"))) {
         // pick out the name, throw .png on the end, and store the filename
         QRegExp regex(QLatin1String( "favicons/(.*)" ));
         regex.indexIn(bookmark.icon(), 0);
         kDebug() << "will save " << (regex.cap(1) + QLatin1String( ".png" )) ;
         saveTheseFavicons << (regex.cap(1) + QLatin1String( ".png" ));
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
        QDBusMessage::createSignal(QLatin1String( "/kickoff/RecentAppDoc" ), QLatin1String( "org.kde.plasma" ), QLatin1String( "clearRecentDocumentsAndApplications" ));
    QDBusConnection::sessionBus().send(message);

    return true;
}


// kate: tab-width 3; indent-mode cstyle; replace-tabs true;
