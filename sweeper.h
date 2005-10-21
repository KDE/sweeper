/**
  * Copyright (c) 2003-2005 Ralf Hoelzer <ralf@well.com>
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

#ifndef SWEEPER_H
#define SWEEPER_H

#include <klistview.h>
#include <kmainwindow.h>

#include <QLinkedList>

#include "privacyaction.h"
#include "sweeperdialog.h"

class Sweeper: public KMainWindow
{
    Q_OBJECT

signals:
    // TODO remove
    void changed(bool);
        
public:
    Sweeper(const char *name = 0);
    ~Sweeper();

    virtual void load();
    virtual void save();
    virtual void defaults();

public slots:
    void cleanup();
    void selectAll();
    void selectNone();

private:
    SweeperDialog  *cleaningDialog;

    QLinkedList<PrivacyAction*> checklist;

    KListViewItem *generalCLI;
    KListViewItem *webbrowsingCLI;

    PrivacyAction *clearThumbnails;	
    PrivacyAction *clearRunCommandHistory;
    PrivacyAction *clearAllCookies;
    PrivacyAction *clearSavedClipboardContents;
    PrivacyAction *clearWebHistory;
    PrivacyAction *clearWebCache;
    PrivacyAction *clearFormCompletion;
    PrivacyAction *clearRecentDocuments;
    PrivacyAction *clearQuickStartMenu;
    PrivacyAction *clearFavIcons;
    PrivacyAction *clearAllCookiePolicies;
    //QCheckListItem *clearFileDialogHistory;


};

#endif
