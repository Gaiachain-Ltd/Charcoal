#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "databasemanager.h"
#include "pagemanager.h"
#include "usermanager.h"
#include "data/datamanager.h"
#ifndef FAKE_DATA
#include "session/sessionmanager.h"
#else
#include "session/dummy/fakesessionmanager.h"
#endif

class MainController : public AbstractManager
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine) override;
    void startInitialWork();

private:
    void setupConnections();
    void setupDataConnections();
    void initialWork();

    PageManager m_pageManager;
    UserManager m_userManager;

    DatabaseManager m_dbManager;
    DataManager m_dataManager;

#ifndef FAKE_DATA
    SessionManager m_sessionManager;
#else
    FakeSessionManager m_sessionManager;
#endif

#ifdef Q_OS_ANDROID
    void setupAppPermissions();
#endif
    void setupQZXing(QQmlApplicationEngine &engine);
};

#endif // MAINCONTROLLER_H
