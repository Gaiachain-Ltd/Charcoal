#pragma once

#include "simplelistquerymodel.h"

class DestinationsModel : public SimpleListQueryModel
{
    Q_OBJECT

public:
    explicit DestinationsModel(QObject *parent = nullptr);

public slots:
    void refreshWebData() override;

protected slots:
    void webReplyHandler(const QJsonDocument &reply) override;
};

