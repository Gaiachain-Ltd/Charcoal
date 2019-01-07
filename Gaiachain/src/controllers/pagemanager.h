#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QVector>
#include <QJsonObject>

#include "abstractmanager.h"
#include "../models/pagesectionsmodel.h"
#include "../common/enums.h"


class PageManager : public AbstractManager
{
    Q_OBJECT
public:
    explicit PageManager(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine);

    Q_INVOKABLE QString getInitialPageUrl() const;

signals:
    void push(const Enums::Page page, const QJsonObject properites = QJsonObject(), const bool immediate = false) const;
    void pop() const;
    void back() const; // it's same as pop()
    void backTo(const Enums::Page backPage) const;
    void backToSection(const Enums::PageSections section) const;
    void goToInitial(bool immediate = false) const;

signals:
    // Signals below should only be used by StackView!!!
    void stackViewPush(const QString &url, const QJsonObject properites = QJsonObject(), const bool immediate = false) const;
    void stackViewPop() const;
    void stackViewBackToInitial(const bool immediate = false) const;
    void stackViewBackToPage(const Enums::Page backPage) const;

private slots:
    void enterPage(const Enums::Page page, QJsonObject properites = QJsonObject(), const bool immediate = false);
    void popPage();
    bool backToPage(const Enums::Page backPage);
    bool backToFirstSectionPage(const Enums::PageSections section);
    void goToInitialPage(const bool immediate = false);

private:
    const QString m_pagePrefix = QStringLiteral("qrc:/pages/");
    const Enums::Page m_initialPage = Enums::Page::ViewType;

    QVector<Enums::Page> m_pageStack;
    PageSectionsModel m_pageSectionsModel;

    void prepareConnections();
    QString pageToQString(const Enums::Page p) const;
};

#endif // PAGEMANAGER_H
