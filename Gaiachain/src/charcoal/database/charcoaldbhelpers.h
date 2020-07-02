#pragma once

#include "common/enums.h"

class QString;

class CharcoalDbHelpers
{
public:
    static QString propertiesToString(const QVariantMap &properties);
    static QString getPlotName(const QString &packageName);
    static QString getHarvestName(const QString &packageName);
    static QString actionAbbreviation(const Enums::SupplyChainAction action);
    static Enums::SupplyChainAction actionById(const QString &connectionName, const int id);
    static int bagCountInTransport(const QString &connectionName, const int id);
    static QVariantList defaultOvenDimensions(const QString &connectionName,
                                              const int ovenType);

    static int getWebPackageId(const QString &connectionName, const int entityId);
    static int getVillageId(const QString &connectionName, const QString &name);
    static int getDestinationId(const QString &connectionName, const QString &name);
    static int getParcelId(const QString &connectionName, const QString &parcel);
    static int getTreeSpeciesId(const QString &connectionName, const QString &species);
    static int getOvenId(const QString &connectionName, const int plotId,
                         const QString &ovenName, const bool verbose = true);
    static int getOvenTypeId(const QString &connectionName, const QString &ovenType);
    static int getOvenTypeIdFromName(const QString &connectionName, const QString &name);

    static int getEntityIdFromWebId(const QString &connectionName, const int webId,
                                    const bool verbose = true);
    static int getEntityIdFromName(const QString &connectionName, const QString &name,
                                   const bool verbose = true);

    static int getEntityTypeId(const QString &connectionName, const Enums::PackageType type);

    static int getEventTypeId(const QString &connectionName,
                              const Enums::SupplyChainAction action);

    static int getEventIdFromWebId(const QString &connectionName, const int webId,
                                   const bool verbose = true);

    static int getEventId(const QString &connectionName, const int entityId,
                          const int eventTypeId, qint64 timestamp,
                          const bool verbose = true);

    static int getEventTypeId(const QString &connectionName, const QString &action);

    static int getSimpleInteger(const QString &connectionName, const QString &table,
                                const QString &matchColumn, const QVariant &matchValue,
                                const QString &returnColumn, const bool verbose = true);

    static int getInteger(const QString &connectionName, const QString &table,
                          const QStringList &matchColumns, const QVariantList &matchValues,
                          const QString &returnColumn, const bool verbose = true);

    static QString getParcelCode(const QString &connectionName, const int id);
    static QString getVillageName(const QString &connectionName, const int id);
    static QString getTreeSpeciesName(const QString &connectionName, const int id);
    static QString getOvenLetter(const QString &connectionName, const int ovenId);
    static QString getEventType(const QString &connectionName, const int typeId);
    static QString getEntityName(const QString &connectionName, const int entityId);
    static Enums::PackageType getEntityType(const QString &connectionName,
                                            const int typeId);

    static QString getSimpleString(const QString &connectionName, const QString &table,
                                   const QString &matchColumn, const QVariant &matchValue,
                                   const QString &returnColumn, const bool verbose = true);

    static constexpr char sep = '/';
    static constexpr int metalOvenType = 2;
};

