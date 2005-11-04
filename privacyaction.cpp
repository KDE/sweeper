/**
  * Copyright (C) 2005 Brian S. Stephan ( bssteph at irtonline dot org )
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

#include "privacyaction.h"

PrivacyAction::PrivacyAction(KListViewItem * parent, QString name, QString desc)
   : Q3CheckListItem(parent, name, Q3CheckListItem::CheckBox)
{
   if (!desc.isNull()) {
      this->setText(1, desc);
   }
}

PrivacyAction::~PrivacyAction()
{
   // nothing to do
}

void PrivacyAction::setDescription(QString desc)
{
   this->setText(1, desc);
}

bool PrivacyAction::action()
{
   return false;
}

// kate: tab-width 3; indent-mode cstyle; replace-tabs true;
