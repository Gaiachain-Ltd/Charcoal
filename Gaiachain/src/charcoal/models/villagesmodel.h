#pragma once

#include "querymodel.h"

class VillagesModel : public QueryModel
{
    Q_OBJECT

public:
    explicit VillagesModel(QObject *parent = nullptr);

public slots:
    void refreshWebData() override;

protected slots:
    void webReplyHandler(const QJsonDocument &reply) override;
};
