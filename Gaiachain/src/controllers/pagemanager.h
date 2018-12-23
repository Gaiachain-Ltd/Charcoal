#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QObject>

#include "../common/enums.h"

class QQmlApplicationEngine;

class PageManager : public QObject
{
    Q_OBJECT
public:
    explicit PageManager(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine);

    Q_INVOKABLE void enterPage(Enums::Page page);

    Q_INVOKABLE void popPage();
    Q_INVOKABLE void goToInitialPage();

signals:
    void push(const QString &url);
    void pop();
    void goToInitial();

private:
    QString m_pagePrefix = "qrc:/pages/";
};

#endif // PAGEMANAGER_H
