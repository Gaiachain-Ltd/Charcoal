#ifndef OVERLAYMANAGER_H
#define OVERLAYMANAGER_H

#include "abstractmanager.h"

class OverlayManager : public AbstractManager
{
    Q_OBJECT
    Q_PROPERTY(bool loginRequest READ loginRequest WRITE setLoginRequest NOTIFY loginRequestChanged)
public:
    explicit OverlayManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    bool loginRequest() const;
    void setLoginRequest(const bool enable);

signals:
    void loginRequestChanged(bool) const;

private:
    bool m_loginRequest = false;
};

#endif // OVERLAYMANAGER_H
