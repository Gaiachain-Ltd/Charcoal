#include "destinationsmodel.h"

#include "rest/additionaldatarequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "common/logs.h"
#include "listmodelhelper.h"

#include <QSqlQuery>

DestinationsModel::DestinationsModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(false);
    setDbQuery("SELECT name FROM Destinations");
}

void DestinationsModel::refreshWebData()
{
    const auto request = QSharedPointer<AdditionalDataRequest>::create(
        AdditionalDataRequest::DataType::Destinations);

    if (m_userManager->isLoggedIn()) {
        request->setToken(m_sessionManager->token());
        m_sessionManager->sendRequest(request, this,
                                      &DestinationsModel::webErrorHandler,
                                      &DestinationsModel::webReplyHandler);
    } else {
        qDebug() << "Enqueing request";
        m_queuedRequest = request;
    }
}

void DestinationsModel::webReplyHandler(const QJsonDocument &reply)
{
    TableUpdater updates("Destinations", m_connectionName);
    if (updates.updateTable("name", reply, query()) == false) {
        qWarning() << RED("Updating items has failed");
    }
}
