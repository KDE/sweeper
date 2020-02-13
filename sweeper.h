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

#include <KConfigGroup>
#include <KXmlGuiWindow>

#include <QVector>

#include "ui_sweeperdialog.h"

class PrivacyAction;
class QTreeWidgetItem;

class Sweeper: public KXmlGuiWindow
{
   public:
      // if automatic is true, no user interaction is required
      explicit Sweeper(bool automatic);
      ~Sweeper();

      void cleanup();

   private:
      void selectAll();
      void selectNone();

      /**
       * methods
       */
      void load();
      void save();

      /**
       * Set up all the actions we are going to manage.
       */
      void InitActions();

      /**
       * attributes
       */
      Ui::SweeperDialog ui;

      QVector<PrivacyAction*> checklist;

      QTreeWidgetItem *generalCLI;
      QTreeWidgetItem *webbrowsingCLI;

      KConfigGroup m_privacyConfGroup;

      bool m_automatic;
};

#endif

// kate: tab-width 3; indent-mode cstyle; replace-tabs true;
