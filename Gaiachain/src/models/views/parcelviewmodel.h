#ifndef PARCELVIEWMODEL_H
#define PARCELVIEWMODEL_H

#include "../parcelmodel.h"
#include "abstractmodelview.h"

class ParcelViewModel : public ParcelModel, public AbstractModelView
{
    Q_OBJECT
    Q_PROPERTY(quint32 producerId READ producerId WRITE setProducerId NOTIFY producerIdChanged)

public:
    ParcelViewModel(QObject *parent = nullptr);

    quint32 producerId() const;

    void setSourceModel(SqlQueryModel *sourceModel) override;

public slots:
    void setProducerId(quint32 producerId);

signals:
    void producerIdChanged(quint32 producerId);

private:
    quint32 m_producerId = 0;

    void updateFilterQuery() override;

    using ParcelModel::setSourceModel;
};

#endif // PARCELVIEWMODEL_H
