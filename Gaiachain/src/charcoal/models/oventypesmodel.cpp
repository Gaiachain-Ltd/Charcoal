#include "oventypesmodel.h"

#include "helpers/requestshelper.h"
#include "rest/additionaldatarequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "common/logs.h"
#include "listupdater.h"
#include "charcoal/database/charcoaldbhelpers.h"

#include <QSqlQuery>
#include <QDebug>

OvenTypesModel::OvenTypesModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(false);
    setDbQuery("SELECT id, name, type FROM OvenTypes");
}

QVariant OvenTypesModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() == false) {
        return {};
    }

    query().seek(index.row());

    switch (role) {
    case Qt::ItemDataRole::DisplayRole:
    case OvenTypesRole::TranslatedName:
        if (isTraditional()) {
            return tr("%1 (Traditional oven)").arg(query().value("name").toString());
        } else {
            return tr("%1 (Metallic oven)").arg(query().value("name").toString());
        }
        break;
    case OvenTypesRole::Id:
        return query().value("id").toInt();
    case OvenTypesRole::Name:
        return query().value("name").toString();
    case OvenTypesRole::IsTraditionalOven:
        return isTraditional();
    case OvenTypesRole::Type:
        return query().value("type").toInt();
    }

    return {};
}

QHash<int, QByteArray> OvenTypesModel::roleNames() const
{
    return m_roleNames;
}

void OvenTypesModel::refreshWebData()
{
    const auto request = QSharedPointer<AdditionalDataRequest>::create(
        AdditionalDataRequest::DataType::OvenTypes);

    if (RequestsHelper::isOnline(m_sessionManager.get(), m_userManager.get())) {
        request->setToken(m_sessionManager->token());
        m_sessionManager->sendRequest(request, this,
                                      &OvenTypesModel::webErrorHandler,
                                      &OvenTypesModel::webReplyHandler);
    } else {
        qDebug() << "Enqueing request";
        m_queuedRequests.append(request);
    }
}

void OvenTypesModel::webReplyHandler(const QJsonDocument &reply)
{
    //qDebug() << "Oven types:" << reply;

    ListUpdater updates("OvenTypes", m_connectionName);
    if (updates.updateTable(reply, { "name", "type", "oven_height",
                                    "oven_width", "oven_length" }))
    {
        emit webDataRefreshed();
    } else {
        qWarning() << RED("Updating items has failed");
    }
}

bool OvenTypesModel::isTraditional() const
{
    const int type(query().value("type").toInt());
    if (type == 1) {
        return true;
    } else if (type == CharcoalDbHelpers::metalOvenType) {
        return false;
    }

    return false;
}
