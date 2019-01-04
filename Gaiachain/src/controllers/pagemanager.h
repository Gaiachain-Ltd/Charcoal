#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QObject>
#include <QVector>
#include <QJsonObject>

#include "../models/pagesectionsmodel.h"

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
    Q_INVOKABLE void goToInitialPage(bool immediate = false);
    Q_INVOKABLE bool backTo(Enums::Page backPage);
    Q_INVOKABLE bool backToSection(Enums::PageSections section);

    Q_INVOKABLE QString getInitialPageUrl() const;

signals:
    // DO NOT EMIT THOSE SIGNALS IN QML. USE ABOVE FUNCTIONS!
    void push(const QString &url, const QVariant &properites) const;
    void pop() const;
    void back(); // it's same as pop()
    void goBackToPage(Enums::Page backPage);
    void goToInitial(bool immediate = false) const;

private:
    const QString m_pagePrefix = "qrc:/pages/";
    const Enums::Page m_initialPage = Enums::Page::ViewType;

    QVector<Enums::Page> m_pageStack;
    PageSectionsModel m_pageSectionsModel;

    QString pageToQString(Enums::Page p) const;
};

#endif // PAGEMANAGER_H
