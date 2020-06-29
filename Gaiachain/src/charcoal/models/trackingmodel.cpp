#include "trackingmodel.h"

#include "database/dbhelpers.h"
#include "rest/baserequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "common/logs.h"
#include "common/tags.h"
#include "helpers/utility.h"
#include "trackingupdater.h"
#include "charcoal/database/charcoaldbhelpers.h"
#include "charcoal/picturesmanager.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDate>

TrackingModel::TrackingModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(true);
    setDbQuery("SELECT id, typeId, name, parent, webId FROM Entities");
}

void TrackingModel::setPicturesManager(PicturesManager *manager)
{
    m_picturesManager = manager;
}

QVariant TrackingModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() == false) {
        return {};
    }

    query().seek(index.row());

    switch (role) {
    case TrackingRole::Id:
        return query().value("id").toString();
    case TrackingRole::Name:
    case Qt::ItemDataRole::DisplayRole:
        return query().value("name").toString();
    case TrackingRole::Type:
    {
        const int id(query().value("typeId").toInt());
        return int(CharcoalDbHelpers::getEntityType(m_connectionName, id));
    }
    case TrackingRole::Events:
    {
        const QString id(query().value("id").toString());
        QSqlQuery query(QString(), db::Helpers::databaseConnection(m_connectionName));
        query.prepare("SELECT id, typeId, userId, date, properties FROM Events WHERE entityId=:id");
        query.bindValue(":id", id);

        if (query.exec() == false) {
            qWarning() << RED("Getting event details has failed!")
                       << query.lastError().text() << "for query:" << query.lastQuery();
            return {};
        }

        QVariantList events;
        while (query.next()) {
            const QDate date(query.value("date").toDate());
            Enums::SupplyChainAction action = CharcoalDbHelpers::actionById(
                m_connectionName, query.value("typeId").toInt());

            QString name;
            switch (action) {
            case Enums::SupplyChainAction::LoggingBeginning:
                name = tr("Logging has begun");
                break;
            case Enums::SupplyChainAction::LoggingEnding:
                name = tr("Logging has ended");
                break;
            case Enums::SupplyChainAction::CarbonizationBeginning:
            case Enums::SupplyChainAction::CarbonizationEnding:
            {
                const bool isBeginning = action == Enums::SupplyChainAction::CarbonizationBeginning;
                const QString begin(tr("Oven %1 - carbonization has begun"));
                const QString end(tr("Oven %1 - carbonization has ended"));
                const QString eventId(query.value("id").toString());
                const QString ovenQuery("SELECT name FROM Ovens WHERE %1=:eventId");

                QSqlQuery q(QString(), db::Helpers::databaseConnection(m_connectionName));
                if (isBeginning) {
                    q.prepare(ovenQuery.arg("carbonizationBeginning"));
                } else {
                    q.prepare(ovenQuery.arg("carbonizationEnding"));

                }
                q.bindValue(":eventId", eventId);

                if (q.exec() == false) {
                    qWarning() << RED("Getting oven name has failed!")
                               << q.lastError().text() << "for query:" << q.lastQuery();
                    return {};
                }

                q.next();
                const QString ovenName(q.value("name").toString());
                if (isBeginning) {
                    name = begin.arg(ovenName);
                } else {
                    name = end.arg(ovenName);
                }
            }
                break;
            case Enums::SupplyChainAction::LoadingAndTransport:
            {
                const QByteArray propertiesString(query.value("properties").toByteArray());
                const QJsonDocument propertiersJson(QJsonDocument::fromJson(propertiesString));
                const QVariantMap properties(propertiersJson.toVariant().toMap());
                name = tr("Bags have been loaded on truck %1")
                           .arg(properties.value(Tags::webPlateNumber).toString());
            }
                break;
            case Enums::SupplyChainAction::Reception:
                name = tr("Reception at storage facility");
                break;
            default: name = QString();
            }

            const QVariantMap event {
                { "eventName", name },
                { "date", date.toString("dd/MM/yyyy") }
            };

            events.append(event);
        }

        return events;
    }
    case TrackingRole::EventSummary:
    {
        /*
         * Algorithm:
         * -
         * - pack all data into Utility::createSummaryItem() and return
         */

        const int entityId = query().value("id").toInt();
        const QString entityName(query().value("name").toString());
        const int entityTypeId(query().value("typeId").toInt());
        const Enums::PackageType entityType = CharcoalDbHelpers::getEntityType(
            m_connectionName, entityTypeId);

        QSqlQuery q(QString(), db::Helpers::databaseConnection(m_connectionName));
        q.prepare("SELECT id, typeId, userId, date, properties FROM Events WHERE entityId=:id");
        q.bindValue(":id", entityId);

        if (q.exec() == false) {
            qWarning() << RED("Getting event details has failed!")
                       << q.lastError().text() << "for query:" << q.lastQuery();
            return {};
        }

        int eventCount = 0;
        QString userId;
        QVector<QJsonObject> properties;
        while (q.next()) {
            eventCount++;
            if (userId.isEmpty()) {
                userId = q.value("userId").toString();
            }

            properties.append(QJsonDocument::fromJson(q.value("properties").toString().toUtf8())
                                  .object());
        }

        QVariantList result;
        switch (entityType) {
        case Enums::PackageType::Plot:
        {
            int parcelId = -1;
            int villageId = -1;
            int treeSpeciesId = -1;
            qint64 beginningTimestamp = -1;
            qint64 endingTimestamp = -1;

            if (properties.isEmpty() == false) {
                const auto &first = properties.at(0);
                parcelId = first.value(Tags::webParcel).toInt(-1);
                villageId = first.value(Tags::webVillage).toInt(-1);
                treeSpeciesId = first.value(Tags::webTreeSpecies).toInt(-1);
                beginningTimestamp = first.value(Tags::webEventDate).toVariant().toLongLong();

                if (properties.length() > 1) {
                    endingTimestamp = properties.at(1).value(Tags::webEventDate)
                                          .toVariant().toLongLong();
                }
            }

            QString parcel;
            if (parcelId != -1) {
                parcel = CharcoalDbHelpers::getParcelCode(m_connectionName, parcelId);
            }

            QString village;
            if (villageId != -1) {
                village = CharcoalDbHelpers::getVillageName(m_connectionName, villageId);
            }

            QString treeSpecies;
            if (treeSpeciesId != -1) {
                treeSpecies = CharcoalDbHelpers::getTreeSpeciesName(m_connectionName, treeSpeciesId);
            }

            result.append(Utility::instance().createSummaryItem(
                tr("Plot ID"), entityName, QString(), QString(),
                m_plotHighlightColor, m_plotTextColor, QString(),
                Enums::DelegateType::Standard));
            result.append(Utility::instance().createSummaryItem(
                tr("Malebi Rep's ID"), userId));
            result.append(Utility::instance().createSummaryItem(
                tr("Parcel"), parcel));
            result.append(Utility::instance().createSummaryItem(
                tr("Village"), village));
            result.append(Utility::instance().createSummaryItem(
                tr("Beginning date"), dateString(beginningTimestamp)));
            result.append(Utility::instance().createSummaryItem(
                tr("Ending date"), dateString(endingTimestamp)));
            result.append(Utility::instance().createSummaryItem(
                tr("Tree species"), treeSpecies));
            break;
        }
        case Enums::PackageType::Harvest:
        {
            result.append(Utility::instance().createSummaryItem(
                tr("Harvest ID"), entityName, QString(), QString(),
                m_harvestHighlightColor, m_harvestTextColor, QString(),
                Enums::DelegateType::Standard));
            break;
        }
        case Enums::PackageType::Transport:
        {
            qint64 beginningTimestamp = -1;
            qint64 endingTimestamp = -1;
            QString plateNumber;
            QVariantList receptionData;

            if (properties.isEmpty() == false) {
                const auto &first = properties.at(0);
                plateNumber = first.value(Tags::webPlateNumber).toString();
                beginningTimestamp = first.value(Tags::webEventDate).toVariant().toLongLong();

                if (properties.length() > 1) {
                    const auto &second = properties.at(1);
                    endingTimestamp = second.value(Tags::webEventDate)
                                          .toVariant().toLongLong();

                    // TODO: create a helper struct and use it in Utility
                    // overload for createSummaryItem!!
                    QStringList docs;
                    QStringList recs;

                    const QString cache(m_picturesManager->cachePath());

                    const QJsonArray docsArray(second.value(Tags::documents).toArray());
                    for (const auto &value : docsArray) {
                        docs.append(cache + "/" + value.toString());
                    }

                    const QJsonArray recsArray(second.value(Tags::receipts).toArray());
                    for (const auto &value : recsArray) {
                        recs.append(cache + "/" + value.toString());
                    }

                    const bool hasDocs = (docs.isEmpty() == false);
                    const bool hasRecs = (recs.isEmpty() == false);

                    m_picturesManager->setCurrentDocuments(docs);
                    m_picturesManager->setCurrentReceipts(recs);

                    const QString uploaded(tr("Uploaded"));
                    const QString noPhoto(tr("No photo"));

                    const QString bagNumberString(QString::number(
                        CharcoalDbHelpers::bagCountInTransport(m_connectionName, entityId)));

                    Utility::SummaryValue value;
                    value.titles = QStringList {
                        bagNumberString,
                        dateString(beginningTimestamp),
                        QString(hasDocs? uploaded : noPhoto),
                        QString(hasRecs? uploaded : noPhoto)
                    };

                    value.values = QStringList {
                        tr("Number of bags"),
                        tr("Loading date"),
                        tr("Documents"),
                        tr("Receipts")
                    };

                    value.icons = QStringList {
                        QString(),
                        QString(),
                        "image://tickmark/document-" + QString(hasDocs? "true" : "false"),
                        "image://tickmark/receipt-" + QString(hasRecs? "true" : "false")
                    };

                    value.linkDestinationPages = QList<Enums::Page>{
                        Enums::Page::InvalidPage,
                        Enums::Page::InvalidPage,
                        (hasDocs? Enums::Page::PhotoGallery : Enums::Page::InvalidPage),
                        (hasRecs? Enums::Page::PhotoGallery : Enums::Page::InvalidPage)
                    };

                    value.linkDatas = QVariantList {
                        QVariant(),
                        QVariant(),
                        QVariantMap{ {"urls", docs} },
                        QVariantMap{ {"urls", recs} }
                    };

                    receptionData = value.toList();
                }
            }

            result.append(Utility::instance().createSummaryItem(
                tr("Transport ID"), entityName, QString(), QString(),
                m_transportHighlightColor, m_transportTextColor, QString(),
                Enums::DelegateType::Standard));
            result.append(Utility::instance().createSummaryItem(
                QString(), receptionData,
                QString(), QString(),
                m_harvestHighlightColor, m_harvestTextColor, QColor("000000"),
                Enums::DelegateType::ColumnStack));
            result.append(Utility::instance().createSummaryItem(
                tr("Plate number"), plateNumber));
            result.append(Utility::instance().createSummaryItem(
                tr("Reception at the storage facility"), dateString(endingTimestamp)));
            break;
        }
        default:
            result.append(Utility::instance().createSummaryItem(
                "default", "error"));
        }
        return result;
    }
    }

    return {};
}

QHash<int, QByteArray> TrackingModel::roleNames() const
{
    return m_roleNames;
}

void TrackingModel::refreshWebData()
{
    // TODO: limit amount of data with canFetchMore() etc.
    const auto request = QSharedPointer<BaseRequest>::create(
        "/entities/packages/",
        BaseRequest::Type::Get);

    if (m_userManager->isLoggedIn()) {
        request->setToken(m_sessionManager->token());
        m_sessionManager->sendRequest(request, this,
                                      &TrackingModel::webErrorHandler,
                                      &TrackingModel::webReplyHandler);
    } else {
        qDebug() << "Enqueing request";
        m_queuedRequests.append(request);
    }
}

void TrackingModel::webReplyHandler(const QJsonDocument &reply)
{
    qDebug() << "Data is:" << reply;
    TrackingUpdater updater(m_connectionName);
    if (updater.updateTable(reply)) {
        emit webDataRefreshed();
    } else {
        emit error(tr("Error updating tracking information"));
    }
}

QString TrackingModel::dateString(const qint64 timestamp) const
{
    return QDateTime::fromSecsSinceEpoch(timestamp).toString(QStringLiteral("dd/MM/yyyy"));
}
