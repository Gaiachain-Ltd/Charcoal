#include "pagemanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include "../helpers/utility.h"

PageManager::PageManager(QObject *parent) : QObject(parent)
{
}

void PageManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("pageManager"), this);
}

void PageManager::enterPage(Enums::Page page)
{
    const QString pageStr = Utility::enumToQString<Enums::Page>(page, "Page");
    QString pageUrl = m_pagePrefix + pageStr + QStringLiteral("Page.qml");

    qDebug() << "Enter page" << pageUrl;
    // TO_DO Add checking of page url correctness

    emit push(pageUrl);
}

void PageManager::popPage()
{
    emit pop();
}

void PageManager::goToInitialPage()
{
    emit goToInitial();
}
