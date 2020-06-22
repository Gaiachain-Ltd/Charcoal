#pragma once

#include "querymodel.h"

class LocalEventsModel : public QueryModel
{
    Q_OBJECT

    Q_PROPERTY(int size READ size NOTIFY sizeChanged)

    int m_size;

public:
    explicit LocalEventsModel(QObject *parent = nullptr);

    Q_INVOKABLE int size();

signals:
    void sizeChanged(const int size) const;

private:
    int m_oldRowCount = 0;
};

