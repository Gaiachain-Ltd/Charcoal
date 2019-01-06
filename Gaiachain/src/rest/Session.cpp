#include "Session.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(session, "core.session")

#include "RestAPIClient.h"
#include "LoginRequest.h"

Session::Session(QObject *parent)
    : QObject(parent)
{

}

void Session::login(const QString &email, const QString &password)
{
    auto request = QSharedPointer<LoginRequest>::create(email, password);

    mClient.send(request);
}
