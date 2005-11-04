/**
  * Copyright (C) 2005 Benjamin C Meyer (ben at meyerhome dot net)
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

#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <dcopclient.h>

#include "sweeper.h"

int main(int argc, char *argv[])
{
   KAboutData aboutData("sweeper", I18N_NOOP("Sweeper"), "1.0",
                        I18N_NOOP("Helps clean unwanted traces the user leaves on the system."),
                        KAboutData::License_LGPL, "(c) 2003-2005, Ralf Hoelzer", 0);
   
   aboutData.addAuthor("Ralf Hoelzer", I18N_NOOP("Original author"), "ralf@well.com");
   aboutData.addAuthor("Brian S. Stephan", I18N_NOOP("Maintainer"), "bssteph@irtonline.org");
   aboutData.addAuthor("Benjamin Meyer", I18N_NOOP("Thumbnail Cache"), "ben+kdeprivacy@meyerhome.net");
   
   // command line
   KCmdLineArgs::init(argc, argv, &aboutData);
   KApplication a(argc, argv);
   Sweeper *app = new Sweeper();
   a.setMainWidget(app);
   
   // register with DCOP
   a.dcopClient()->registerAs(a.name(), false);
   a.dcopClient()->setDefaultObject(app->objId());
   
   app->show();
   return a.exec();
}

// kate: tab-width 3; indent-mode cstyle; replace-tabs true;
