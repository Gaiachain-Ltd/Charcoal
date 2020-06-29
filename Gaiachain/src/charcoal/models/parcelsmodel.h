#pragma once

#include "querymodel.h"

class ParcelsModel : public QueryModel
{
    Q_OBJECT

public:
    explicit ParcelsModel(QObject *parent = nullptr);

public slots:
    void refreshWebData() override;
    void getUnusedParcels();

protected slots:
    void webReplyHandler(const QJsonDocument &reply) override;
    void webUnusedParcelsReplyHandler(const QJsonDocument &reply);
};

