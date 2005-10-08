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

#include <qcheckbox.h>
#include <qlayout.h>
#include <q3ptrlist.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qtooltip.h>

//Added by qt3to4:
#include <QVBoxLayout>
#include <QBoxLayout>

#include <kapplication.h>
#include <kmenu.h>
#include <kmenubar.h>
#include <kconfig.h>
#include <kdialog.h>
#include <kglobal.h>
#include <klistview.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <ktextedit.h>
#include <kmainwindow.h>
#include <kaction.h>
#include <kdebug.h>

#include "privacy.h"

Privacy::Privacy(const char *name)
    : KMainWindow(0, name)
{
  //setButtons( KDialogBase::Default|KDialogBase::Apply|KDialogBase::Help );

  m_privacymanager = new KPrivacyManager();

  // add this once the P3P stuff is finished
  //QTabWidget *privacyTabs = new QTabWidget(this, "privacytabs");

  cleaningDialog = new KPrivacyDialog(this);
  //p3pSettings    = new KPrivacySettings(this);

  //top->addWidget(privacyTabs);
  //privacyTabs->addTab(cleaningDialog, i18n("Cleanup"));
  //privacyTabs->addTab(p3pSettings, i18n("Privacy Settings"));


  KListView *sw = cleaningDialog->privacyListView;

  sw->addColumn(i18n("Privacy Settings"));
  sw->addColumn(i18n("Description"));

  sw->setRootIsDecorated(true);
  sw->setTooltipColumn(1);
  sw->setColumnWidthMode(0, Q3ListView::Maximum);

  KToggleAction *actionSaveFavis = new KToggleAction(i18n("Save Bookmark &Favicons"), 0,
					    actionCollection(), "save_favis");
  connect(actionSaveFavis, SIGNAL(toggled(bool)), SLOT(slotSaveFaviconsToggle(bool)));
  actionSaveFavis->setChecked(true);
  m_privacymanager->setSaveFavicons(true);
  
  createGUI("privacyui.rc");

  generalCLI     = new KListViewItem(sw, i18n("General") );
  webbrowsingCLI = new KListViewItem(sw, i18n("Web Browsing") );

  generalCLI->setOpen(true);
  webbrowsingCLI->setOpen(true);

  clearThumbnails = new Q3CheckListItem(generalCLI,
      i18n("Thumbnail Cache"),Q3CheckListItem::CheckBox);
  clearRunCommandHistory = new Q3CheckListItem(generalCLI,
      i18n("Run Command History"),Q3CheckListItem::CheckBox);
  clearAllCookies = new Q3CheckListItem(webbrowsingCLI,
      i18n("Cookies"),Q3CheckListItem::CheckBox);
  clearSavedClipboardContents = new Q3CheckListItem(generalCLI,
      i18n("Saved Clipboard Contents"),Q3CheckListItem::CheckBox);
  clearWebHistory = new Q3CheckListItem(webbrowsingCLI,
      i18n("Web History"),Q3CheckListItem::CheckBox);
  clearWebCache = new Q3CheckListItem(webbrowsingCLI,
      i18n("Web Cache"),Q3CheckListItem::CheckBox);
  clearFormCompletion = new Q3CheckListItem(webbrowsingCLI,
      i18n("Form Completion Entries"),Q3CheckListItem::CheckBox);
  clearRecentDocuments = new Q3CheckListItem(generalCLI,
      i18n("Recent Documents"),Q3CheckListItem::CheckBox);
  clearQuickStartMenu = new Q3CheckListItem(generalCLI,
      i18n("Quick Start Menu"),Q3CheckListItem::CheckBox);
  clearFavIcons = new Q3CheckListItem(webbrowsingCLI,
      i18n("Favorite Icons"),Q3CheckListItem::CheckBox);

  sw->setWhatsThis( i18n("Check all cleanup actions you would like to perform. These will be executed by pressing the button below"));
  cleaningDialog->cleanupButton->setWhatsThis( i18n("Immediately performs the cleanup actions selected above"));

  clearThumbnails->setText(1, i18n("Clears all cached thumbnails"));
  clearRunCommandHistory->setText(1, i18n("Clears the history of commands run through the Run Command tool on the desktop"));
  clearAllCookies->setText(1, i18n("Clears all stored cookies set by websites"));
  clearWebHistory->setText(1, i18n("Clears the history of visited websites"));
  clearSavedClipboardContents->setText(1, i18n("Clears the clipboard contents stored by Klipper"));
  clearWebCache->setText(1, i18n("Clears the temporary cache of websites visited"));
  clearFormCompletion->setText(1, i18n("Clears values which were entered into forms on websites"));
  clearRecentDocuments->setText(1, i18n("Clears the list of recently used documents from the KDE applications menu"));
  clearQuickStartMenu->setText(1, i18n("Clears the entries from the list of recently started applications"));
  clearFavIcons->setText(1, i18n("Clears the FavIcons cached from visited websites"));

  connect(sw, SIGNAL(selectionChanged()), SLOT(changed()));

  // store all entries in a list for easy access later on
  checklist.append(clearThumbnails);
  checklist.append(clearRunCommandHistory);
  checklist.append(clearAllCookies);
  checklist.append(clearSavedClipboardContents);
  checklist.append(clearWebHistory);
  checklist.append(clearWebCache);
  checklist.append(clearFormCompletion);
  checklist.append(clearRecentDocuments);
  checklist.append(clearQuickStartMenu);
  checklist.append(clearFavIcons);

  connect(cleaningDialog->cleanupButton, SIGNAL(clicked()), SLOT(cleanup()));
  connect(cleaningDialog->selectAllButton, SIGNAL(clicked()), SLOT(selectAll()));
  connect(cleaningDialog->selectNoneButton, SIGNAL(clicked()), SLOT(selectNone()));

  setCentralWidget(cleaningDialog);
}


Privacy::~Privacy()
{
    delete m_privacymanager;
}


void Privacy::load()
{
  KConfig *c = new KConfig("kprivacyrc", false, false);

  // get general privacy settings
  {
    KConfigGroupSaver saver(c, "Cleaning");

    clearThumbnails->setOn(c->readBoolEntry("ClearThumbnails", true));
    clearRunCommandHistory->setOn(c->readBoolEntry("ClearRunCommandHistory", true));
    clearAllCookies->setOn(c->readBoolEntry("ClearAllCookies", true));
    clearSavedClipboardContents->setOn(c->readBoolEntry("ClearSavedClipboardContents", true));
    clearWebHistory->setOn(c->readBoolEntry("ClearWebHistory", true));
    clearWebCache->setOn(c->readBoolEntry("ClearWebCache", true));
    clearFormCompletion->setOn(c->readBoolEntry("ClearFormCompletion", true));
    clearRecentDocuments->setOn(c->readBoolEntry("ClearRecentDocuments", true));
    clearQuickStartMenu->setOn(c->readBoolEntry("ClearQuickStartMenu", true));
    clearFavIcons->setOn(c->readBoolEntry("ClearFavIcons", true));
  }

  {
    KConfigGroupSaver saver(c, "P3P");

    // TODO: add P3P settings here
  }

  delete c;
  emit changed(false);

}


void Privacy::defaults()
{
  selectNone();
  emit changed(true);
}


void Privacy::save()
{
  KConfig *c = new KConfig("kprivacyrc", false, false);
  {
    KConfigGroupSaver saver(c, "Cleaning");

    c->writeEntry("ClearThumbnails", clearThumbnails->isOn());
    c->writeEntry("ClearRunCommandHistory", clearRunCommandHistory->isOn());
    c->writeEntry("ClearAllCookies", clearAllCookies->isOn());
    c->writeEntry("ClearSavedClipboardContents", clearSavedClipboardContents->isOn());
    c->writeEntry("ClearWebCache", clearWebCache->isOn());
    c->writeEntry("ClearWebHistory", clearWebHistory->isOn());
    c->writeEntry("ClearFormCompletion", clearFormCompletion->isOn());
    c->writeEntry("ClearRecentDocuments", clearRecentDocuments->isOn());
    c->writeEntry("ClearQuickStartMenu", clearQuickStartMenu->isOn());
    c->writeEntry("ClearFavIcons", clearFavIcons->isOn());
  }

  {
    KConfigGroupSaver saver(c, "P3P");

    // TODO: add settings for P3P
  }

  c->sync();

  delete c;
  emit changed(false);

}

void Privacy::selectAll()
{
  QLinkedList<Q3CheckListItem*>::iterator itr;

  for (itr = checklist.begin(); itr != checklist.end(); ++itr)
    (*itr)->setOn(true);

  emit changed(true);
}

void Privacy::selectNone()
{
  QLinkedList<Q3CheckListItem*>::iterator itr;

  for (itr = checklist.begin(); itr != checklist.end(); ++itr)
    (*itr)->setOn(false);

  emit changed(true);
}

void Privacy::slotSaveFaviconsToggle(bool toggle) {
  kdDebug() << "save favicons: " << toggle << endl;
  m_privacymanager->setSaveFavicons(toggle);
  
  emit changed(true);
}

void Privacy::cleanup()
{
  if (KMessageBox::warningContinueCancel(this, i18n("You are deleting data that is potentially valuable to you. Are you sure?")) != KMessageBox::Continue) return;

  cleaningDialog->statusTextEdit->clear();
  cleaningDialog->statusTextEdit->setText(i18n("Starting cleanup..."));

  bool error = false;

  QLinkedList<Q3CheckListItem*>::const_iterator itr;

  for (itr = checklist.begin(); itr != checklist.end(); ++itr)
  {
    if((*itr)->isOn())
    {
      QString statusText = i18n("Clearing %1...").arg((*itr)->text());
      cleaningDialog->statusTextEdit->append(statusText);

      if((*itr) == clearThumbnails)
        error = m_privacymanager->clearThumbnails();

      if((*itr) == clearRunCommandHistory)
        error = !m_privacymanager->clearRunCommandHistory();

      if((*itr) == clearSavedClipboardContents)
        error = !m_privacymanager->clearSavedClipboardContents();

      if((*itr) == clearAllCookies)
        error = !m_privacymanager->clearAllCookies();

      if((*itr) == clearFormCompletion)
        error = !m_privacymanager->clearFormCompletion();

      if((*itr) == clearWebCache)
        error = !m_privacymanager->clearWebCache();

      if((*itr) == clearWebHistory)
        error = !m_privacymanager->clearWebHistory();

      if((*itr) == clearRecentDocuments)
        error = !m_privacymanager->clearRecentDocuments();

      if((*itr) == clearQuickStartMenu)
        error = !m_privacymanager->clearQuickStartMenu();

      if((*itr) == clearFavIcons)
        error = m_privacymanager->clearFavIcons();

      if(error)
      {
        QString errorText =  i18n("Clearing of %1 failed").arg((*itr)->text());
        cleaningDialog->statusTextEdit->append(errorText);
      }
    }
  }


  cleaningDialog->statusTextEdit->append(i18n("Clean up finished."));

}

#include "privacy.moc"
