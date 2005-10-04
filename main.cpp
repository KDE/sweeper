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
  *  Foundation, Inc., 51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.
  */

#include <kapplication.h>
#include "privacy.h"
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>

int main(int argc, char *argv[]){
  KAboutData aboutData("privacy", I18N_NOOP("KDE Privacy Tool"), "1.0",
    I18N_NOOP("Helps clean unwanted traces the user leaves on the system."), KAboutData::License_LGPL, "(c) 2003-2005, Ralf Hoelzer", 0);
    aboutData.addAuthor("Ralf Hoelzer", I18N_NOOP("Original author"), "ralf@well.com");
    aboutData.addAuthor("Brian S. Stephan", I18N_NOOP("Maintainer"), "bssteph@irtonline.org");
    aboutData.addAuthor("Benjamin Meyer", I18N_NOOP("Thumbnail Cache"), "ben+kdeprivacy@meyerhome.net");
  
  // command line
  KCmdLineArgs::init(argc, argv, &aboutData);
  KApplication a(argc, argv);
  Privacy *app = new Privacy();
  a.setMainWidget(app);

  app->show();
  return a.exec();
}

