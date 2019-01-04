#include "pagemanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include "../helpers/utility.h"

PageManager::PageManager(QObject *parent) : QObject(parent)
{
    connect(this, &PageManager::back, this, &PageManager::pop, Qt::DirectConnection);
}

void PageManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("pageManager"), this);
}

void PageManager::enterPage(Enums::Page page, QJsonObject properites)
{
    const QString pageStr = Utility::enumToQString<Enums::Page>(page, "Page");
    QString pageUrl = m_pagePrefix + pageStr + QStringLiteral("Page.qml");

    qDebug() << "Enter page" << pageUrl;
    // TO_DO Add checking of page url correctness
    properites.insert(QStringLiteral("page"), static_cast<int>(page));

    emit push(pageUrl, properites);
}

void PageManager::popPage()
{
    emit pop();
}

void PageManager::goToInitialPage()
{
    emit goToInitial();
}
