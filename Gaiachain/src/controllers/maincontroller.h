#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "datamanager.h"
#include "pagemanager.h"
#include "usermanager.h"
#include "sessionmanager.h"

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
    SessionManager m_sessionManager;

    void setupQZXing(QQmlApplicationEngine &engine);
};

#endif // MAINCONTROLLER_H
