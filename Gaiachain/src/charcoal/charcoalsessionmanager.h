#pragma once

#include "controllers/session/restsessionmanager.h"

#include <QObject>

class CharcoalSessionManager : public RestSessionManager
{
    Q_OBJECT

public:
    CharcoalSessionManager(QObject *parent = nullptr);

    Q_INVOKABLE void getDestinations();
    Q_INVOKABLE void getOvenTypes();
    Q_INVOKABLE void getParcels();
    Q_INVOKABLE void getTreeSpecies();
    Q_INVOKABLE void getVillages();
};
