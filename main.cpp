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

#include <QApplication>
#include <QCommandLineParser>
#include <QIcon>

#include <KAboutData>
#include <KCrash>
#include <Kdelibs4ConfigMigrator>
#include <KLocalizedString>

#include <config-sweeper.h>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   KLocalizedString::setApplicationDomain("sweeper");

    Kdelibs4ConfigMigrator migrator(QStringLiteral("sweeper"));
    migrator.setConfigFiles(QStringList() << QStringLiteral("sweeperrc"));
    migrator.setConfigFiles(QStringList() << QStringLiteral("kprivacyrc"));
    migrator.setUiFiles(QStringList() << QStringLiteral("sweeperui.rc"));
    migrator.migrate();

   KAboutData aboutData(QStringLiteral("sweeper"), i18n("Sweeper"),
                        QStringLiteral(SWEEPER_VERSION),
                        i18n("Helps clean unwanted traces the user leaves on the system."),
                        KAboutLicense::LGPL,
                        i18n("© 2003-2005, Ralf Hoelzer"),
                        QString(),
                        QStringLiteral("https://utils.kde.org/projects/sweeper"));

   aboutData.addAuthor(i18n("Ralf Hoelzer"), i18n("Original author"), QStringLiteral("ralf@well.com"));
   aboutData.addAuthor(i18n("Brian S. Stephan"), i18n("Maintainer"), QStringLiteral("bssteph@irtonline.org"));
   aboutData.addAuthor(i18n("Benjamin Meyer"), i18n("Thumbnail Cache"), QStringLiteral("ben+kdeprivacy@meyerhome.net"));

   // command line
   QCommandLineParser parser;
   KAboutData::setApplicationData(aboutData);
   a.setWindowIcon(QIcon::fromTheme(QStringLiteral("trash-empty")));

   parser.addOption(QCommandLineOption(QStringLiteral("automatic"),
                    i18n("Sweeps without user interaction")));
   aboutData.setupCommandLine(&parser);
   parser.process(a);
   aboutData.processCommandLine(&parser);

   KCrash::initialize();

   // Application
   Sweeper *app;
   if(parser.isSet(QStringLiteral("automatic"))) {
      app = new Sweeper(true);
   } else {
      app = new Sweeper(false);
      app->show();
   }
   return a.exec();
}

// kate: tab-width 3; indent-mode cstyle; replace-tabs true;
