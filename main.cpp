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

#include "sweeper.h"

#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <klocale.h>

int main(int argc, char *argv[])
{
   KAboutData aboutData("sweeper", 0, ki18n("Sweeper"), "1.9",
                        ki18n("Helps clean unwanted traces the user leaves on the system."),
                        KAboutData::License_LGPL, ki18n("(c) 2003-2005, Ralf Hoelzer"), KLocalizedString(),
                        "http://utils.kde.org/projects/sweeper");

   aboutData.addAuthor(ki18n("Ralf Hoelzer"), ki18n("Original author"), "ralf@well.com");
   aboutData.addAuthor(ki18n("Brian S. Stephan"), ki18n("Maintainer"), "bssteph@irtonline.org");
   aboutData.addAuthor(ki18n("Benjamin Meyer"), ki18n("Thumbnail Cache"), "ben+kdeprivacy@meyerhome.net");

   aboutData.setProgramIconName(QLatin1String( "trash-empty" ));

   // command line
   KCmdLineArgs::init(argc, argv, &aboutData);
   KCmdLineOptions options;
   options.add("automatic", ki18n("Sweeps without user interaction"));
   KCmdLineArgs::addCmdLineOptions(options);
   KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

   // Application
   KApplication a(true);
   Sweeper *app;
   if(args->isSet("automatic")) {
      app = new Sweeper(true);
   } else {
      app = new Sweeper(false);
      app->show();
   }
   return a.exec();
}

// kate: tab-width 3; indent-mode cstyle; replace-tabs true;
