#ifndef SESSION_H
#define SESSION_H

#include "abstractmanager.h"
#include "../rest/restapiclient.h"
#include "../rest/baserequest.h"

class OverlayManager;

class SessionManager : public AbstractManager
{
    Q_OBJECT
public:
    explicit SessionManager(QObject *parent = nullptr);

    void setOverlayManager(OverlayManager *manager);
    void setupQmlContext(QQmlApplicationEngine &engine);

    Q_INVOKABLE void login(const QString &email, const QString &password);

private:
    OverlayManager *m_overlayManager;
    RestAPIClient m_client;

    void displayErrorPopup(const QString &errorMessage);

signals:
    void displayError(const QString &error, const QString &button1, const QString &button2) const;

    void loginFinished(const QJsonDocument &doc) const;
};

#endif // SESSION_H
