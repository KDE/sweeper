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

#include <klistview.h>

#include <QString>

#include <Q3CheckListItem>

class PrivacyAction : public Q3CheckListItem
{
public:
    PrivacyAction(KListViewItem * parent, QString name, bool (*action)() = NULL, QString desc = QString::null);
    ~PrivacyAction();
    
    void setDescription(QString desc);
    
    bool doAction() const;
    
private:
    bool (*func)();
};

// kate: tab-width 4; indent-mode cstyle; replace-tabs true;
