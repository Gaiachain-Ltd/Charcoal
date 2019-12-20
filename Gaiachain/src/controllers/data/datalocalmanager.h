#ifndef DATALOCALMANAGER_H
#define DATALOCALMANAGER_H

#include "../abstractmanager.h"

#include <QDir>
#include <QGeoCoordinate>

#include "../../common/globals.h"
#include "../../common/enums.h"

class DataLocalManager : public AbstractManager
{
    Q_OBJECT
public:
    DataLocalManager(QObject *parent = nullptr);

    void addLocalAction(const QString &packageId, const Enums::SupplyChainAction &action,
                        const QGeoCoordinate &coordinate, const QDateTime &timestamp, const QVariantMap &properties) const;
    void removeLocalAction(const QString &packageId, const Enums::SupplyChainAction &action) const;

    void sendLocalAction(const QString &packageId, const Enums::SupplyChainAction &action,
                         const QGeoCoordinate &coordinate, const QDateTime &timestamp, const QVariantMap &properties) const;

signals:
    void localActionAddRequest(const QString &packageId, const Enums::SupplyChainAction &action,
                               const QGeoCoordinate &coordinate, const QDateTime &timestamp, const QVariantMap &properties) const;
    void localActionDataError(const QString &packageId, const Enums::SupplyChainAction &action) const;

private:
    const QString c_dataPath;
    const QDir c_dataDir;

    QString filePath(const QString &packageId, const Enums::SupplyChainAction &action) const;
};

#endif // DATALOCALMANAGER_H
