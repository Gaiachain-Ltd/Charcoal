#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QObject>
#include <QJsonObject>

#include "../common/enums.h"

class QQmlApplicationEngine;

class PageManager : public QObject
{
    Q_OBJECT
public:
    explicit PageManager(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine);

    Q_INVOKABLE void enterPage(Enums::Page page, QJsonObject properites = QJsonObject());
    Q_INVOKABLE void popPage();
    Q_INVOKABLE void goToInitialPage();

signals:
    void push(const QString &url, const QVariant &properites);
    void pop();
    void back(); // it's same as pop()
    void goToInitial();

private:
    QString m_pagePrefix = "qrc:/pages/";
};

#endif // PAGEMANAGER_H
