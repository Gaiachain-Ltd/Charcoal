#ifndef PACKAGEVIEWMODEL_H
#define PACKAGEVIEWMODEL_H

#include "../eventmodel.h"
#include "abstractmodelview.h"

struct PackageData;

class PackageViewModel final : public EventModel, public AbstractModelView
{
    Q_OBJECT
    Q_PROPERTY(QString packageId READ packageId WRITE setPackageId NOTIFY packageIdChanged)

public:
    PackageViewModel(QObject *parent = nullptr);

    void setSourceModel(SqlQueryModel *sourceModel) override;

    QString packageId() const;

    void fillPackageData(PackageData &packageData) const;

public slots:
    void setPackageId(const QString &packageId);

signals:
    void packageIdChanged(const QString &packageId) const;

private:
    QString m_packageId;

    int idShift(const EventModel::Columns &role) const;

    void updateFilterQuery() override;

    using EventModel::setSourceModel;
};

#endif // PACKAGEVIEWMODEL_H
