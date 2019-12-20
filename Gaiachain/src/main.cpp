/*******************************************************************************
Copyright (C) 2017 Milo Solutions
Contact: https://www.milosolutions.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/


/*
  TEMPLATE main.cpp by Milo Solutions. Copyright 2016
*/

#include <QGuiApplication>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#ifndef ENGLISH_LANGUAGE
#include <QTranslator>
#endif

#include "../src/controllers/maincontroller.h"
#include "../milo/mlog/mlog.h"
#include "../src/common/locale.h"


// Prepare logging categories. Modify these to your needs
//Q_DECLARE_LOGGING_CATEGORY(core) // already declared in MLog header
Q_LOGGING_CATEGORY(coreMain, "core.main")

/*!
  Main routine. Remember to update the application name and initialise logger
  class, if present.
  */
int main(int argc, char *argv[])
{
#ifdef Q_OS_ANDROID
    qputenv("QT_ANDROID_ENABLE_WORKAROUND_TO_DISABLE_PREDICTIVE_TEXT", QByteArrayLiteral("1"));
#endif

    MLog::instance()->enableLogToFile(AppName);

    // Set up basic application data. Modify this to your needs
    QGuiApplication app(argc, argv);
    app.setApplicationVersion(AppVersion);
    app.setOrganizationName(CompanyName);
    app.setOrganizationDomain(CompanyDomain);
    app.setApplicationName(AppName);

    qCInfo(coreMain) << "\nName:" << app.applicationName()
                     << "\nOrganisation:" << app.organizationName()
                     << "\nDomain:" << app.organizationDomain()
                     << "\nVersion:" << app.applicationVersion()
                     << "\nSHA:" << GitCommit
                     << "\nBuild date:" << BuildDate;

    QQmlApplicationEngine engine;
    MainController mc;
    mc.setupQmlContext(engine);

#ifndef ENGLISH_LANGUAGE
    QTranslator translator;
    translator.load(QLocale(), QLatin1String(DEFAULT_LANGUAGE), QLatin1String(), QLatin1String(":/translations"), QLatin1String(".qm"));
    QLocale::setDefault(QLocale(LocaleLanguage[DEFAULT_LANGUAGE]));
    qApp->installTranslator(&translator);
#else
    QLocale::setDefault(QLocale(LocaleLanguage["english"]));
#endif

#ifdef Q_OS_ANDROID
    const int splashScreenTimeMs = 2000;
    QTimer::singleShot(splashScreenTimeMs, [&engine]() { engine.load(QUrl(QStringLiteral("qrc:///main.qml"))); } );
#else
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
#endif

    mc.startInitialWork();
    return app.exec();
}
