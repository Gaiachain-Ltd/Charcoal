#include "oventypesmodel.h"

#include "helpers/requestshelper.h"
#include "rest/additionaldatarequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "common/logs.h"
#include "common/tags.h"
#include "listupdater.h"
#include "charcoal/database/charcoaldbhelpers.h"

#include <QSqlQuery>
#include <QDebug>

OvenTypesModel::OvenTypesModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(true);
    setDbQuery("SELECT id, name, type, active FROM OvenTypes WHERE active=1");
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
            return tr("%1 (Traditional oven)").arg(query().value(Tags::name).toString());
        } else {
            return tr("%1 (Metallic oven)").arg(query().value(Tags::name).toString());
        }
        break;
    case OvenTypesRole::Id:
        return query().value(Tags::id).toInt();
    case OvenTypesRole::Name:
        return query().value(Tags::name).toString();
    case OvenTypesRole::IsTraditionalOven:
        return isTraditional();
    case OvenTypesRole::Type:
        return query().value(Tags::type).toInt();
    case OvenTypesRole::IsActive:
        return query().value(Tags::active).toInt();
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

    m_isPending = true;
}

void OvenTypesModel::webReplyHandler(const QJsonDocument &reply)
{
    m_isPending = false;
    //qDebug() << "Oven types:" << reply;

    ListUpdater updates("OvenTypes", m_connectionName);
    if (updates.updateTable(reply, { Tags::name, Tags::type, Tags::webOvenHeight,
                                    Tags::webOvenWidth, Tags::webOvenLength }))
    {
        emit webDataRefreshed();
    } else {
        qWarning() << RED("Updating items has failed");
    }
}

bool OvenTypesModel::isTraditional() const
{
    const int type(query().value(Tags::type).toInt());
    if (type == 1) {
        return true;
    } else if (type == CharcoalDbHelpers::metalOvenType) {
        return false;
    }

    return false;
}
