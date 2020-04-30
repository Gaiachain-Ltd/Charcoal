#include <QGuiApplication>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

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
                     << "\nFlavor:"
#ifdef COCOA
                     << "Cocoa"
#elif CHARCOAL
                     << "Charcoal"
#endif
                     << "\nOrganisation:" << app.organizationName()
                     << "\nDomain:" << app.organizationDomain()
                     << "\nVersion:" << app.applicationVersion()
                     << "\nSHA:" << GitCommit
                     << "\nBuild date:" << BuildDate;

    QQmlApplicationEngine engine;
    MainController mc;
    mc.setupQmlContext(engine);

    Language language;
    language.load();

#ifdef Q_OS_ANDROID
    const int splashScreenTimeMs = 2000;
    QTimer::singleShot(splashScreenTimeMs, [&engine]() { engine.load(QUrl(QStringLiteral("qrc:///main.qml"))); } );
#else
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
#endif

    mc.startInitialWork();
    return app.exec();
}
