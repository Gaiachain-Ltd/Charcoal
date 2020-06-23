#include "parcelsmodel.h"

#include "rest/additionaldatarequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "common/logs.h"
#include "listmodelhelper.h"

#include <QSqlQuery>

ParcelsModel::ParcelsModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(false);
    setDbQuery("SELECT name FROM Parcels");
}

void ParcelsModel::refreshWebData()
{
    const auto request = QSharedPointer<AdditionalDataRequest>::create(
        AdditionalDataRequest::DataType::Parcels);

    if (m_userManager->isLoggedIn()) {
        request->setToken(m_sessionManager->token());
        m_sessionManager->sendRequest(request, this,
                                      &ParcelsModel::webErrorHandler,
                                      &ParcelsModel::webReplyHandler);
    } else {
        qDebug() << "Enqueing request";
        m_queuedRequest = request;
    }
}

void ParcelsModel::webReplyHandler(const QJsonDocument &reply)
{
    TableUpdater updates("Parcels", m_connectionName);
    if (updates.updateTable("name", reply, query()) == false) {
        qWarning() << RED("Updating items has failed");
    }
}
