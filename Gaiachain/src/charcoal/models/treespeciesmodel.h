#pragma once

#include "simplelistquerymodel.h"

class TreeSpeciesModel : public SimpleListQueryModel
{
    Q_OBJECT

public:
    explicit TreeSpeciesModel(QObject *parent = nullptr);

public slots:
    void refreshWebData() override;

protected slots:
    void webReplyHandler(const QJsonDocument &reply) override;
};
