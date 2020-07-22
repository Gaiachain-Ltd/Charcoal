#include "treespeciesmodel.h"

#include "helpers/requestshelper.h"
#include "rest/additionaldatarequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "common/logs.h"
#include "common/tags.h"
#include "listupdater.h"

#include <QSqlQuery>

TreeSpeciesModel::TreeSpeciesModel(QObject *parent) : SimpleListQueryModel(parent)
{
    setWebModelCanChange(true);
    setDbQuery("SELECT id, name FROM TreeSpecies");
}

void TreeSpeciesModel::refreshWebData()
{
    const auto request = QSharedPointer<AdditionalDataRequest>::create(
        AdditionalDataRequest::DataType::TreeSpecies);

    if (RequestsHelper::isOnline(m_sessionManager.get(), m_userManager.get())) {
        request->setToken(m_sessionManager->token());
        m_sessionManager->sendRequest(request, this,
                                      &TreeSpeciesModel::webErrorHandler,
                                      &TreeSpeciesModel::webReplyHandler);
    } else {
        qDebug() << "Enqueing request";
        m_queuedRequests.append(request);
    }

    m_isPending = true;
}

void TreeSpeciesModel::webReplyHandler(const QJsonDocument &reply)
{
    m_isPending = false;
    ListUpdater updates("TreeSpecies", m_connectionName);
    if (updates.updateTable(reply, Tags::name)) {
        emit webDataRefreshed();
    } else {
        qWarning() << RED("Updating items has failed");
    }
}
