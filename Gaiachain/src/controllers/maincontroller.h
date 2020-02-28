#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "databasemanager.h"
#include "pagemanager.h"
#include "usermanager.h"
#include "data/datamanager.h"

#include "session/sessionmanager.h"

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
    SessionManager m_sessionManager;

    void setupQZXing(QQmlApplicationEngine &engine);
};

#endif // MAINCONTROLLER_H
