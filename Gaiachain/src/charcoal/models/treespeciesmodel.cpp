#include "treespeciesmodel.h"

#include "rest/additionaldatarequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"

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
        m_sessionManager->sendRequest(request, this,
                                      &TreeSpeciesModel::webErrorHandler,
                                      &TreeSpeciesModel::webReplyHandler);
    } else {
        m_queuedRequest = request;
    }
}

void TreeSpeciesModel::webErrorHandler(const QString &error,
                                       const QNetworkReply::NetworkError code)
{
    qDebug() << Q_FUNC_INFO << "Web ERROR handler" << error << code;
}

void TreeSpeciesModel::webReplyHandler(const QJsonDocument &reply)
{
    qDebug() << Q_FUNC_INFO << "Web reply handler" << reply;
}
