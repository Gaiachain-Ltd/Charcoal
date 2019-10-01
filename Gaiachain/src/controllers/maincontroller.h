#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "datamanager.h"
#include "pagemanager.h"
#include "usermanager.h"
#ifndef FAKE_DATA
#include "session/sessionmanager.h"
#else
#include "session/fakesessionmanager.h"
#endif

class MainController : public AbstractManager
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) Q_DECL_OVERRIDE;

signals:

public slots:

private:
    void setupConnections();

    DataManager m_dataManager;
    PageManager m_pageManager;
    UserManager m_userManager;
#ifndef FAKE_DATA
    SessionManager m_sessionManager;
#else
    FakeSessionManager m_sessionManager;
#endif

    void setupQZXing(QQmlApplicationEngine &engine);
};

#endif // MAINCONTROLLER_H
