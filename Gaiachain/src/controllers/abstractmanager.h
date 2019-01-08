#ifndef ABSTRACTMANAGER_H
#define ABSTRACTMANAGER_H

#include <QObject>

class QQmlApplicationEngine;

class AbstractManager : public QObject
{
    Q_OBJECT
public:
    explicit AbstractManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) = 0;
};

#endif // ABSTRACTMANAGER_H
