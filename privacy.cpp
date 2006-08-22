/**
  * privacy.cpp
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

#include <qcheckbox.h>
#include <qlayout.h>
#include <qptrlist.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include <kaboutdata.h>
#include <kconfig.h>
#include <kdialog.h>
#include <kglobal.h>
#include <klistview.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <ktextedit.h>

#include "privacy.h"

Privacy::Privacy(QWidget *parent, const char *name)
    : KCModule(parent, name)
{
  setQuickHelp( i18n("The privacy module allows a user to erase traces which KDE leaves on "
              "the system, such as command histories or browser caches."));

  setButtons( KCModule::Default|KCModule::Apply|KCModule::Help );

  KAboutData *about =
    new KAboutData(I18N_NOOP("kcm_privacy"), I18N_NOOP("KDE Privacy Control Module"),
                  0, 0, KAboutData::License_GPL,
                  I18N_NOOP("(c) 2003 Ralf Hoelzer"));

  about->addAuthor("Ralf Hoelzer", 0, "ralf@well.com");
  about->addAuthor("Benjamin Meyer", I18N_NOOP("Thumbnail Cache"), "ben+kdeprivacy@meyerhome.net");
  setAboutData( about );

  m_privacymanager = new KPrivacyManager();

  QBoxLayout *top = new QVBoxLayout(this, 0, KDialog::spacingHint());

  // add this once the P3P stuff is finished
  //QTabWidget *privacyTabs = new QTabWidget(this, "privacytabs");

  cleaningDialog = new KCMPrivacyDialog(this);
  //p3pSettings    = new KPrivacySettings(this);

  top->addWidget(cleaningDialog);

  //top->addWidget(privacyTabs);
  //privacyTabs->addTab(cleaningDialog, i18n("Cleanup"));
  //privacyTabs->addTab(p3pSettings, i18n("Privacy Settings"));


  KListView *sw = cleaningDialog->privacyListView;

  sw->addColumn(i18n("Privacy Settings"));
  sw->addColumn(i18n("Description"));

  sw->setRootIsDecorated(true);
  sw->setTooltipColumn(1);
  sw->setColumnWidthMode(0, QListView::Maximum);



  generalCLI     = new KListViewItem(sw, i18n("General") );
  webbrowsingCLI = new KListViewItem(sw, i18n("Web Browsing") );

  generalCLI->setOpen(true);
  webbrowsingCLI->setOpen(true);

  clearThumbnails = new QCheckListItem(generalCLI,
      i18n("Thumbnail Cache"),QCheckListItem::CheckBox);
  clearRunCommandHistory = new QCheckListItem(generalCLI,
      i18n("Run Command History"),QCheckListItem::CheckBox);
  clearAllCookies = new QCheckListItem(webbrowsingCLI,
      i18n("Cookies"),QCheckListItem::CheckBox);
  clearSavedClipboardContents = new QCheckListItem(generalCLI,
      i18n("Saved Clipboard Contents"),QCheckListItem::CheckBox);
  clearWebHistory = new QCheckListItem(webbrowsingCLI,
      i18n("Web History"),QCheckListItem::CheckBox);
  clearWebCache = new QCheckListItem(webbrowsingCLI,
      i18n("Web Cache"),QCheckListItem::CheckBox);
  clearFormCompletion = new QCheckListItem(webbrowsingCLI,
      i18n("Form Completion Entries"),QCheckListItem::CheckBox);
  clearRecentDocuments = new QCheckListItem(generalCLI,
      i18n("Recent Documents"),QCheckListItem::CheckBox);
  clearQuickStartMenu = new QCheckListItem(generalCLI,
      i18n("Quick Start Menu"),QCheckListItem::CheckBox);
  clearFavIcons = new QCheckListItem(webbrowsingCLI,
      i18n("Favorite Icons"),QCheckListItem::CheckBox);

  QWhatsThis::add(sw, i18n("Check all cleanup actions you would like to perform. These will be executed by pressing the button below"));
  QWhatsThis::add(cleaningDialog->cleanupButton, i18n("Immediately performs the cleanup actions selected above"));

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

  load();
}


Privacy::~Privacy()
{
    delete m_privacymanager;
}

void Privacy::load()
{
	load( false );
}

void Privacy::load(bool useDefaults)
{
  KConfig *c = new KConfig("kprivacyrc", false, false);

  c->setReadDefaults( useDefaults );

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
  emit changed( useDefaults );
}


void Privacy::defaults()
{
  load( true );
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
  QCheckListItem *item;

  for ( item  = checklist.first(); item; item = checklist.next() )
    item->setOn(true);

  emit changed(true);
}

void Privacy::selectNone()
{
  QCheckListItem *item;

  for ( item  = checklist.first(); item; item = checklist.next() )
    item->setOn(false);

  emit changed(true);
}


void Privacy::cleanup()
{
  if (KMessageBox::warningContinueCancel(this, i18n("You are deleting data that is potentially valuable to you. Are you sure?")) != KMessageBox::Continue) return;

  cleaningDialog->statusTextEdit->clear();
  cleaningDialog->statusTextEdit->setText(i18n("Starting cleanup..."));

  QCheckListItem *item;
  bool error = false;

  for ( item  = checklist.first(); item; item = checklist.next() )
  {
    if(item->isOn())
    {
      QString statusText = i18n("Clearing %1...").arg(item->text());
      cleaningDialog->statusTextEdit->append(statusText);

      if(item == clearThumbnails)
        error = m_privacymanager->clearThumbnails();

      if(item == clearRunCommandHistory)
        error = !m_privacymanager->clearRunCommandHistory();

      if(item == clearSavedClipboardContents)
        error = !m_privacymanager->clearSavedClipboardContents();

      if(item == clearAllCookies)
        error = !m_privacymanager->clearAllCookies();

      if(item == clearFormCompletion)
        error = !m_privacymanager->clearFormCompletion();

      if(item == clearWebCache)
        error = !m_privacymanager->clearWebCache();

      if(item == clearWebHistory)
        error = !m_privacymanager->clearWebHistory();

      if(item == clearRecentDocuments)
        error = !m_privacymanager->clearRecentDocuments();

      if(item == clearQuickStartMenu)
        error = !m_privacymanager->clearQuickStartMenu();

      if(item == clearFavIcons)
        error = m_privacymanager->clearFavIcons();

      if(error)
      {
        QString errorText =  i18n("Clearing of %1 failed").arg(item->text());
        cleaningDialog->statusTextEdit->append(errorText);
      }
    }
  }


  cleaningDialog->statusTextEdit->append(i18n("Clean up finished."));

}

extern "C"
{

  KDE_EXPORT KCModule *create_privacy(QWidget *parent, const char * /*name*/)
  {
    KGlobal::locale()->insertCatalogue("privacy");
    return new Privacy(parent, "Privacy");
  }
}

#include "privacy.moc"
