#pragma once

#include "querymodel.h"

class TreeSpeciesModel : public QueryModel
{
    Q_OBJECT

public:
    explicit TreeSpeciesModel(QObject *parent = nullptr);

public slots:
    void refreshWebData() override;

protected slots:
    void webReplyHandler(const QJsonDocument &reply) override;
};
