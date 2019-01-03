#include "pagemanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include "../helpers/utility.h"

PageManager::PageManager(QObject *parent) : QObject(parent)
{
    m_pageSectionsModel.stackReset(m_initialPage);
}

void PageManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("pageManager"), this);
    engine.rootContext()->setContextProperty(QStringLiteral("sectionsModel"), &m_pageSectionsModel);
}

void PageManager::enterPage(Enums::Page page, const QVariant &properites)
{
    QString pageUrl = pageToQString(page);

    qDebug() << "Enter page" << pageUrl;
    // TO_DO Add checking of page url correctness

    m_pageSectionsModel.pagePushed(page);

    emit push(pageUrl, properites);
}

void PageManager::popPage()
{
    //m_pageSectionsModel.pagePushed(page); //TO_DO

    emit pop();
}

void PageManager::goToInitialPage()
{
    m_pageSectionsModel.stackReset(m_initialPage);
    emit goToInitial();
}

QString PageManager::getInitialPageUrl() const
{
    return pageToQString(m_initialPage);
}

QString PageManager::pageToQString(Enums::Page p) const
{
    const QString pageStr = Utility::enumToQString<Enums::Page>(p, "Page");
    return m_pagePrefix + pageStr + QStringLiteral("Page.qml");
}
