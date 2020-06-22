#pragma once

#include "querymodel.h"

class LocalEventsModel : public QueryModel
{
    Q_OBJECT

public:
    explicit LocalEventsModel(QObject *parent = nullptr);
};

