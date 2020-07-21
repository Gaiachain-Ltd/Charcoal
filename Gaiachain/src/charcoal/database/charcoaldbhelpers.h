#pragma once

#include "common/enums.h"

#include <QHash>
#include <QString>
#include <QJsonObject>

namespace db {
enum QueryFlag {
    Silent = 0x0,
    Verbose = 0x1,
    MatchFirst = 0x2,
    MatchLast = 0x4
};
Q_DECLARE_FLAGS(QueryFlags, QueryFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(QueryFlags)
}

struct ContinueEvent {
    int entityId = -1;
    int eventId = -1;
};

class CharcoalDbHelpers
{
public:
    static QString propertiesToString(const QVariantMap &properties);
    static QString getPlotName(const QString &packageName);
    static QString getHarvestName(const QString &packageName);
    static QString actionAbbreviation(const Enums::SupplyChainAction action);
    static Enums::SupplyChainAction actionById(const QString &connectionName,
                                               const int id);
    static Enums::SupplyChainAction actionByName(const QString &actionName);
    static int bagCountInTransport(const QString &connectionName, const int id);
    static QVariantList defaultOvenDimensions(const QString &connectionName,
                                              const int ovenType);

    static QJsonObject dbPropertiesToJson(const QString &properties);

    static int getWebPackageId(const QString &connectionName, const int entityId);
    static QVector<int> getWebPackageIds(const QString &connectionName,
                                         const QString &plotName,
                                         const int parentId);
    static int getVillageId(const QString &connectionName, const QString &name);
    static int getDestinationId(const QString &connectionName, const QString &name);
    static int getParcelId(const QString &connectionName, const QString &parcel);
    static int getTreeSpeciesId(const QString &connectionName, const QString &species);
    static int getOvenId(const QString &connectionName, const int plotId,
                         const QString &ovenName, const bool verbose = true);
    static int getOvenTypeId(const QString &connectionName, const QString &ovenType);
    static int getOvenTypeIdFromName(const QString &connectionName, const QString &name);

    static int getEntityIdFromWebId(const QString &connectionName, const int webId,
                                    const db::QueryFlags settings = db::QueryFlag::Verbose);
    static int getEntityIdFromName(const QString &connectionName, const QString &name,
                                   const db::QueryFlags settings = db::QueryFlag::Verbose);

    static int getEntityTypeId(const QString &connectionName, const Enums::PackageType type);

    static int getEventTypeId(const QString &connectionName,
                              const Enums::SupplyChainAction action);

    static Enums::SupplyChainAction getEventTypeFromEventId(const QString &connectionName,
                                                            const int eventId);

    static int getEventIdFromWebId(const QString &connectionName, const int webId,
                                   const db::QueryFlags settings = db::QueryFlag::Verbose);

    static int getEventId(const QString &connectionName, const int entityId,
                          const int eventTypeId, qint64 timestamp,
                          const bool verbose = true);

    static int getEventId(const QString &connectionName, qint64 timestamp,
                          const db::QueryFlags settings = db::QueryFlag::Verbose);

    static ContinueEvent getContinueEvent(const QString &connectionName,
                                          const int eventTypeId);

    static int getEventTypeId(const QString &connectionName, const QString &action);

    static int getSimpleInteger(const QString &connectionName, const QString &table,
                                const QString &matchColumn, const QVariant &matchValue,
                                const QString &returnColumn,
                                const db::QueryFlags settings = db::QueryFlag::Verbose);

    static int getInteger(const QString &connectionName, const QString &table,
                          const QStringList &matchColumns, const QVariantList &matchValues,
                          const QString &returnColumn, const QString &extra = QString(),
                          const bool verbose = true);

    static QString getParcelCode(const QString &connectionName, const int id);
    static QString getVillageName(const QString &connectionName, const int id);
    static QString getTreeSpeciesName(const QString &connectionName, const int id);
    static QString getDestinationName(const QString &connectionName, const int id);
    static QString getOvenLetter(const QString &connectionName, const int ovenId);
    static QString getEventType(const QString &connectionName, const int typeId);
    static QString getEntityName(const QString &connectionName, const int entityId);
    static Enums::PackageType getEntityType(const QString &connectionName,
                                            const int typeId);

    static QString getSimpleString(const QString &connectionName, const QString &table,
                                   const QString &matchColumn, const QVariant &matchValue,
                                   const QString &returnColumn,
                                   const db::QueryFlags settings = db::QueryFlag::Verbose);

    static constexpr char sep = '/';
    static constexpr int metalOvenType = 2;

private:
    static Enums::SupplyChainAction cacheSupplyAction(const QString &connectionName,
                                                      const int actionId);
    static Enums::SupplyChainAction cacheSupplyAction(const QString &connectionName,
                                                      const Enums::SupplyChainAction action);

    static Enums::PackageType cachePackageType(const QString &connectionName,
                                               const int typeId);
    static Enums::PackageType cachePackageType(const QString &connectionName,
                                               const Enums::PackageType type);

    static const QHash<Enums::SupplyChainAction, QString> m_supplyActionMap;
    static QHash<int, Enums::SupplyChainAction> m_supplyActionDbMap;

    static const QHash<Enums::PackageType, QString> m_packageTypeMap;
    static QHash<int, Enums::PackageType> m_packageTypeDbMap;
};

