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

#include <klocale.h>



#include "privacyaction.h"

/**
@author Ralf Hoelzer
*/

class ClearAllCookiesAction : public PrivacyAction
{
   public:
      ClearAllCookiesAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Cookies"),
                         i18n("Clears all stored cookies set by websites")) { }
      
      bool action();
};

class ClearAllCookiesPoliciesAction : public PrivacyAction
{
   public:
      ClearAllCookiesPoliciesAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Cookie Policies"),
                         i18n("Clears the cookie policies for all visited websites")) { }
      bool action();
};

class ClearSavedClipboardContentsAction : public PrivacyAction
{
   public:
      ClearSavedClipboardContentsAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Saved Clipboard Contents"),
                         i18n("Clears the clipboard contents stored by Klipper")) { }
      bool action();
};

class ClearThumbnailsAction : public PrivacyAction
{
   public:
      ClearThumbnailsAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Thumbnail Cache"),
                         i18n("Clears all cached thumbnails")) { }
      bool action();
};

class ClearRunCommandHistoryAction : public PrivacyAction
{
   public:
      ClearRunCommandHistoryAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Run Command History"),
                         i18n("Clears the history of commands run through the Run Command tool on the desktop")) { }
      bool action();
};

class ClearFormCompletionAction : public PrivacyAction
{
   public:
      ClearFormCompletionAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Form Completion Entries"),
                         i18n("Clears values which were entered into forms on websites")) { }
      bool action();
};

class ClearWebHistoryAction : public PrivacyAction
{
   public:
      ClearWebHistoryAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Web History"),
                         i18n("Clears the history of visited websites")) { }
      bool action();
};

class ClearWebCacheAction : public PrivacyAction
{
   public:
      ClearWebCacheAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Web Cache"),
                         i18n("Clears the temporary cache of websites visited")) { }
      bool action();
};

class ClearQuickStartMenuAction : public PrivacyAction
{
   public:
      ClearQuickStartMenuAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Quick Start Menu"),
                         i18n("Clears the entries from the list of recently started applications")) { }
      bool action();
};

class ClearRecentDocumentsAction : public PrivacyAction
{
   public:
      ClearRecentDocumentsAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Recent Documents"),
                         i18n("Clears the list of recently used documents from the KDE applications menu")) { }
      bool action();
};

class ClearFaviconsAction : public PrivacyAction
{
   public:
      ClearFaviconsAction(QTreeWidgetItem * parent)
         : PrivacyAction(parent, i18n("Favorite Icons"),
                         i18n("Clears the FavIcons cached from visited websites")) { }
      bool action();
};

#endif

// kate: tab-width 3; indent-mode cstyle; replace-tabs true;