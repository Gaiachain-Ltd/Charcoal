#include "trackingmodel.h"

#include "helpers/requestshelper.h"
#include "database/dbhelpers.h"
#include "rest/baserequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"
#include "common/logs.h"
#include "common/tags.h"
#include "common/enums.h"
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
    m_updateTimer.setInterval(200);
    m_updateTimer.setTimerType(Qt::TimerType::CoarseTimer);
    m_updateTimer.setSingleShot(false);

    setWebModelCanChange(true);
    setDbQuery("SELECT id, typeId, name, parent, webId FROM Entities "
               "ORDER BY webId DESC");
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
        return query().value(Tags::id).toString();
    case TrackingRole::Name:
    case Qt::ItemDataRole::DisplayRole:
        return query().value(Tags::name).toString();
    case TrackingRole::Type:
    {
        const int id(query().value(Tags::typeId).toInt());
        return int(CharcoalDbHelpers::getEntityType(m_connectionName, id));
    }
    case TrackingRole::Events:
    {
        Entity entity;
        entity.id = query().value(Tags::id).toInt();
        entity.parent = query().value(Tags::parent).toInt();
        entity.typeId = query().value(Tags::typeId).toInt();
        entity.name = query().value(Tags::name).toString();
        const QVector<Event> events = entity.loadEvents(m_connectionName);

        QVariantList result;

        for (const Event &event : events) {
            const auto action = CharcoalDbHelpers::actionById(
                m_connectionName, event.typeId);

            QString name;
            switch (action) {
            case Enums::SupplyChainAction::LoggingBeginning:
                name = tr("Logging has begun");
                break;
            case Enums::SupplyChainAction::LoggingEnding:
                name = tr("Logging has ended");
                break;
            case Enums::SupplyChainAction::CarbonizationBeginning:
            {
                const Oven oven = event.loadOven(m_connectionName);
                name = tr("Oven %1 - carbonization has begun").arg(oven.name);
            }
            break;
            case Enums::SupplyChainAction::CarbonizationEnding:
            {
                const Oven oven = event.loadOven(m_connectionName);
                name = tr("Oven %1 - carbonization has ended").arg(oven.name);
            }
            break;
            case Enums::SupplyChainAction::LoadingAndTransport:
            {
                name = tr("Bags have been loaded on truck %1")
                           .arg(event.properties.value(Tags::webPlateNumber).toString());
            }
            break;
            case Enums::SupplyChainAction::LocalReception:
                name = tr("Bags sold at a local market");
                break;
            case Enums::SupplyChainAction::Reception:
                name = tr("Reception at storage facility");
                break;
            default: name = QString();
            }

            const QVariantMap row {
                { "eventName", name },
                { Tags::date, dateString(event.date) }
            };

            result.append(row);
        }

        return result;
    }
    case TrackingRole::EventSummary:
    {
        /*
         * This scary code looks heavy (and it is) - but it is called only
         * when user clicks on a delegate on Tracking page.
         */

        Entity entity;
        entity.id = query().value(Tags::id).toInt();
        entity.parent = query().value(Tags::parent).toInt();
        entity.typeId = query().value(Tags::typeId).toInt();
        entity.name = query().value(Tags::name).toString();

        const Enums::PackageType entityType = CharcoalDbHelpers::getEntityType(
            m_connectionName, entity.typeId);

        const QVector<Event> events = entity.loadEvents(m_connectionName);

        for (const auto &event : events) {
            qDebug() << event.properties;
        }

        QVariantList result;
        switch (entityType) {
        case Enums::PackageType::Plot:
            result = summaryForPlot(entity, events);
            break;
        case Enums::PackageType::Harvest:
            result = summaryForHarvest(entity, events);
            break;
        case Enums::PackageType::Transport:
            result = summaryForTransport(entity, events);
            break;
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

QVariantList TrackingModel::summaryForPlot(
    const Entity entity,
    const QVector<Event> &events) const
{
    const auto &utility = Utility::instance();

    QVariantList result;

    int parcelId = -1;
    int villageId = -1;
    int treeSpeciesId = -1;
    int numberOfTrees = -1;
    qint64 beginningTimestamp = -1;
    qint64 endingTimestamp = -1;

    for (const Event &event : events) {
        const Enums::SupplyChainAction action = CharcoalDbHelpers::actionById(
            m_connectionName, event.typeId);

        if (action == Enums::SupplyChainAction::LoggingBeginning
            && parcelId == -1) {
            parcelId = event.properties.value(Tags::webParcel).toInt(-1);
            villageId = event.properties.value(Tags::webVillage).toInt(-1);
            treeSpeciesId = event.properties.value(Tags::webTreeSpecies).toInt(-1);
            beginningTimestamp = event.date;
        }

        if (action == Enums::SupplyChainAction::LoggingEnding
            && endingTimestamp == -1) {
            endingTimestamp = event.date;
            numberOfTrees = event.properties.value(Tags::webNumberOfTrees).toInt(-1);
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

    result.append(utility.createSummaryItem(
        tr("Plot ID"), entity.name, QString(), QString(),
        m_plotHighlightColor, m_plotTextColor, m_plotTextColor,
        Enums::DelegateType::Standard,
        true));
    if (events.isEmpty() == false) {
        result.append(utility.createSummaryItem(
            tr("Malebi Rep's ID"), events.at(0).userId));
    }
    result.append(utility.createSummaryItem(
        tr("Parcel"), parcel));
    result.append(utility.createSummaryItem(
        tr("Village"), village));
    result.append(utility.createSummaryItem(
        QString(),
        QVariantList {
            QVariantList {
                tr("Beginning date"),
                tr("Ending date")
            },
            QVariantList {
                dateString(beginningTimestamp, true),
                dateString(endingTimestamp, true)
            }
        },
        QString(), QString(), QString(), QString(), QString(),
        Enums::DelegateType::BeginEndRow
        ));
    result.append(utility.createSummaryItem(
        tr("Tree species"), treeSpecies));
    result.append(utility.createSummaryItem(
        tr("Number of trees"), numberOfTrees));
    return result;
}

QVariantList TrackingModel::summaryForHarvest(
    const Entity entity,
    const QVector<Event> &events) const
{
    const auto &utility = Utility::instance();

    QVariantList result;

    const QString plotName(CharcoalDbHelpers::getPlotName(entity.name));

    Utility::SummaryValue plotValue;
    {
        plotValue.titles.append(tr("Plot ID"));
        plotValue.values.append(plotName);

        Entity plotEntity;
        if (plotEntity.loadFromDb(m_connectionName, entity.parent)) {
            const QVector<Event> plotEvents(plotEntity.loadEvents(m_connectionName));
            plotValue.icons.append("qrc:/ui/loupe");
            plotValue.linkDestinationPages.append(Enums::Page::SupplyChainSummary);
            plotValue.linkDatas.append(QVariantMap{
                {"title", tr("Plot ID details")},
                {"proceedButtonVisible", false},
                {"summary", summaryForPlot(plotEntity, plotEvents)}
            });
        }
    }

    result.append(utility.createSummaryItem(
        tr("Harvest ID"), entity.name, QString(), QString(),
        m_harvestHighlightColor, m_harvestTextColor, m_harvestTextColor,
        Enums::DelegateType::Standard,
        true));
    result.append(utility.createSummaryItem(
        QString(),
        plotValue.toList(),
        QString(), QString(),
        m_harvestHighlightColor, m_harvestTextColor, m_plotTextColor,
        Enums::DelegateType::ColumnStack,
        true));

    // QMap because we want it to be *sorted*
    QMap<QString, Oven> ovens;
    for (const auto &event : events) {
        const Oven oven = event.loadOven(m_connectionName);
        if (ovens.contains(oven.name)) {
            Oven existing = ovens.value(oven.name);
            existing.updateDates(oven, event);
            ovens.insert(oven.name, existing);
        } else {
            ovens.insert(oven.name, oven);
        }
    }

    for (const auto &ovenName : ovens.keys()) {
        const Oven oven = ovens.value(ovenName);

        QVariantList ovenHeaders = {
            tr("Width"),
            tr("Length"),
        };

        QString unit(" m");
        QVariantList ovenValues = {
            QString(QString::number(oven.dimensions.width) + unit),
            QString(QString::number(oven.dimensions.length) + unit),
            QString(QString::number(oven.dimensions.height1) + unit),
            QString(QString::number(oven.dimensions.height2) + unit)
        };

        if (oven.dimensions.count() == 3) {
            ovenHeaders.append(tr("Height"));
            ovenValues.removeLast();
        } else {
            ovenHeaders.append(tr("Height A"));
            ovenHeaders.append(tr("Height B"));
        }

        QVariantList ovenSummary;
        ovenSummary.append(utility.createSummaryItem(
            QString(),
            QVariantList {
                QVariantList {
                    tr("Beginning date"),
                    tr("Ending date")
                },
                QVariantList {
                    dateString(oven.carbonizationBeginning, true),
                    dateString(oven.carbonizationEnding, true)
                }
            },
            QString(), QString(), QString(), QString(), QString(),
            Enums::DelegateType::BeginEndRow
            ));
        ovenSummary.append(utility.createSummaryItem(
            tr("Carbonizer ID"), oven.carbonizerId));
        ovenSummary.append(utility.createSummaryItem(
            tr("Oven measurement (meters)"),
            QVariantList {
                ovenHeaders,
                ovenValues
            },
            QString(), QString(), QString(), QString(), QString(),
            Enums::DelegateType::Row));
        ovenSummary.append(utility.createSummaryItem(
            tr("Timber volume"), QString("%1 m³").arg(oven.dimensions.volume())
            ));

        result.append(utility.createSummaryItem(
            tr("Oven %1").arg(oven.name), ovenSummary,
            QString(), QString(), QColor(), QColor(), QColor(),
            Enums::DelegateType::Collapsible
            ));
    }

    return result;
}

QVariantList TrackingModel::summaryForTransport(
    const Entity entity,
    const QVector<Event> &events) const
{
    const auto &utility = Utility::instance();

    QVariantList result;

    qint64 beginningTimestamp = -1;
    qint64 endingTimestamp = -1;
    QString plateNumber;

    result.append(utility.createSummaryItem(
        tr("Transport ID"), entity.name, QString(), QString(),
        m_transportHighlightColor, m_transportTextColor, m_transportTextColor,
        Enums::DelegateType::Standard,
        true));

    QVector<Event> eventsAscending = events;
    std::reverse(eventsAscending.begin(), eventsAscending.end());

    for (const Event &event : qAsConst(eventsAscending)) {
        const Enums::SupplyChainAction action = CharcoalDbHelpers::actionById(
            m_connectionName, event.typeId);

        Utility::SummaryValue value;
        const QString bagNumberString(QString::number(event.qrCodes().size()));

        if (action == Enums::SupplyChainAction::LoadingAndTransport
            && beginningTimestamp == -1) {
            plateNumber = event.properties.value(Tags::webPlateNumber).toString();
            beginningTimestamp = event.date;

            value.titles.append({
                dateString(beginningTimestamp, true),
                bagNumberString
            });

            value.values.append({
                tr("Loading date"),
                tr("Number of loaded bags")
            });

            value.icons.append({
                QString(),
                QString()
            });

            value.linkDestinationPages.append({
                Enums::Page::InvalidPage,
                Enums::Page::InvalidPage
            });

            value.linkDatas.append({
                QVariant(),
                QVariant()
            });
        }

        if (action == Enums::SupplyChainAction::LocalReception) {
            value.titles.append({
                bagNumberString,
                dateString(event.date, true)
            });

            value.values.append({
                tr("Number of bags sold"),
                tr("Selling date")
            });

            value.icons.append({
                QString(),
                QString()
            });

            value.linkDestinationPages.append({
                Enums::Page::InvalidPage,
                Enums::Page::InvalidPage
            });

            value.linkDatas.append({
                QVariant(),
                QVariant()
            });
        }

        if (action == Enums::SupplyChainAction::Reception
            && endingTimestamp == -1) {
            endingTimestamp = event.date;

            QStringList docs;
            QStringList recs;

            const QString cache(m_picturesManager->cachePath());

            const QJsonArray docsArray(event.properties.value(Tags::webDocuments).toArray());
            for (const auto &value : docsArray) {
                docs.append(cache + "/" + value.toString());
            }

            const QJsonArray recsArray(event.properties.value(Tags::webReceipts).toArray());
            for (const auto &value : recsArray) {
                recs.append(cache + "/" + value.toString());
            }

            const bool hasDocs = (docs.isEmpty() == false);
            const bool hasRecs = (recs.isEmpty() == false);

            m_picturesManager->setCurrentDocuments(docs);
            m_picturesManager->setCurrentReceipts(recs);

            const QString uploaded(tr("Uploaded"));
            const QString noPhoto(tr("No photo"));

            value.titles.append({
                bagNumberString,
                dateString(endingTimestamp, true)
            });

            value.values.append({
                tr("Number of received bags"),
                tr("Final reception date")
            });

            value.icons.append({
                QString(),
                QString()
            });

            value.linkDestinationPages.append({
                Enums::Page::InvalidPage,
                Enums::Page::InvalidPage
            });

            value.linkDatas.append({
                QVariant(),
                QVariant()
            });

            result.append(utility.createSummaryItem(
                QString(), value.toList(),
                QString(), QString(),
                m_transportHighlightColor, m_transportTextColor, QColor("#000000"),
                Enums::DelegateType::ColumnStack,
                true));

            value = Utility::SummaryValue();

            value.titles.append({
                QString(hasDocs? uploaded : noPhoto),
                QString(hasRecs? uploaded : noPhoto)
            });

            value.values.append({
                tr("Documents"),
                tr("Receipts")
            });

            value.icons.append({
                "image://tickmark/document-" + QString(hasDocs? "true" : "false"),
                "image://tickmark/receipt-" + QString(hasRecs? "true" : "false")
            });

            value.linkDestinationPages.append({
                (hasDocs? Enums::Page::PhotoGallery : Enums::Page::InvalidPage),
                (hasRecs? Enums::Page::PhotoGallery : Enums::Page::InvalidPage)
            });

            value.linkDatas.append({
                QVariantMap{ {"urls", docs} },
                QVariantMap{ {"urls", recs} }
            });
        }

        result.append(utility.createSummaryItem(
            QString(), value.toList(),
            QString(), QString(),
            m_transportHighlightColor, m_transportTextColor, QColor("#000000"),
            Enums::DelegateType::ColumnStack,
            true));
    }

    result.append(utility.createSummaryItem(
        tr("Plate number"), plateNumber));
    result.append(utility.createSummaryItem(
        tr("Reception at the storage facility"),
        dateString(endingTimestamp, true)));

    return result;
}

void TrackingModel::refreshWebData()
{
    // TODO: limit amount of data with canFetchMore() etc.
    const auto request = QSharedPointer<BaseRequest>::create(
        "/entities/packages/",
        BaseRequest::Type::Get);

    if (RequestsHelper::isOnline(m_sessionManager.get(), m_userManager.get())) {
        request->setToken(m_sessionManager->token());
        m_sessionManager->sendRequest(request, this,
                                      &TrackingModel::webErrorHandler,
                                      &TrackingModel::webReplyHandler);
    } else {
        qDebug() << "Enqueing request";
        m_queuedRequests.append(request);
    }

    m_isPending = true;
}

void TrackingModel::webReplyHandler(const QJsonDocument &reply)
{
    m_isPending = false;
    //qDebug() << "Data is:" << reply;
    TrackingUpdater updater(m_connectionName);
    const auto result = updater.updateTable(reply);
    if (result.success) {
        getMissingPictures();

        if (result.toFinish.isEmpty() == false) {
            emit finalizePackages(result.toFinish);
        }

        startPackageDetailsUpdate();
    } else {
        emit error(tr("Error updating tracking information"));
    }
}

void TrackingModel::detailsReplyHandler(const QJsonDocument &reply)
{
    //qDebug() << "Update data is:" << reply;
    TrackingUpdater updater(m_connectionName);
    updater.setPicturesManager(m_picturesManager);
    if (updater.updateDetails(reply)) {
        emit webDataRefreshed();
    } else {
        emit error(tr("Error updating detailed information"));
    }
}

QString TrackingModel::dateString(const qint64 timestamp, const bool withTime) const
{
    const auto dateTime = QDateTime::fromSecsSinceEpoch(timestamp);

    if (dateTime < QDate(1980, 1, 1).startOfDay()) {
        return QString(Tags::noDateTime);
    }

    if (withTime) {
        return dateTime.toString(QStringLiteral("dd/MM/yyyy hh:mm:ss"));
    } else {
        return dateTime.toString(QStringLiteral("dd/MM/yyyy"));
    }
}

/*!
 * Begins updating information about all packages handled by TrackingModel.
 */
void TrackingModel::startPackageDetailsUpdate()
{
    QSqlQuery q(QString(), db::Helpers::databaseConnection(m_connectionName));
    q.prepare("SELECT DISTINCT parentWebId FROM Events "
              "WHERE properties IS NULL");

    if (q.exec()) {
        // Populate list first, then send events: to prevent situation where
        // running query is interrupted byt incoming replies from server
        QVector<int> webIds;
        while (q.next()) {
            webIds.append(q.value("parentWebId").toInt());
        }

        qDebug() << "Parent Web IDs are" << webIds;

        for (const int webId : qAsConst(webIds)) {
            const QString url(QString("/entities/packages/%1/get_package_details/")
                                  .arg(webId));

            // Check if such request was already queued
            for (const auto &request : qAsConst(m_queuedRequests)) {
                if (request->type() != BaseRequest::Type::Get) {
                    continue;
                }

                if (url == request->path()) {
                    continue;
                }
            }

            // TODO: delay!

            const auto request = QSharedPointer<BaseRequest>::create(
                url, BaseRequest::Type::Get);

            if (RequestsHelper::isOnline(m_sessionManager.get(), m_userManager.get())) {
                request->setToken(m_sessionManager->token());
                m_sessionManager->sendRequest(request, this,
                                              &TrackingModel::webErrorHandler,
                                              &TrackingModel::detailsReplyHandler);
            } else {
                qDebug() << "Enqueing request";
                m_queuedRequests.append(request);
            }
        }
    } else {
        qDebug() << "No event requires updating details";
        emit webDataRefreshed();
    }
}

void TrackingModel::getMissingPictures()
{
    const int typeId = CharcoalDbHelpers::getEventTypeId(
        m_connectionName, Enums::SupplyChainAction::Reception);

    QSqlQuery q(QString(), db::Helpers::databaseConnection(m_connectionName));
    q.prepare("SELECT properties FROM Events "
              "WHERE typeId=:typeId");
    q.bindValue(":typeId", typeId);

    if (q.exec() == false) {
        qWarning() << RED("Could not load event properties from DB")
                   << "SQL error:" << q.lastError()
                   << "For query:" << q.lastQuery()
                   << typeId;
    }

    while (q.next()) {
        const QByteArray propertiesString(q.value(Tags::properties).toByteArray());
        const QJsonObject properties(
            CharcoalDbHelpers::dbPropertiesToJson(propertiesString));
        const auto docs(properties.value(Tags::webDocuments).toArray());
        const auto recs(properties.value(Tags::webReceipts).toArray());

        for (const auto &doc : docs) {
            m_picturesManager->checkFileIsCached(doc.toString());
        }

        for (const auto &rec : recs) {
            m_picturesManager->checkFileIsCached(rec.toString());
        }
    }
}
