#include "treespeciesmodel.h"

#include "rest/additionaldatarequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "common/logs.h"
#include "listupdater.h"

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
        m_queuedRequests.append(request);
    }
}

void TreeSpeciesModel::webErrorHandler(const QString &errorString,
                                       const QNetworkReply::NetworkError code)
{
    qWarning() << RED("Web ERROR handler") << errorString << code;
}

void TreeSpeciesModel::webReplyHandler(const QJsonDocument &reply)
{
    ListUpdater updates("TreeSpecies", m_connectionName);
    if (updates.updateTable(reply, "name")) {
        emit webDataRefreshed();
    } else {
        qWarning() << RED("Updating items has failed");
    }
}
