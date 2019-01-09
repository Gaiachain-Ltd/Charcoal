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

    virtual void setupQmlContext(QQmlApplicationEngine &engine) Q_DECL_OVERRIDE;

    Q_INVOKABLE QString getInitialPageUrl() const;
    Q_INVOKABLE Enums::Page homePage() const;
    Q_INVOKABLE bool isOnHomePage() const;

signals:
    void popupAction(Enums::PopupAction action);

signals:
    // Signals below should only be used by StackView!!!
    void stackViewPush(const QString &url, const QJsonObject properites = QJsonObject(), const bool immediate = false) const;
    void stackViewPop(const bool immediate = false) const;
    void stackViewBackToPage(const Enums::Page backPage, const bool immediate = false) const;

public slots:
    // Page managment
    void enter(const Enums::Page page, QJsonObject properites = QJsonObject(), const bool immediate = false);
    void back(const bool immediate = false);
    bool backTo(const Enums::Page backPage, const bool immediate = false);
    void backToAndEnter(const Enums::Page backPage, const Enums::Page page, QJsonObject properites = QJsonObject(),
                            const bool backImmediate = false, const bool enterImmediate = false);
    bool backToSection(const Enums::PageSections section);

    // Popup managment
    void enterPopup(const Enums::Page page, QJsonObject properites = QJsonObject());
    void sendAction(Enums::PopupAction action);

private:
    const QString m_pagePrefix = QStringLiteral("qrc:/pages/");
    const Enums::Page m_initialPage = Enums::Page::Login;
    const Enums::Page m_homePage = Enums::Page::ViewType;

    QVector<Enums::Page> m_pageStack;
    PageSectionsModel m_pageSectionsModel;

    void prepareConnections();
    QString pageToQString(const Enums::Page p) const;
};

#endif // PAGEMANAGER_H
