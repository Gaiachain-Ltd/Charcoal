#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "databasemanager.h"
#include "pagemanager.h"
#include "usermanager.h"
#include "data/datamanager.h"

#include "session/sessionmanager.h"

class Application : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString domain READ domain CONSTANT)
    Q_PROPERTY(QString version READ version CONSTANT)
    Q_PROPERTY(QString commmit READ commit CONSTANT)

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

    Q_PROPERTY(const Application* application READ application CONSTANT)

public:
    explicit MainController(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine) override;
    void startInitialWork();

    QString flavor() const;
    bool isDesktop() const;

    bool isEasyLogin() const;
    QStringList easyLoginList() const;
    QString easyLoginPassword() const;

    const Application *application() const;

private:
    void setupConnections();
    void setupDataConnections();
    void initialWork();

    Application m_application;

    PageManager m_pageManager;
    UserManager m_userManager;

    DatabaseManager m_dbManager;
    DataManager m_dataManager;
    SessionManager m_sessionManager;

    void setupQZXing(QQmlApplicationEngine &engine);
};

#endif // MAINCONTROLLER_H
