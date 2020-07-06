#pragma once

#include "querymodel.h"

class DestinationsModel : public QueryModel
{
    Q_OBJECT

public:
    explicit DestinationsModel(QObject *parent = nullptr);

public slots:
    void refreshWebData() override;

protected slots:
    void webReplyHandler(const QJsonDocument &reply) override;
};

