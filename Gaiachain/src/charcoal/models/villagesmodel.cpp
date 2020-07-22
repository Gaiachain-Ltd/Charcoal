#include "villagesmodel.h"

#include "helpers/requestshelper.h"
#include "rest/additionaldatarequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "common/logs.h"
#include "common/tags.h"
#include "listupdater.h"

#include <QSqlQuery>

VillagesModel::VillagesModel(QObject *parent) : SimpleListQueryModel(true, parent)
{
    setWebModelCanChange(true);
    setDbQuery("SELECT id, name, active FROM Villages WHERE active=1");
}

void VillagesModel::refreshWebData()
{
    const auto request = QSharedPointer<AdditionalDataRequest>::create(
        AdditionalDataRequest::DataType::Villages);

    if (RequestsHelper::isOnline(m_sessionManager.get(), m_userManager.get())) {
        request->setToken(m_sessionManager->token());
        m_sessionManager->sendRequest(request, this,
                                      &VillagesModel::webErrorHandler,
                                      &VillagesModel::webReplyHandler);
    } else {
        qDebug() << "Enqueing request";
        m_queuedRequests.append(request);
    }

    m_isPending = true;
}

void VillagesModel::webReplyHandler(const QJsonDocument &reply)
{
    m_isPending = false;
    ListUpdater updates("Villages", m_connectionName);
    if (updates.updateTable(reply, { Tags::name, Tags::active } )) {
        emit webDataRefreshed();
    } else {
        qWarning() << RED("Updating items has failed");
    }
}
