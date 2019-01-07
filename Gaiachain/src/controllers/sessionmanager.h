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

    void connectFinishingLambda(QSharedPointer<BaseRequest> &sharedPointer, std::function<void()> &lambda);
};

#endif // SESSION_H
