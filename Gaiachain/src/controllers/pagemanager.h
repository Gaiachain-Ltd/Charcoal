#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QVector>
#include <QJsonObject>

#include "abstractmanager.h"
#include "../common/enums.h"
#include "../helpers/utility.h"

class PageManager : public AbstractManager
{
    Q_OBJECT
public:
    explicit PageManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    Q_INVOKABLE QString getInitialPageUrl() const;
    Q_INVOKABLE Enums::Page homePage() const;
    Q_INVOKABLE bool isOnHomePage() const;
    Q_INVOKABLE bool isOnTop(Enums::Page page) const;

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

    // Popup managment
    // Use enterPopup only on QML side to handle strings and don't mess sendAction signal receivers!
    void enterPopup(const Enums::Popup popup, QJsonObject properites = QJsonObject(), const bool immediate = true);
    void sendAction(Enums::PopupAction action);

private:
    const QString m_pagePrefix = QStringLiteral("qrc:/pages/");
    const Enums::Page m_initialPage = Enums::Page::Login;
    const Enums::Page m_homePage = Enums::Page::MainMenu;

    QVector<Enums::Page> m_pageStack;
    QVector<Enums::Popup> m_popupStack;

    void prepareConnections();

    template<typename T>
    QString pageToQString(const T p, const char* enumName) const {
        const QString pageStr = Utility::enumToQString<T>(p, enumName);
        return  m_pagePrefix + pageStr + enumName + QStringLiteral(".qml");
    }

    QString pageToQString(Enums::Page p) const {
        return pageToQString<Enums::Page>(p, "Page");
    }

    QString pageToQString(Enums::Popup p) const {
        return pageToQString<Enums::Popup>(p, "Popup");
    }
};

#endif // PAGEMANAGER_H
