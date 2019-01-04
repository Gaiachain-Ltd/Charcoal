#include "pagemanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include "../helpers/utility.h"

PageManager::PageManager(QObject *parent) : QObject(parent)
{
    m_pageStack.push_back(m_initialPage);
    m_pageSectionsModel.stackReset(m_initialPage);
}

void PageManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("pageManager"), this);
    engine.rootContext()->setContextProperty(QStringLiteral("sectionsModel"), &m_pageSectionsModel);
}

/*!
 * \brief PageManager::enterPage
 *
    Method assuming that loops in page graph are prohibited.
    We're going back to specific page if it is already on the stack.

 * \param page
 * \param properites
 */
void PageManager::enterPage(Enums::Page page, QJsonObject properites)
{
    qDebug() << "Print stack on enter" << m_pageStack;

    if (m_pageStack.contains(page)) {
        qWarning() << "Page" << page << "is already on the stack. Going back to page.";
        backTo(page);
        return;
    }

    m_pageStack.push_back(page);
    m_pageSectionsModel.pagePushed(page);

    // TO_DO Add checking of page url correctness
    QString pageUrl = pageToQString(page);
    qDebug() << "Enter page" << pageUrl;

    properites.insert(QStringLiteral("page"), static_cast<int>(page));

    emit push(pageUrl, properites);
}

void PageManager::popPage()
{
    qDebug() << "Print stack on pop" << m_pageStack;

    if (m_pageStack.isEmpty()) {
        qWarning() << "Popping empty stack! Aborting.";
        return;
    }

    auto poppedPage = m_pageStack.takeLast();
    m_pageSectionsModel.pagePopped(m_pageStack.last());

    qDebug() << "Popped page" << pageToQString(poppedPage);

    emit pop();
}

void PageManager::goToInitialPage()
{
    m_pageStack.clear();
    m_pageStack.push_back(m_initialPage);
    m_pageSectionsModel.stackReset(m_initialPage);

    emit goToInitial();
}

bool PageManager::backTo(Enums::Page backPage)
{
    if (!m_pageStack.contains(backPage)) {
        qWarning() << "Page" << backPage << "is not on the stack. Returning.";
        return false;
    }

    Enums::Page currentTop = m_pageStack.last();
    while (currentTop != backPage) {
        m_pageStack.pop_back();
        currentTop = m_pageStack.last();
        m_pageSectionsModel.pagePopped(currentTop);
    }

    qDebug() << "Going back to page" << pageToQString(backPage);

    emit goBackToPage(backPage);

    return true;
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
