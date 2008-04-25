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

#include "sweeper.h"

#include "privacyaction.h"
#include "privacyfunctions.h"
#include "sweeperadaptor.h"

#include <kaction.h>
#include <kstandardaction.h>
#include <kconfig.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <QtDBus/QtDBus>
#include <kactioncollection.h>
#include <kconfiggroup.h>

Sweeper::Sweeper()
   : KXmlGuiWindow(0)
{
   //setButtons( KDialogBase::Default|KDialogBase::Apply|KDialogBase::Help );

   QWidget *mainWidget = new QWidget(this);
   ui.setupUi(mainWidget);
   setCentralWidget(mainWidget);

   QTreeWidget *sw = ui.privacyListView;

   KStandardAction::quit(this, SLOT(close()), actionCollection());

   createGUI("sweeperui.rc");

   setAutoSaveSettings();

   generalCLI     = new QTreeWidgetItem(QStringList() << i18nc("General system content", "General"));
   sw->addTopLevelItem(generalCLI);
   webbrowsingCLI = new QTreeWidgetItem(QStringList() << i18nc("Web browsing content", "Web Browsing"));
   sw->addTopLevelItem(webbrowsingCLI);

   generalCLI->setExpanded(true);
   webbrowsingCLI->setExpanded(true);

   this->InitActions();


   connect(ui.cleanupButton, SIGNAL(clicked()), SLOT(cleanup()));
   connect(ui.selectAllButton, SIGNAL(clicked()), SLOT(selectAll()));
   connect(ui.selectNoneButton, SIGNAL(clicked()), SLOT(selectNone()));

   new KsweeperAdaptor(this);
   QDBusConnection::sessionBus().registerObject("/ksweeper", this);

   load();
}


Sweeper::~Sweeper()
{
   save();
}


void Sweeper::load()
{
   KConfig *c = new KConfig("kprivacyrc", KConfig::NoGlobals);

   // get general privacy settings
   KConfigGroup group(c, "Cleaning");

   QLinkedList<PrivacyAction*>::iterator itr;

   for (itr = checklist.begin(); itr != checklist.end(); ++itr) {
      (*itr)->setCheckState(0, group.readEntry((*itr)->text(0), true) ? Qt::Checked : Qt::Unchecked);
   }

   delete c;
}


void Sweeper::defaults()
{
   selectNone();
}


void Sweeper::save()
{
   KConfigGroup group(KSharedConfig::openConfig("kprivacyrc", KConfig::NoGlobals) , "Cleaning");

   QLinkedList<PrivacyAction*>::iterator itr;

   for (itr = checklist.begin(); itr != checklist.end(); ++itr) {
      group.writeEntry((*itr)->text(0), (*itr)->checkState(0) == Qt::Checked);
   }

   group.sync();
}

void Sweeper::selectAll()
{
   QLinkedList<PrivacyAction*>::iterator itr;

   for (itr = checklist.begin(); itr != checklist.end(); ++itr) {
      (*itr)->setCheckState(0, Qt::Checked);
   }

}

void Sweeper::selectNone()
{
   QLinkedList<PrivacyAction*>::iterator itr;

   for (itr = checklist.begin(); itr != checklist.end(); ++itr) {
      (*itr)->setCheckState(0, Qt::Unchecked);
   }

}


void Sweeper::cleanup()
{
   if (KMessageBox::warningContinueCancel(this, i18n("You are deleting data that is potentially valuable to you. Are you sure?")) != KMessageBox::Continue) {
      return;
   }

   ui.statusTextEdit->clear();
   ui.statusTextEdit->setText(i18n("Starting cleanup..."));

   QLinkedList<PrivacyAction*>::iterator itr;

   for (itr = checklist.begin(); itr != checklist.end(); ++itr) {
      if((*itr)->checkState(0) == Qt::Checked) {
         QString statusText = i18n("Clearing %1...", (*itr)->text(0));
         ui.statusTextEdit->append(statusText);

         // actions return whether they were successful
         if(!(*itr)->action()) {
            QString errorText =  i18n("Clearing of %1 failed: %2", (*itr)->text(0), (*itr)->getErrMsg());
            ui.statusTextEdit->append(errorText);
         }
      }
   }

   ui.statusTextEdit->append(i18n("Clean up finished."));
}

void Sweeper::InitActions() {
    // store all entries in a list for easy access later on
   checklist.append(new ClearSavedClipboardContentsAction(generalCLI));
   checklist.append(new ClearRecentDocumentsAction(generalCLI));
#ifdef Q_WS_X11
   checklist.append(new ClearRunCommandHistoryAction(generalCLI));
   checklist.append( new ClearRecentApplicationAction( generalCLI ) );
#endif
   checklist.append(new ClearThumbnailsAction(generalCLI));

   checklist.append(new ClearAllCookiesAction(webbrowsingCLI));
   checklist.append(new ClearFaviconsAction(webbrowsingCLI));
   checklist.append(new ClearWebHistoryAction(webbrowsingCLI));
   checklist.append(new ClearWebCacheAction(webbrowsingCLI));
   checklist.append(new ClearFormCompletionAction(webbrowsingCLI));
   checklist.append(new ClearAllCookiesPoliciesAction(webbrowsingCLI));

   ui.privacyListView->resizeColumnToContents(0);
}

#include "sweeper.moc"

// kate: tab-width 3; indent-mode cstyle; replace-tabs true;
