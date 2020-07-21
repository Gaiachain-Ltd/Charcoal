#pragma once

#include "simplelistquerymodel.h"

class UnusedPlotIdsModel : public SimpleListQueryModel
{
    Q_OBJECT

public:
    explicit UnusedPlotIdsModel(QObject *parent = nullptr);
};

