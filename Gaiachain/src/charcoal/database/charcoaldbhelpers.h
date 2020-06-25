#pragma once

class QString;

class CharcoalDbHelpers
{
public:
    // TODO: unify these methods, do not do such massive copy-paste
    // TODO: transfer more generic methods away from ActionController and other
    // model classes
    static int getWebPackageId(const QString &connectionName, const QString &entityId);
    static QString getOvenLetter(const QString &connectionName, const QString &ovenId);
    static int getVillageId(const QString &connectionName, const QString &name);
    static int getDestinationId(const QString &connectionName, const QString &name);
    static int getParcelId(const QString &connectionName, const QString &parcel);

    static int getSimpleInteger(const QString &connectionName, const QString &table,
                                const QString &matchColumn, const QString &matchValue,
                                const QString &returnColumn);
};

