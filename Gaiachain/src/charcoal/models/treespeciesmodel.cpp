#include "treespeciesmodel.h"

#include "rest/additionaldatarequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "common/logs.h"
#include "listmodelhelper.h"

#include <QSqlQuery>

TreeSpeciesModel::TreeSpeciesModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(false);
    setDbQuery("SELECT name FROM TreeSpecies");
}

void TreeSpeciesModel::refreshWebData()
{
    const auto request = QSharedPointer<AdditionalDataRequest>::create(
        AdditionalDataRequest::DataType::TreeSpecies);

    if (m_userManager->isLoggedIn()) {
        request->setToken(m_sessionManager->token());
        m_sessionManager->sendRequest(request, this,
                                      &TreeSpeciesModel::webErrorHandler,
                                      &TreeSpeciesModel::webReplyHandler);
    } else {
        qDebug() << "Enqueing request";
        m_queuedRequest = request;
    }
}

void TreeSpeciesModel::webErrorHandler(const QString &error,
                                       const QNetworkReply::NetworkError code)
{
    qWarning() << RED("Web ERROR handler") << error << code;
}

void TreeSpeciesModel::webReplyHandler(const QJsonDocument &reply)
{
    const QString field("name");

    Updates updates("TreeSpecies", m_connectionName);
    if (updates.updateTable("name", reply, query()) == false) {
        qWarning() << RED("Updating items has failed");
    }
}
