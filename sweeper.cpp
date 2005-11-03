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

#include <kaction.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <dcopclient.h>

#include "privacyfunctions.h"

#include "sweeper.h"

Sweeper::Sweeper(const char *name)
    : DCOPObject("Actions"),
      KMainWindow(0, name)
{
    //setButtons( KDialogBase::Default|KDialogBase::Apply|KDialogBase::Help );
    
    cleaningDialog = new SweeperDialog(this);
    
    KListView *sw = cleaningDialog->privacyListView;
    
    sw->addColumn(i18n("Privacy Settings"));
    sw->addColumn(i18n("Description"));
    
    sw->setRootIsDecorated(true);
    sw->setTooltipColumn(1);
    sw->setColumnWidthMode(0, Q3ListView::Maximum);
    
    KStdAction::quit(this, SLOT(close()), actionCollection());
    
    createGUI("sweeperui.rc");
    
    generalCLI     = new KListViewItem(sw, i18n("General"));
    webbrowsingCLI = new KListViewItem(sw, i18n("Web Browsing"));
    
    generalCLI->setOpen(true);
    webbrowsingCLI->setOpen(true);
    
    sw->setWhatsThis( i18n("Check all cleanup actions you would like to perform. These will be executed by pressing the button below"));
    cleaningDialog->cleanupButton->setWhatsThis( i18n("Immediately performs the cleanup actions selected above"));
    
    this->InitActions();
    
    connect(sw, SIGNAL(selectionChanged()), SLOT(changed()));
    
    connect(cleaningDialog->cleanupButton, SIGNAL(clicked()), SLOT(cleanup()));
    connect(cleaningDialog->selectAllButton, SIGNAL(clicked()), SLOT(selectAll()));
    connect(cleaningDialog->selectNoneButton, SIGNAL(clicked()), SLOT(selectNone()));
    
    setCentralWidget(cleaningDialog);
}


Sweeper::~Sweeper()
{
    // cleanup pointers
    qDeleteAll(checklist.begin(), checklist.end());
}


void Sweeper::load()
{
    KConfig *c = new KConfig("kprivacyrc", false, false);
    
    // get general privacy settings
    KConfigGroup group(c, "Cleaning");
    
    QLinkedList<PrivacyAction*>::iterator itr;
    
    for (itr = checklist.begin(); itr != checklist.end(); ++itr) {
        (*itr)->setOn(group.readBoolEntry((*itr)->text(), true));
    }
    
    delete c;
    emit changed(false);
}


void Sweeper::defaults()
{
    selectNone();
    emit changed(true);
}


void Sweeper::save()
{
    KConfig *c = new KConfig("kprivacyrc", false, false);
    
    KConfigGroup group(c, "Cleaning");
    
    QLinkedList<PrivacyAction*>::iterator itr;
    
    for (itr = checklist.begin(); itr != checklist.end(); ++itr) {
        group.writeEntry((*itr)->text(), (*itr)->isOn());
    }
    
    c->sync();
    
    delete c;
    emit changed(false);
}

void Sweeper::selectAll()
{
    QLinkedList<PrivacyAction*>::iterator itr;
    
    for (itr = checklist.begin(); itr != checklist.end(); ++itr) {
        (*itr)->setOn(true);
    }
    
    emit changed(true);
}

void Sweeper::selectNone()
{
    QLinkedList<PrivacyAction*>::iterator itr;
    
    for (itr = checklist.begin(); itr != checklist.end(); ++itr) {
        (*itr)->setOn(false);
    }
    
    emit changed(true);
}


void Sweeper::cleanup()
{
    if (KMessageBox::warningContinueCancel(this, i18n("You are deleting data that is potentially valuable to you. Are you sure?")) != KMessageBox::Continue) {
        return;
    }
    
    cleaningDialog->statusTextEdit->clear();
    cleaningDialog->statusTextEdit->setText(i18n("Starting cleanup..."));
    
    QLinkedList<PrivacyAction*>::const_iterator itr;
    
    for (itr = checklist.begin(); itr != checklist.end(); ++itr) {
        if((*itr)->isOn()) {
            QString statusText = i18n("Clearing %1...").arg((*itr)->text());
            cleaningDialog->statusTextEdit->append(statusText);
            
            // actions return whether they were successful
            if(!(*itr)->doAction()) {
                QString errorText =  i18n("Clearing of %1 failed").arg((*itr)->text());
                cleaningDialog->statusTextEdit->append(errorText);
            }
        }
    }
    
    cleaningDialog->statusTextEdit->append(i18n("Clean up finished."));
}


void Sweeper::InitActions() {
    // store all entries in a list for easy access later on
    checklist.append(new PrivacyAction(generalCLI, i18n("Thumbnail Cache"),
                     PrivacyFunctions::clearThumbnails,
                     i18n("Clears all cached thumbnails")));
    
    checklist.append(new PrivacyAction(generalCLI, i18n("Run Command History"),
                     PrivacyFunctions::clearRunCommandHistory,
                     i18n("Clears the history of commands run through the Run Command tool on the desktop")));
    
    checklist.append(new PrivacyAction(webbrowsingCLI, i18n("Cookies"),
                     PrivacyFunctions::clearAllCookies,
                     i18n("Clears all stored cookies set by websites")));
    
    checklist.append(new PrivacyAction(webbrowsingCLI, i18n("Cookie Policies"),
                     PrivacyFunctions::clearAllCookiePolicies,
                     i18n("Clears the cookie policies for all visited websites")));
    
    checklist.append(new PrivacyAction(generalCLI, i18n("Saved Clipboard Contents"),
                     PrivacyFunctions::clearSavedClipboardContents,
                     i18n("Clears the clipboard contents stored by Klipper")));
    
    checklist.append(new PrivacyAction(webbrowsingCLI, i18n("Web History"),
                     PrivacyFunctions::clearWebHistory,
                     i18n("Clears the history of visited websites")));
    
    checklist.append(new PrivacyAction(webbrowsingCLI, i18n("Web Cache"),
                     PrivacyFunctions::clearWebCache,
                     i18n("Clears the temporary cache of websites visited")));
    
    checklist.append(new PrivacyAction(webbrowsingCLI, i18n("Form Completion Entries"),
                     PrivacyFunctions::clearFormCompletion,
                     i18n("Clears values which were entered into forms on websites")));
    
    checklist.append(new PrivacyAction(generalCLI, i18n("Recent Documents"),
                     PrivacyFunctions::clearRecentDocuments,
                     i18n("Clears the list of recently used documents from the KDE applications menu")));
    
    checklist.append(new PrivacyAction(generalCLI, i18n("Quick Start Menu"),
                     PrivacyFunctions::clearQuickStartMenu,
                     i18n("Clears the entries from the list of recently started applications")));
    
    checklist.append(new PrivacyAction(webbrowsingCLI, i18n("Favorite Icons"),
                     PrivacyFunctions::clearFavIcons,
                     i18n("Clears the FavIcons cached from visited websites")));
}

#include "sweeper.moc"

// kate: tab-width 4; indent-mode cstyle; replace-tabs true;
