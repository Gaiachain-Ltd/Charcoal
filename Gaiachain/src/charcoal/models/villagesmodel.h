#pragma once

#include "simplelistquerymodel.h"

class VillagesModel : public SimpleListQueryModel
{
    Q_OBJECT

public:
    explicit VillagesModel(QObject *parent = nullptr);

public slots:
    void refreshWebData() override;

protected slots:
    void webReplyHandler(const QJsonDocument &reply) override;
};
