/**
  * kprivacymanager.h
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

#ifndef PRIVACYFUNCTIONS_H
#define PRIVACYFUNCTIONS_H

#include "privacyaction.h"

#include <KLocalizedString>


/**
@author Ralf Hoelzer
*/

class ClearAllCookiesAction : public PrivacyAction
{
   public:
      explicit ClearAllCookiesAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Cookies"),
                         i18n("Clears all stored cookies set by websites")) { }

      bool action() override;
      QString configKey() const override { return QStringLiteral("ClearAllCookies"); }
};

class ClearAllCookiesPoliciesAction : public PrivacyAction
{
   public:
      explicit ClearAllCookiesPoliciesAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Cookie Policies"),
                         i18n("Clears the cookie policies for all visited websites")) { }
      bool action() override;
      QString configKey() const override { return QStringLiteral("ClearAllCookiesPolicies"); }
};

class ClearSavedClipboardContentsAction : public PrivacyAction
{
   public:
      explicit ClearSavedClipboardContentsAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Saved Clipboard Contents"),
                         i18n("Clears the clipboard contents stored by Klipper")) { }
      bool action() override;
      QString configKey() const override { return QStringLiteral("ClearSavedClipboardContents"); }
};

class ClearThumbnailsAction : public PrivacyAction
{
   public:
      explicit ClearThumbnailsAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Thumbnail Cache"),
                         i18n("Clears all cached thumbnails")) { }
      bool action() override;
      QString configKey() const override { return QStringLiteral("ClearThumbnails"); }
};

class ClearRunCommandHistoryAction : public PrivacyAction
{
   public:
      explicit ClearRunCommandHistoryAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Run Command History"),
                         i18n("Clears the history of commands run through the Run Command tool on the desktop")) { }
      bool action() override;
      QString configKey() const override { return QStringLiteral("ClearRunCommandHistory"); }
};

class ClearFormCompletionAction : public PrivacyAction
{
   public:
      explicit ClearFormCompletionAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Form Completion Entries"),
                         i18n("Clears values which were entered into forms on websites")) { }
      bool action() override;
      QString configKey() const override { return QStringLiteral("ClearFormCompletion"); }
};

class ClearWebHistoryAction : public PrivacyAction
{
   public:
      explicit ClearWebHistoryAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Web History"),
                         i18n("Clears the history of visited websites")) { }
      bool action() override;
      QString configKey() const override { return QStringLiteral("ClearWebHistory"); }
};

class ClearWebCacheAction : public PrivacyAction
{
   public:
      explicit ClearWebCacheAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Web Cache"),
                         i18n("Clears the temporary cache of websites visited")) { }
      bool action() override;
      QString configKey() const override { return QStringLiteral("ClearWebCache"); }
};

class ClearRecentDocumentsAction : public PrivacyAction
{
   public:
      explicit ClearRecentDocumentsAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Recent Documents"),
                         i18n("Clears the list of recently used documents from the KDE applications menu")) { }
      bool action() override;
      QString configKey() const override { return QStringLiteral("ClearRecentDocuments"); }
};

class ClearFaviconsAction : public PrivacyAction
{
   public:
      explicit ClearFaviconsAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Favorite Icons"),
                         i18n("Clears the FavIcons cached from visited websites")) { }
      bool action() override;
      QString configKey() const override { return QStringLiteral("ClearFavIcons"); }
};

class ClearRecentApplicationAction : public PrivacyAction
{
   public:
      explicit ClearRecentApplicationAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Recent Applications"),
                         i18n("Clears the list of recently used applications from KDE menu")) { }
      bool action() override;
      QString configKey() const override { return QStringLiteral("ClearQuickStartMenu"); }
};


#endif

// kate: tab-width 3; indent-mode cstyle; replace-tabs true;
