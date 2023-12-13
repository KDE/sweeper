/*
    SPDX-FileCopyrightText: 2005 Benjamin C Meyer <ben@meyerhome.net>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "sweeper.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QIcon>

#include <KAboutData>
#include <KCrash>
#include <KLocalizedString>

#include <config-sweeper.h>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   KLocalizedString::setApplicationDomain(QByteArrayLiteral("sweeper"));

   KAboutData aboutData(QStringLiteral("sweeper"), i18n("Sweeper"),
                        QStringLiteral(SWEEPER_VERSION),
                        i18n("Helps clean unwanted traces the user leaves on the system."),
                        KAboutLicense::LGPL,
                        i18n("© 2003-2005, Ralf Hoelzer"),
                        QString(),
                        QStringLiteral("https://apps.kde.org/sweeper"));

   aboutData.addAuthor(i18n("Ralf Hoelzer"), i18n("Original author"), QStringLiteral("ralf@well.com"));
   aboutData.addAuthor(i18n("Brian S. Stephan"), i18n("Maintainer"), QStringLiteral("bssteph@irtonline.org"));
   aboutData.addAuthor(i18n("Benjamin Meyer"), i18n("Thumbnail Cache"), QStringLiteral("ben+kdeprivacy@meyerhome.net"));

   // command line
   QCommandLineParser parser;
   KAboutData::setApplicationData(aboutData);
   a.setWindowIcon(QIcon::fromTheme(QStringLiteral("trash-empty")));

   const QCommandLineOption automaticOption(QStringLiteral("automatic"), i18n("Sweeps without user interaction"));
   parser.addOption(automaticOption);
   aboutData.setupCommandLine(&parser);
   parser.process(a);
   aboutData.processCommandLine(&parser);

   KCrash::initialize();

   // Application
   Sweeper *app;
   if (parser.isSet(automaticOption)) {
       app = new Sweeper(true);
   } else {
       app = new Sweeper(false);
       app->show();
   }
   return a.exec();
}

// kate: tab-width 3; indent-mode cstyle; replace-tabs true;
