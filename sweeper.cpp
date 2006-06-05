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
#include <kstdaction.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <dbus/qdbus.h>
#include "privacyfunctions.h"

#include "sweeper.h"
#include "sweeperadaptor.h"

Sweeper::Sweeper(const char *name)
   : KMainWindow(0, name)
{
   //setButtons( KDialogBase::Default|KDialogBase::Apply|KDialogBase::Help );
   
   cleaningDialog = new SweeperDialog(this);
   
   K3ListView *sw = cleaningDialog->privacyListView;
   
   sw->addColumn(i18n("Privacy Settings"));
   sw->addColumn(i18n("Description"));
   
   sw->setRootIsDecorated(true);
   sw->setTooltipColumn(1);
   sw->setColumnWidthMode(0, Q3ListView::Maximum);
   
   KStdAction::quit(this, SLOT(close()), actionCollection());
   
   createGUI("sweeperui.rc");
   
   generalCLI     = new K3ListViewItem(sw, i18n("General"));
   webbrowsingCLI = new K3ListViewItem(sw, i18n("Web Browsing"));
   
   generalCLI->setOpen(true);
   webbrowsingCLI->setOpen(true);
   
   sw->setWhatsThis( i18n("Check all cleanup actions you would like to perform. These will be executed by pressing the button below"));
   cleaningDialog->cleanupButton->setWhatsThis( i18n("Immediately performs the cleanup actions selected above"));
   
   this->InitActions();
   
   
   connect(cleaningDialog->cleanupButton, SIGNAL(clicked()), SLOT(cleanup()));
   connect(cleaningDialog->selectAllButton, SIGNAL(clicked()), SLOT(selectAll()));
   connect(cleaningDialog->selectNoneButton, SIGNAL(clicked()), SLOT(selectNone()));
   
   setCentralWidget(cleaningDialog);
   new KsweeperAdaptor(this);
   QDBus::sessionBus().registerObject("/ksweeper", this);
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
      (*itr)->setOn(group.readEntry((*itr)->text(), true));
   }
   
   delete c;
}


void Sweeper::defaults()
{
   selectNone();
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
}

void Sweeper::selectAll()
{
   QLinkedList<PrivacyAction*>::iterator itr;
   
   for (itr = checklist.begin(); itr != checklist.end(); ++itr) {
      (*itr)->setOn(true);
   }
   
}

void Sweeper::selectNone()
{
   QLinkedList<PrivacyAction*>::iterator itr;
   
   for (itr = checklist.begin(); itr != checklist.end(); ++itr) {
      (*itr)->setOn(false);
   }
   
}


void Sweeper::cleanup()
{
   if (KMessageBox::warningContinueCancel(this, i18n("You are deleting data that is potentially valuable to you. Are you sure?")) != KMessageBox::Continue) {
      return;
   }
   
   cleaningDialog->statusTextEdit->clear();
   cleaningDialog->statusTextEdit->setText(i18n("Starting cleanup..."));
   
   QLinkedList<PrivacyAction*>::iterator itr;
   
   for (itr = checklist.begin(); itr != checklist.end(); ++itr) {
      if((*itr)->isOn()) {
         QString statusText = i18n("Clearing %1...", (*itr)->text());
         cleaningDialog->statusTextEdit->append(statusText);
         
         // actions return whether they were successful
         if(!(*itr)->action()) {
            QString errorText =  i18n("Clearing of %1 failed: %2", (*itr)->text(), (*itr)->getErrMsg());
            cleaningDialog->statusTextEdit->append(errorText);
         }
      }
   }
   
   cleaningDialog->statusTextEdit->append(i18n("Clean up finished."));
}

void Sweeper::InitActions() {
    // store all entries in a list for easy access later on
   checklist.append(new ClearAllCookiesAction(webbrowsingCLI));
   checklist.append(new ClearFaviconsAction(webbrowsingCLI));
   checklist.append(new ClearWebHistoryAction(webbrowsingCLI));
   checklist.append(new ClearWebCacheAction(webbrowsingCLI));
   checklist.append(new ClearFormCompletionAction(webbrowsingCLI));
   checklist.append(new ClearAllCookiesPoliciesAction(webbrowsingCLI));
   checklist.append(new ClearSavedClipboardContentsAction(generalCLI));
   checklist.append(new ClearRecentDocumentsAction(generalCLI));
   checklist.append(new ClearQuickStartMenuAction(generalCLI));
   checklist.append(new ClearRunCommandHistoryAction(generalCLI));
   checklist.append(new ClearThumbnailsAction(generalCLI));
}

#include "sweeper.moc"

// kate: tab-width 3; indent-mode cstyle; replace-tabs true;
