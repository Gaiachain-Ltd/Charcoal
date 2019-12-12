#ifndef ABSTRACTMANAGER_H
#define ABSTRACTMANAGER_H

#include <QObject>
#include <QPointer>
#include <QAtomicInt>

class QQmlApplicationEngine;

class AbstractManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool processing READ processing NOTIFY processingChanged)

public:
    explicit AbstractManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine);

    virtual bool processing() const;

signals:
    void processingChanged(bool processing) const;

protected:
    struct ProcessCounter final {
        ProcessCounter(AbstractManager const *manager);
        ~ProcessCounter();

    private:
        QPointer<const AbstractManager> m_manager;
    };

    void processStarted() const;
    void processFinished() const;

private:
    mutable QAtomicInt m_activeProcessesCount = 0;
};

#endif // ABSTRACTMANAGER_H
