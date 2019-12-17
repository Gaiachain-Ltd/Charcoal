#ifndef COOPERATIVEVIEWMODEL_H
#define COOPERATIVEVIEWMODEL_H

#include "../eventmodel.h"
#include "abstractmodelview.h"

#include "../../common/enums.h"

class CooperativeViewModel : public EventModel, public AbstractModelView
{
    Q_OBJECT
    Q_PROPERTY(quint32 cooperativeId READ cooperativeId WRITE setCooperativeId NOTIFY cooperativeIdChanged)
    Q_PROPERTY(bool cooperativeOnly READ cooperativeOnly WRITE setCooperativeOnly NOTIFY cooperativeOnlyChanged)

public:
    CooperativeViewModel(QObject *parent = nullptr);

    void setSourceModel(SqlQueryModel *sourceModel) override;

    Q_INVOKABLE quint32 cooperativeId() const;
    Q_INVOKABLE bool cooperativeOnly() const;

public slots:
    void setCooperativeId(quint32 cooperativeId);
    void setCooperativeOnly(bool cooperativeOnly);

signals:
    void cooperativeIdChanged(quint32 cooperativeId) const;
    void cooperativeOnlyChanged(bool cooperativeOnly) const;

protected:
    quint32 m_cooperativeId = 0;
    bool m_cooperativeOnly = true;

    void updateFilterQuery() override;

    using EventModel::setSourceModel;
};

#endif // COOPERATIVEVIEWMODEL_H
