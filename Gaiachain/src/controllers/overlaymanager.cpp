#include "overlaymanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDebug>

OverlayManager::OverlayManager(QObject *parent)
    : AbstractManager(parent)
{

}

void OverlayManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("overlay"), this);
}

bool OverlayManager::getLoginRequest() const
{
    return m_loginRequest;
}

void OverlayManager::setLoginRequest(const bool enable)
{
    if (m_loginRequest != enable) {
        m_loginRequest = enable;
        emit loginRequestChanged(m_loginRequest);
    }
}
