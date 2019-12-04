#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include "abstractreadmodel.h"

#include <QPointer>
#include <QSqlTableModel>

class AbstractModel : public AbstractReadModel
{
    Q_OBJECT
public:
    explicit AbstractModel(QObject *parent = nullptr);

    virtual void setSourceModel(QSqlTableModel *sourceModel);
    void setSourceModel(QSqlQueryModel *sourceModel) override;

    virtual QList<int> editableRoles() const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void appendData(const Gaia::ModelData &modelData);
    void updateData(const Gaia::ModelEntry &searchEntryData, const Gaia::ModelEntryInfo &updateEntryData);
    void removeData(const Gaia::ModelEntry &entryData);

signals:
    void entryInserted(const Gaia::ModelEntry &entryData) const;
    void entryRemoved(const Gaia::ModelEntry &entryData) const;

protected:
    QPointer<QSqlTableModel> m_writableModel;

    void handleRowsInserted(const QModelIndex &parent, int first, int last);
    void handleRowsToBeRemoved(const QModelIndex &parent, int first, int last);
};

#endif // ABSTRACTMODEL_H
