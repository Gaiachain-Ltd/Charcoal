#ifndef PARCELVIEWMODEL_H
#define PARCELVIEWMODEL_H

#include "../parcelmodel.h"
#include "abstractmodelview.h"

class ParcelViewModel : public ParcelModel, public AbstractModelView
{
    Q_OBJECT
    Q_PROPERTY(quint32 producerId READ producerId WRITE setProducerId NOTIFY producerIdChanged)
    Q_PROPERTY(bool allProducers READ allProducers WRITE setAllProducers NOTIFY allProducersChanged)

public:
    ParcelViewModel(QObject *parent = nullptr);

    quint32 producerId() const;    
    bool allProducers() const;

    void setSourceModel(SqlQueryModel *sourceModel) override;

public slots:
    void setProducerId(quint32 producerId);
    void setAllProducers(bool allProducers);

signals:
    void producerIdChanged(quint32 producerId);
    void allProducersChanged(bool allProducers);

private:
    quint32 m_producerId = 0;
    bool m_allProducers = false;

    void updateFilterQuery() override;

    using ParcelModel::setSourceModel;
};

#endif // PARCELVIEWMODEL_H
