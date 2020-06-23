#include "oventypesmodel.h"

#include "rest/additionaldatarequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "common/logs.h"
#include "listmodelhelper.h"

#include <QSqlQuery>
#include <QDebug>

OvenTypesModel::OvenTypesModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(false);
    setDbQuery("SELECT id, name FROM OvenTypes");
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
            return tr("Traditional oven");
        } else {
            return tr("Metallic oven");
        }
        break;
    case OvenTypesRole::Id:
        return query().value("id").toInt();
    case OvenTypesRole::Name:
        return query().value("name").toString();
    case OvenTypesRole::IsTraditionalOven:
        return isTraditional();
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

    if (m_userManager->isLoggedIn()) {
        request->setToken(m_sessionManager->token());
        m_sessionManager->sendRequest(request, this,
                                      &OvenTypesModel::webErrorHandler,
                                      &OvenTypesModel::webReplyHandler);
    } else {
        qDebug() << "Enqueing request";
        m_queuedRequest = request;
    }
}

void OvenTypesModel::webReplyHandler(const QJsonDocument &reply)
{
    ListUpdater updates("OvenTypes", m_connectionName);
    if (updates.updateTable(reply, { "name", "oven_height", "oven_width", "oven_length" }))
    {
        emit webDataRefreshed();
    } else {
        qWarning() << RED("Updating items has failed");
    }
}

bool OvenTypesModel::isTraditional() const
{
    const QString name(query().value("name").toString().toLower());
    if (name == QStringLiteral("metallic")) {
        return false;
    } else if (name == QStringLiteral("traditional")) {
        return true;
    }

    return false;
}
