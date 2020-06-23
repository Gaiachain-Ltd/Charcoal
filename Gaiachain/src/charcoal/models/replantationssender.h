#pragma once

#include "querymodel.h"

class ReplantationsSender : public QueryModel
{
    Q_OBJECT

public:
    explicit ReplantationsSender(QObject *parent = nullptr);

public slots:
    void sendEvents();

private:
    QVector<QSharedPointer<BaseRequest>> m_queuedRequests;
};

