#include "usermanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

UserManager::UserManager(QObject *parent)
    : AbstractManager(parent)
{

}

void UserManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("userManager"), this);
}
