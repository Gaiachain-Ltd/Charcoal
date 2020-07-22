#pragma once

#include "simplelistquerymodel.h"

class ParcelsModel : public SimpleListQueryModel
{
    Q_OBJECT

public:
    explicit ParcelsModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;

public slots:
    void refreshWebData() override;
    void getUnusedParcels();

protected slots:
    void webReplyHandler(const QJsonDocument &reply) override;
    void webUnusedParcelsReplyHandler(const QJsonDocument &reply);
};

