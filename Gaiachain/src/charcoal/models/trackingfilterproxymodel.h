#pragma once

#include "common/enums.h"

#include <QSortFilterProxyModel>

class TrackingFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(QString searchString READ searchString WRITE setSearchString NOTIFY searchStringChanged)
    Q_PROPERTY(int packageTypes READ packageTypes WRITE setPackageTypes NOTIFY packageTypesChanged)

public:
    explicit TrackingFilterProxyModel(QObject *parent = nullptr);

    QString searchString() const;

    int packageTypes() const;
    Enums::PackageTypes currentPackageTypes() const;

public slots:
    void setSearchString(const QString &searchString);
    void setPackageTypes(const int packageTypes);

signals:
    void searchStringChanged(const QString &searchString) const;
    void packageTypesChanged(const int packageTypes) const;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QString m_searchString;
    int m_packageTypes = Enums::PackageType::Plot
        | Enums::PackageType::Harvest
        | Enums::PackageType::Transport;
};

