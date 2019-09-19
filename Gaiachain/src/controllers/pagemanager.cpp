#include "pagemanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "../common/logs.h"

PageManager::PageManager(QObject *parent) : AbstractManager(parent)
{
    m_pageStack.push_back(m_initialPage);
}

void PageManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("pageManager"), this);
}

/*!
 * \brief PageManager::enter
 *
    Method assuming that loops in page graph are prohibited.
    We're going back to specific page if it is already on the stack.

 * \param page
 * \param properites
 */
void PageManager::enter(const Enums::Page page, QJsonObject properites, const bool immediate)
{
    qDebug() << CYAN("[PAGE] Print stack on enter") << m_pageStack;
    qDebug() << CYAN("[PAGE] Enter:") << page << "properites:" << properites;

    if (!m_popupStack.isEmpty()) {
        qWarning() << "Popup stack not empty:" <<  m_popupStack
                   << ". Returning as page cannot be pushed over popup!";
        return;
    }


    if (m_pageStack.contains(page)) {
        qWarning() << "Page" << page << "is already on the stack. Going back to page.";
        backTo(page);
        return;
    }

    m_pageStack.push_back(page);

    properites.insert(QStringLiteral("page"), static_cast<int>(page));

    emit stackViewPush(pageToQString(page), properites, immediate);
}

void PageManager::enterPopup(const Enums::Popup popup, QJsonObject properites, const bool immediate)
{
    qDebug() << CYAN("[POPUP] Print stack on enter") << m_pageStack;
    qDebug() << CYAN("[POPUP] Enter:") << popup << "properites:" << properites;

    properites.insert(QStringLiteral("isPopup"), true);
    m_popupStack.push_back(popup);

    emit stackViewPush(pageToQString(popup), properites, immediate);
}

void PageManager::sendAction(Enums::PopupAction action)
{
    // Pop popup first
    back(true);

    emit popupAction(action);
}

void PageManager::back(const bool immediate)
{
    qDebug() << CYAN("[PAGE] Print stack on pop") << m_pageStack;
    qDebug() << CYAN("[POPUP] Print stack on pop") << m_popupStack;

    // ************** HANDLE popups first
    if (!m_popupStack.isEmpty()) {
        auto poppedPopup = m_popupStack.takeLast();
        qDebug() << "Popped popup" << pageToQString(poppedPopup);
        emit stackViewPop(immediate);

        return;
    }

    // ************** HANDLE pages next
    if (m_pageStack.isEmpty()) {
        qWarning() << "Popping empty stack! Aborting!";
        return;
    }

    // Prohibit from clearing stack by pop
    if (m_pageStack.count() == 1)
        return;

    auto poppedPage = m_pageStack.takeLast();

    qDebug() << "Popped page" << pageToQString(poppedPage);

    emit stackViewPop(immediate);
}

bool PageManager::backTo(const Enums::Page backPage, const bool immediate)
{
    // ***************** HANDLE popups first
    if (!m_popupStack.isEmpty())
        m_popupStack.clear();

    // ***************** HANDLE pages next
    if (!m_pageStack.contains(backPage)) {
        qWarning() << "Page" << backPage << "is not on the stack. Returning.";
        return false;
    }

    Enums::Page currentTop = m_pageStack.last();
    while (currentTop != backPage) {
        m_pageStack.pop_back();
        currentTop = m_pageStack.last();
    }

    qDebug() << "Going back to page" << pageToQString(backPage);

    emit stackViewBackToPage(backPage, immediate);

    return true;
}

void PageManager::backToAndEnter(const Enums::Page backPage, const Enums::Page page, QJsonObject properites,
                                     const bool backImmediate, const bool enterImmediate)
{
    backTo(backPage, backImmediate);
    enter(page, properites, enterImmediate);
}

QString PageManager::getInitialPageUrl() const
{
    return pageToQString(m_initialPage);
}

Enums::Page PageManager::homePage() const
{
    return m_homePage;
}

bool PageManager::isOnHomePage() const
{
    return isOnTop(m_homePage);
}

bool PageManager::isOnTop(Enums::Page page) const
{
    return m_pageStack.last() == page;
}
