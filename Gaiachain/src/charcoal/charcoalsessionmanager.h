#pragma once

#include "controllers/session/restsessionmanager.h"

#include <QObject>

class CharcoalSessionManager : public RestSessionManager
{
    Q_OBJECT

public:
    CharcoalSessionManager(QObject *parent = nullptr);
};
