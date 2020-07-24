#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "abstractmanager.h"
#include "databasemanager.h"
#include "pagemanager.h"
#include "usermanager.h"

#include "common/languagemanager.h"

#ifdef Q_OS_ANDROID
#include "androidpermissionshandler.h"
#endif

#ifdef CHARCOAL
#include "charcoal/picturesmanager.h"
#include "charcoal/notificationmanager.h"
#endif

#include <QSharedDataPointer>

class RestSessionManager;
class AbstractDataManager;

class Application : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString domain READ domain CONSTANT)
    Q_PROPERTY(QString version READ version CONSTANT)
    Q_PROPERTY(QString commit READ commit CONSTANT)

public:
    Application(QObject *parent = nullptr) : QObject(parent)
    {
    }

    QString name() const
    {
        return AppName;
    }

    QString domain() const
    {
        return AppDomain;
    }

    QString version() const
    {
        return AppVersion;
    }

    QString commit() const
    {
        return GitCommit;
    }
};

class MainController : public AbstractManager
{
    Q_OBJECT

    Q_PROPERTY(QString flavor READ flavor CONSTANT)
    Q_PROPERTY(bool isDesktop READ isDesktop CONSTANT)

    Q_PROPERTY(bool isEasyLogin READ isEasyLogin CONSTANT)
    Q_PROPERTY(QStringList easyLoginList READ easyLoginList CONSTANT)
    Q_PROPERTY(QString easyLoginPassword READ easyLoginPassword CONSTANT)

    Q_PROPERTY(Application* application READ application CONSTANT)
    Q_PROPERTY(LanguageManager* languageManager READ languageManager CONSTANT)

public:
    explicit MainController(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine) override;
    void startInitialWork();

    QString flavor() const;
    bool isDesktop() const;

    bool isEasyLogin() const;
    QStringList easyLoginList() const;
    QString easyLoginPassword() const;

    Application *application() const;
    LanguageManager *languageManager() const;

private slots:
#ifdef Q_OS_ANDROID
    void onPermissionGranted(const Android::PermissionsHandler::Permission permission);
#endif

private:
    void setupConnections();
    void setupDataConnections();
    void initialWork();

    Application *m_application = nullptr;
    LanguageManager *m_languageManager = nullptr;

    PageManager m_pageManager;
    QSharedPointer<UserManager> m_userManager;

    DatabaseManager m_dbManager;

    QSharedPointer<RestSessionManager> m_sessionManager;
    AbstractDataManager *m_dataManager = nullptr;

#ifdef CHARCOAL
    PicturesManager m_picturesManager;
    NotificationManager m_notificationsManager;
#endif

    void setupQZXing(QQmlApplicationEngine &engine);
};

#endif // MAINCONTROLLER_H
