#include "maincontroller.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QTimer>

#include <QZXing.h>

#include "mlog.h"
#include "common/globals.h"
#include "common/enums.h"
#include "common/dataglobals.h"
#include "helpers/utility.h"
#include "helpers/requestshelper.h"
#include "helpers/packagedataproperties.h"

#ifdef EASY_LOGIN
#include "../common/dummy/commondummydata.h"
#endif

#ifdef COCOA
#include "cocoa/cocoasessionmanager.h"
#include "cocoa/cocoadatamanager.h"
#include "cocoa/common/packagedata.h"
#include "cocoa/helpers/keywordfilterproxymodel.h"
#include "cocoa/helpers/modelhelper.h"
#elif CHARCOAL
#include "charcoal/tickmarkiconprovider.h"
#include "charcoal/charcoalsessionmanager.h"
#include "charcoal/charcoaldatamanager.h"
#include "charcoal/components/trapezoidovenitem.h"
#endif

template <typename Singleton>
QObject *registerCppOwnershipSingletonType(QQmlEngine *, QJSEngine *)
{
    QQmlEngine::setObjectOwnership(&Singleton::instance(), QQmlEngine::CppOwnership);
    return &Singleton::instance();
}

MainController::MainController(QObject *parent)
    : AbstractManager(parent),
      m_application(new Application(this)),
      m_languageManager(new LanguageManager(this)),
      m_userManager(QSharedPointer<UserManager>::create()),
#ifdef COCOA
      m_sessionManager(new CocoaSessionManager()),
      m_dataManager(new CocoaDataManager(this))
#elif CHARCOAL
      m_sessionManager(new CharcoalSessionManager()),
      m_dataManager(new CharcoalDataManager(m_sessionManager, m_userManager, this))
#endif
{
    qRegisterMetaType<QNetworkReply::NetworkError>("QNetworkReply::NetworkError");
    qRegisterMetaType<Qt::Orientation>("Qt::Orientation");

#ifdef CHARCOAL
    m_notificationsManager.setSessionManager(m_sessionManager);
    m_notificationsManager.setUserManager(m_userManager);
#endif

    setupConnections();

    m_languageManager->load();
}

void MainController::setupConnections()
{
    connect(m_userManager.get(), &UserManager::offlineModeChanged,
            m_sessionManager.get(), [sessionManager = m_sessionManager](bool offlineMode) {
                sessionManager->setEnabled(!offlineMode);
            });

    connect(m_userManager.get(), &UserManager::tokenChanged,
            m_sessionManager.get(), &AbstractSessionManager::updateToken);
    connect(m_sessionManager.get(), &AbstractSessionManager::loginAttempt,
            m_userManager.get(), &UserManager::handleLoginAttempt);
    connect(m_sessionManager.get(), &AbstractSessionManager::loginFinished,
            m_userManager.get(), &UserManager::readLoginData);

    connect(&m_dbManager, &DatabaseManager::databaseReady,
            m_dataManager, &AbstractDataManager::setupDatabase);
    connect(m_userManager.get(), &UserManager::userDataChanged,
            m_dataManager, &AbstractDataManager::updateUserData);

    setupDataConnections();
}

void MainController::setupDataConnections()
{
    connect(m_sessionManager.get(), &AbstractSessionManager::connectionStateChanged,
            m_dataManager, [dataManager = m_dataManager,
                            userManager = m_userManager.get(),
                            pageManager = &m_pageManager]
            (Enums::ConnectionState connectionState) {
                if (connectionState == Enums::ConnectionState::ConnectionSuccessful &&
                    !userManager->isOfflineMode() &&
                    pageManager->topPage() != Enums::Page::Login &&
                    pageManager->topPage() != Enums::Page::LoginLoading) {
                    dataManager->sendOfflineActions();
                }
            });

#ifdef COCOA
    const auto sessionManager = qobject_cast<CocoaSessionManager*>(m_sessionManager.get());
    const auto dataManager = qobject_cast<CocoaDataManager*>(m_dataManager);

    connect(dataManager, qOverload<const QString &, const Enums::SupplyChainAction &,
                                      const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&CocoaDataManager::addActionRequest),
            sessionManager, qOverload<const QString &, const Enums::SupplyChainAction &,
                                         const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&CocoaSessionManager::postNewEntity));
    connect(dataManager, qOverload<const QString &, const QByteArray &, const Enums::SupplyChainAction &,
                                      const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&CocoaDataManager::addActionRequest),
            sessionManager, qOverload<const QString &, const QByteArray &, const Enums::SupplyChainAction &,
                                         const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&CocoaSessionManager::postNewEntity));
    connect(dataManager, qOverload<const QByteArray &, const Enums::SupplyChainAction &,
                                      const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&CocoaDataManager::addActionRequest),
            sessionManager, qOverload<const QByteArray &, const Enums::SupplyChainAction &,
                                         const QGeoCoordinate &, const QDateTime &, const QVariantMap &>(&CocoaSessionManager::postNewEntity));

    connect(sessionManager, &CocoaSessionManager::entitySaved,
            dataManager, &CocoaDataManager::onActionAdded);
    connect(sessionManager, &CocoaSessionManager::entitySaveError,
            dataManager, &CocoaDataManager::onActionAddError);

    connect(dataManager, qOverload<const QDateTime &, const QDateTime &>(&CocoaDataManager::eventsInfoNeeded),
            sessionManager, qOverload<const QDateTime &, const QDateTime &>(&CocoaSessionManager::getEntitiesInfo));
    connect(dataManager, qOverload<int, int, const QDateTime &, const QDateTime &>(&CocoaDataManager::eventsInfoNeeded),
            sessionManager, qOverload<int, int, const QDateTime &, const QDateTime &>(&CocoaSessionManager::getEntitiesInfo));
    connect(dataManager, qOverload<int, int, const QString &, const QSet<Enums::PackageType> &, int>(&CocoaDataManager::eventsInfoNeeded),
            sessionManager, qOverload<int, int, const QString &, const QSet<Enums::PackageType> &, int>(&CocoaSessionManager::getEntitiesInfo));
    connect(dataManager, &CocoaDataManager::lastActionEventsInfoNeeded,
            sessionManager, &CocoaSessionManager::getLastActionEntitiesInfo);
    connect(dataManager, &CocoaDataManager::eventsNeeded,
            sessionManager, &CocoaSessionManager::getEntities);
    connect(dataManager, &CocoaDataManager::lastActionEventsInfoNeeded,
            sessionManager, &CocoaSessionManager::getLastActionEntitiesInfo);

    connect(sessionManager, &CocoaSessionManager::entitiesInfoLoaded,
            dataManager, &CocoaDataManager::onEntitiesInfoLoaded);
    connect(sessionManager, &CocoaSessionManager::entitiesLoaded,
            dataManager, &CocoaDataManager::onEntitiesLoaded);
    connect(sessionManager, &CocoaSessionManager::additionalDataLoaded,
            dataManager, &CocoaDataManager::onAdditionalDataLoaded);

    connect(sessionManager, &CocoaSessionManager::unusedLotIdsLoaded,
            dataManager, &CocoaDataManager::onUnusedLotIdsLoaded);

#elif CHARCOAL
    auto dataManager = qobject_cast<CharcoalDataManager *>(m_dataManager);

    connect(&m_notificationsManager, &NotificationManager::notify,
            &m_pageManager, &PageManager::showNotificationWithLink);
    connect(&m_notificationsManager, &NotificationManager::notify,
            dataManager, &CharcoalDataManager::refreshTracking);

    connect(dataManager, &CharcoalDataManager::error,
            &m_pageManager, &PageManager::onError);
#endif
}

void MainController::initialWork()
{
#ifdef Q_OS_ANDROID
    connect(&Android::PermissionsHandler::instance(),
            &Android::PermissionsHandler::permissionGranted,
            this, &MainController::onPermissionGranted);

    QMetaObject::invokeMethod(&Android::PermissionsHandler::instance(),
                              std::bind(&Android::PermissionsHandler::requestPermissions,
                                        &Android::PermissionsHandler::instance(),
                                        QList<Android::PermissionsHandler::Permission>{
                                            Android::PermissionsHandler::Permission::Internet,
                                            Android::PermissionsHandler::Permission::Storage
                                        }));
#else
    m_dbManager.setupDatabase();
#endif
}

void MainController::setupQmlContext(QQmlApplicationEngine &engine)
{
    // register namespace for enums
    qmlRegisterUncreatableMetaObject(Enums::staticMetaObject, "com.gaiachain.enums", 1, 0,
                                     "Enums", "Cannot create namespace Enums in QML");

    qRegisterMetaType<Enums::Page>("Enums::Page");
    qRegisterMetaType<Enums::Popup>("Enums::Popup");
    qRegisterMetaType<Enums::PopupAction>("Enums::PopupAction");
    qRegisterMetaType<Enums::ConnectionState>("Enums::ConnectionState");
    qRegisterMetaType<Enums::UserType>("Enums::UserType");
    qRegisterMetaType<Enums::PackageType>("Enums::PackageType");
    qRegisterMetaType<Enums::SupplyChainAction>("Enums::SupplyChainAction");
    qRegisterMetaType<Enums::DelegateType>("Enums::DelegateType");
    qRegisterMetaType<Gaia::ModelData>("Gaia::ModelData");
    qRegisterMetaType<Gaia::ModelEntry>("Gaia::ModelEntry");

    // register other types
#ifdef COCOA
    qRegisterMetaType<PackageData>("PackageData");
    qmlRegisterUncreatableType<PackageData>("com.gaiachain.types", 1, 0, "PackageData", "Cannot create types in QML");
#endif

    // register singleton types
    qmlRegisterSingletonType(QUrl("qrc:///GStrings.qml"), "com.gaiachain.style", 1, 0, "Strings");
#ifdef COCOA
    qmlRegisterSingletonType(QUrl("qrc:///cocoa/CocoaStyle.qml"), "com.gaiachain.style", 1, 0, "GStyle");
    qmlRegisterSingletonType<ModelHelper>("com.gaiachain.helpers", 1, 0,
                                          "ModelHelper", &registerCppOwnershipSingletonType<ModelHelper>);
    qmlRegisterSingletonType<PackageDataProperties>("com.gaiachain.types", 1, 0,
                                                    "PackageDataProperties", &registerCppOwnershipSingletonType<PackageDataProperties>);
#endif
#ifdef CHARCOAL
    qmlRegisterSingletonType(QUrl("qrc:///charcoal/CharcoalStyle.qml"), "com.gaiachain.style", 1, 0, "GStyle");
#endif
    qmlRegisterSingletonType(QUrl("qrc:///GStatic.qml"), "com.gaiachain.static", 1, 0, "Static");
    qmlRegisterSingletonType(QUrl("qrc:///common/Helper.qml"), "com.gaiachain.helpers", 1, 0, "Helper");

    qmlRegisterSingletonType<Utility>("com.gaiachain.helpers", 1, 0,
                                      "Utility", &registerCppOwnershipSingletonType<Utility>);
    qmlRegisterSingletonType<RequestsHelper>("com.gaiachain.helpers", 1, 0,
                                             "RequestHelper", &registerCppOwnershipSingletonType<RequestsHelper>);
    qmlRegisterSingletonType<DataGlobals>("com.gaiachain.helpers", 1, 0,
                                          "DataGlobals", &registerCppOwnershipSingletonType<DataGlobals>);

#ifdef Q_OS_ANDROID
    qmlRegisterSingletonType<Android::PermissionsHandler>("com.gaiachain.platforms", 1, 0,
                                                          "AndroidPermissionsHandler", &registerCppOwnershipSingletonType<Android::PermissionsHandler>);
#else
    qmlRegisterModule("com.gaiachain.platforms", 1, 0);
#endif

    // register qml types
#ifdef COCOA
    qmlRegisterType<KeywordFilterProxyModel>("com.gaiachain.helpers", 1, 0, "KeywordFilterProxyModel");
#endif

    // add context property
    engine.rootContext()->setContextProperty(QStringLiteral("mainController"), this);

    // setup other components
    m_languageManager->connectQmlEngine(&engine);
    m_pageManager.setupQmlContext(engine);
    m_userManager->setupQmlContext(engine);
    m_dbManager.setupQmlContext(engine);
    m_sessionManager->setupQmlContext(engine);
    m_dataManager->setupQmlContext(engine);

#ifdef CHARCOAL
    auto dataManager = qobject_cast<CharcoalDataManager *>(m_dataManager);
    dataManager->setPicturesManager(&m_picturesManager);
    m_picturesManager.setupQmlContext(engine);
    m_notificationsManager.setupQmlContext(engine);

    auto tickMarkProvider = new TickMarkIconProvider;
    tickMarkProvider->setPicturesManager(&m_picturesManager);
    engine.addImageProvider(TickMarkIconProvider::name(), tickMarkProvider);

    qmlRegisterType<TrapezoidOvenItem>("com.gaiachain.items", 1, 0,
                                       "TrapezoidOvenItem");
#endif

    setupQZXing(engine);
}

void MainController::startInitialWork()
{
    QMetaObject::invokeMethod(this, &MainController::initialWork);
}

QString MainController::flavor() const
{
#ifdef COCOA
    return QStringLiteral("cocoa");
#elif CHARCOAL
    return QStringLiteral("charcoal");
#else
    return QString();
#endif
}

bool MainController::isDesktop() const
{
#ifdef DESKTOP_TESTS
    return true;
#else
    return false;
#endif
}

bool MainController::isEasyLogin() const
{
#ifdef EASY_LOGIN
    return true;
#else
    return false;
#endif
}

QStringList MainController::easyLoginList() const
{
#ifdef EASY_LOGIN
    return CommonDummyData::availableLogins();
#else
    return QStringList();
#endif
}

QString MainController::easyLoginPassword() const
{
#ifdef EASY_LOGIN
    return CommonDummyData::commonPassword();
#else
    return QString();
#endif
}

Application *MainController::application() const
{
    return m_application;
}

LanguageManager *MainController::languageManager() const
{
    return m_languageManager;
}

#ifdef Q_OS_ANDROID
void MainController::onPermissionGranted(
    const Android::PermissionsHandler::Permission permission)
{
    qDebug() << "Permission granted" << permission;

    const auto androidPermission = Android::PermissionsHandler::Permission(permission);
    if (androidPermission == Android::PermissionsHandler::Permission::Storage) {
        qDebug() << "Enabling log to file" << ApplicationName;
        MLog::instance()->enableLogToFile(ApplicationName);
        qDebug() << "Enabled log to file" << MLog::instance()->currentLogPath();
    }

    m_dbManager.setupDatabase();
}
#endif

void MainController::setupQZXing(QQmlApplicationEngine &engine)
{
    QZXing::registerQMLTypes();
    QZXing::registerQMLImageProvider(engine);
}

