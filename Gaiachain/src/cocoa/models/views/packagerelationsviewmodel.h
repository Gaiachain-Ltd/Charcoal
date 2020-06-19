#ifndef PACKAGERELATIONSVIEWMODEL_H
#define PACKAGERELATIONSVIEWMODEL_H

#include "../relationmodel.h"
#include "abstractmodelview.h"

class PackageRelationsViewModel final : public RelationModel, public AbstractModelView
{
    Q_OBJECT
    Q_PROPERTY(QString packageId READ packageId WRITE setPackageId NOTIFY packageIdChanged)

public:
    PackageRelationsViewModel(QObject *parent = nullptr);

    void setSourceModel(SqlQueryModel *sourceModel) override;

    QString packageId() const;

    QStringList relatedPackages() const;

public slots:
    void setPackageId(const QString &packageId);

signals:
    void packageIdChanged(const QString &packageId) const;

private:
    QString m_packageId;

    int idShift(const RelationModel::Columns &role) const;

    void updateFilterQuery() override;

    using RelationModel::setSourceModel;
};

#endif // PACKAGERELATIONSVIEWMODEL_H
