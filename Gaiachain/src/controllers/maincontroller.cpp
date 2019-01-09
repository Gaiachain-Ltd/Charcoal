#include "maincontroller.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "../common/enums.h"
#include "../helpers/utility.h"

#include <QZXing>

MainController::MainController(QObject *parent)
    : AbstractManager(parent)
{
    m_sessionManager.setOverlayManager(&m_overlayManager);
    auto enterInformationPopup = [&](const QString &text, const QString &acceptButtonString, const QString &rejectButtonString) {
        QJsonObject obj;
        obj.insert(QStringLiteral("text"), text);
        obj.insert(QStringLiteral("acceptButtonText"), acceptButtonString);
        obj.insert(QStringLiteral("rejectButtonText"), rejectButtonString);

        m_pageManager.enterPopup(Enums::Page::InformationPopup, obj);
    };
    connect(&m_sessionManager, &SessionManager::displayError, this, enterInformationPopup);
    connect(&m_sessionManager, &SessionManager::loginFinished, &m_userManager, &UserManager::parseLoginData, Qt::DirectConnection);

    connect(&m_pageManager, &PageManager::popupAction, this, [&](Enums::PopupAction action){
        switch (action) {
        case Enums::PopupAction::Accept:
            qDebug() << "Implement accept action!";
            break;
        case Enums::PopupAction::Cancel:
        default:
            break;
        }
    });
}

void MainController::setupQmlContext(QQmlApplicationEngine &engine)
{
    //Register namespace for enums first
    qmlRegisterUncreatableMetaObject(Enums::staticMetaObject, "com.gaiachain.enums", 1, 0,
                                     "Enums", "Cannot create namespace Enums in QML");
    qRegisterMetaType<Enums::CommodityType>("CommodityType");
    qRegisterMetaType<Enums::Page>("Page");
    qRegisterMetaType<Enums::PageSections>("PageSections");
    qRegisterMetaType<Enums::UserType>("UserType");
    qRegisterMetaType<Enums::PlaceType>("PlaceType");
    qRegisterMetaType<Enums::PopupAction>("PopupAction");

    engine.rootContext()->setContextProperty(QStringLiteral("utility"), Utility::instance());

    qmlRegisterSingletonType(QUrl("qrc:///GaiaStrings.qml"), "com.gaiachain.style", 1, 0, "Strings");
    qmlRegisterSingletonType(QUrl("qrc:///GaiaStyle.qml"), "com.gaiachain.style", 1, 0, "Style");

    m_pageManager.setupQmlContext(engine);
    m_dataManager.setupQmlContext(engine);
    m_userManager.setupQmlContext(engine);
    m_overlayManager.setupQmlContext(engine);
    m_sessionManager.setupQmlContext(engine);

    setupQZXing(engine);
}

void MainController::setupQZXing(QQmlApplicationEngine &engine)
{
    QZXing::registerQMLTypes();
    QZXing::registerQMLImageProvider(engine);
}
