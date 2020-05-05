#include <QGuiApplication>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#include "../src/controllers/maincontroller.h"
#include "../milo/mlog/mlog.h"

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
    auto mc = new MainController(&engine);
    mc->setupQmlContext(engine);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

#ifdef Q_OS_ANDROID
    const int splashScreenTimeMs = 2000;
    QTimer::singleShot(splashScreenTimeMs, [&engine, url]() { engine.load(url); } );
#else
    engine.load(url);
#endif

    mc->startInitialWork();
    return app.exec();
}
