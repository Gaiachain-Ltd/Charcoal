#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QObject>

#include "../models/pagesectionsmodel.h"

#include "../common/enums.h"

class QQmlApplicationEngine;

class PageManager : public QObject
{
    Q_OBJECT
public:
    explicit PageManager(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine);

    Q_INVOKABLE void enterPage(Enums::Page page, const QVariant &properites);
    Q_INVOKABLE void popPage();
    Q_INVOKABLE void goToInitialPage();

    Q_INVOKABLE QString getInitialPageUrl() const;

signals:
    void push(const QString &url, const QVariant &properites) const;
    void pop() const;
    void goToInitial() const;

private:
    const QString m_pagePrefix = "qrc:/pages/";
    const Enums::Page m_initialPage = Enums::Page::ViewType;

    PageSectionsModel m_pageSectionsModel;

    QString pageToQString(Enums::Page p) const;
};

#endif // PAGEMANAGER_H
