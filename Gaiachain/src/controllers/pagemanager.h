﻿#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QVector>
#include <QJsonObject>

#include "abstractmanager.h"
#include "../common/enums.h"
#include "../helpers/utility.h"

class PageManager : public AbstractManager
{
    Q_OBJECT
    Q_PROPERTY(Enums::Page topPage READ topPage NOTIFY topPageChanged)

public:
    explicit PageManager(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine) override;

    Q_INVOKABLE QString getInitialPageUrl() const;
    Q_INVOKABLE Enums::Page topPage() const;
    Q_INVOKABLE Enums::Page homePage() const;
    Q_INVOKABLE bool isOnTop(Enums::Page page) const;
    Q_INVOKABLE bool isOnHomePage() const;
    Q_INVOKABLE bool isBackToHomePage() const;

signals:
    void popupAction(const Enums::PopupAction &action, const QString &popupId) const;
    void popupClosed(const Enums::Popup &popup, const QString &popupId) const;

signals:
    // Signals below should only be used by StackView!!!
    void stackViewPush(const QString &url, const QVariantMap properties = QVariantMap{}, const bool immediate = false) const;
    void stackViewPop(const bool immediate = false) const;
    void stackViewPopTo(const Enums::Page page, const QVariantMap properties = QVariantMap{}, const bool immediate = false) const;
    void stackViewReplace(const QString &url, const QVariantMap properties = QVariantMap{}, const bool immediate = false) const;
    void popupManagerOpen(const QString &url, const QVariantMap properties = QVariantMap{}) const;
    void popupManagerClose() const;

    void topPageChanged(Enums::Page topPage);

public slots:
    // Page managment
    void enter(const Enums::Page page, QVariantMap properties = QVariantMap{}, const bool immediate = false);
    void enterReplace(const Enums::Page page, QVariantMap properties = QVariantMap{}, const bool immediate = false);
    void back(const bool immediate = false);
    bool backTo(const Enums::Page page, QVariantMap properties = QVariantMap{}, const bool immediate = false);

    // Popup managment
    void openPopup(const Enums::Popup popup, QVariantMap properties = QVariantMap{}, const QString &id = {});
    void closePopup();
    void sendAction(Enums::PopupAction action);

    bool backToAndOpenPopup(const Enums::Page page, const Enums::Popup popup,
                            QVariantMap pageProperties = QVariantMap{}, QVariantMap popupProperties = QVariantMap{},
                            const bool immediateBack = false, const QString &popupId = {});

private:
    const QString m_pagePrefix = QStringLiteral("qrc:/pages/");
    const QString m_popupsPrefix = QStringLiteral("qrc:/popups/");
    const Enums::Page m_initialPage = Enums::Page::Login;
    const Enums::Page m_homePage = Enums::Page::MainMenu;

    QVector<Enums::Page> m_pageStack;
    QVector<QPair<Enums::Popup, QString>> m_popupStack;

    void prepareConnections();

    template<typename T>
    QString toFilePath(const T p, const char* enumName) const {
        const QString pageStr = Utility::enumToQString<T>(p, enumName);
        return pageStr + enumName + QStringLiteral(".qml");
    }

    QString toFilePath(Enums::Page p) const {
        return m_pagePrefix + toFilePath<Enums::Page>(p, "Page");
    }

    QString toFilePath(Enums::Popup p) const {
        return m_popupsPrefix + toFilePath<Enums::Popup>(p, "Popup");
    }
};

#endif // PAGEMANAGER_H
