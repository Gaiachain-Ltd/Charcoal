#include "pagemanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "../common/logs.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(corePageManager, "core.pageManager")

PageManager::PageManager(QObject *parent) : AbstractManager(parent)
{
    m_pageStack.append(m_initialPage);
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
 * \param properties
 */
void PageManager::enter(const Enums::Page page, QVariantMap properties, const bool immediate)
{
    qCDebug(corePageManager) << CYAN("[PAGE] Print stack on enter") << m_pageStack;
    qCDebug(corePageManager) << CYAN("[PAGE] Enter:") << page << "properties:" << properties;

    if (!m_popupStack.isEmpty()) {
        qCWarning(corePageManager) << "Popup stack not empty:" <<  m_popupStack
                   << ". Returning as page cannot be pushed over popup!";
        return;
    }

    if (m_pageStack.contains(page)) {
        qCWarning(corePageManager) << "Page" << page << "is already on the stack. Going back to page.";
        backTo(page, properties, immediate);
        return;
    }

    m_pageStack.append(page);
    properties.insert(QStringLiteral("page"), static_cast<int>(page));
    emit stackViewPush(toFilePath(page), properties, immediate);
    emit topPageChanged(topPage());
}

void PageManager::enterReplace(const Enums::Page page, QVariantMap properties, const bool immediate)
{
    qCDebug(corePageManager) << CYAN("[PAGE] Print stack on enter") << m_pageStack;
    qCDebug(corePageManager) << CYAN("[PAGE] Enter replace:") << page << "properties:" << properties;

    if (!m_popupStack.isEmpty()) {
        qCWarning(corePageManager) << "Popup stack not empty:" <<  m_popupStack
                   << ". Returning as page cannot be pushed over popup!";
        return;
    }

    if (m_pageStack.contains(page)) {
        qCWarning(corePageManager) << "Page" << page << "is already on the stack. Going back to page.";
        backTo(page);
        return;
    }

    m_pageStack.removeLast();
    m_pageStack.append(page);
    properties.insert(QStringLiteral("page"), static_cast<int>(page));
    emit stackViewReplace(toFilePath(page), properties, immediate);
    emit topPageChanged(topPage());
}

void PageManager::openPopup(const Enums::Popup popup,
                            QVariantMap properties,
                            const QString &id)
{
    qCDebug(corePageManager) << CYAN("[POPUP] Print stack on enter") << m_pageStack;
    qCDebug(corePageManager) << CYAN("[POPUP] Enter:") << popup << "properties:" << properties;

    properties.insert(QStringLiteral("popup"), static_cast<int>(popup));
    properties.insert(QStringLiteral("popupId"), id);
    m_popupStack.append({ popup, id });
    emit popupManagerOpen(toFilePath(popup), properties);
}

void PageManager::closePopup()
{
    qCDebug(corePageManager) << CYAN("[PAGE] Print stack on pop") << m_pageStack;
    qCDebug(corePageManager) << CYAN("[POPUP] Print stack on pop") << m_popupStack;

    if (m_popupStack.isEmpty()) {
        qCWarning(corePageManager) << "Closing empty popup! Aborting!";
        return;
    }

    auto [ popup, popupId ] = m_popupStack.takeLast();
    qCDebug(corePageManager) << "Closing popup" << toFilePath(popup);
    emit popupClosed(popup, popupId);
    emit popupManagerClose();
}

void PageManager::sendAction(Enums::PopupAction action)
{
    // close popup first
    auto [ popup, popupId ] = m_popupStack.last();
    closePopup();

    emit popupAction(action, popupId);
}

bool PageManager::backToAndOpenPopup(const Enums::Page page,
                                     const Enums::Popup popup,
                                     const QVariantMap &pageProperties,
                                     const QVariantMap &popupProperties,
                                     const bool immediateBack,
                                     const QString &popupId)
{
    auto result = backTo(page, pageProperties, immediateBack);
    openPopup(popup, popupProperties, popupId);

    return result;
}

void PageManager::showNotificationWithLink(const Enums::Page page,
                                           const QString &header,
                                           const QString &text,
                                           const QString &redirectText)
{
    qDebug() << "Please notify!" << page << header << text << redirectText;
    openPopup(Enums::Popup::NotificationWithLink,
              {
                  { "headerText", header },
                  { "text", text },
                  { "redirectText", redirectText },
                  { "redirectPage", int(page) }
              });
}

void PageManager::onError(const QString &error)
{
    openPopup(Enums::Popup::Notification,
              {
                  { "text", error },
                  { "backgroundColor", "#cb0000" }
              });
}

void PageManager::back(const bool immediate)
{
    qCDebug(corePageManager) << CYAN("[PAGE] Print stack on pop") << m_pageStack;
    qCDebug(corePageManager) << CYAN("[POPUP] Print stack on pop") << m_popupStack;

    if (!m_popupStack.isEmpty()) {
        qCWarning(corePageManager) << "Trying back with popup opened! Aborting!";
        return;
    }

    if (m_pageStack.isEmpty()) {
        qCWarning(corePageManager) << "Popping empty stack! Aborting!";
        return;
    }

    // Prohibit from clearing stack by pop
    if (m_pageStack.count() == 1)
        return;

    auto poppedPage = m_pageStack.takeLast();
    qCDebug(corePageManager) << "Popped page" << toFilePath(poppedPage);
    emit stackViewPop(immediate);
    emit topPageChanged(topPage());
}

bool PageManager::backTo(const Enums::Page page, QVariantMap properties, const bool immediate)
{
    if (!m_popupStack.isEmpty()) {
        qCWarning(corePageManager) << "Trying back with popup opened! Aborting!";
        return false;
    }

    if (!m_pageStack.contains(page)) {
        qCWarning(corePageManager) << "Page" << page << "is not on the stack. Returning.";
        return false;
    }

    Enums::Page currentTop = m_pageStack.last();
    const Enums::Page closedPage = currentTop;

    while (currentTop != page) {
        m_pageStack.removeLast();
        currentTop = m_pageStack.last();
    }

    qCDebug(corePageManager) << "Going back to page" << toFilePath(page)
                             << "properties:" << properties
                             << "from page:" << closedPage;
    emit stackViewPopTo(page, properties, immediate);
    emit topPageChanged(topPage());

    return true;
}

QString PageManager::getInitialPageUrl() const
{
    return toFilePath(m_initialPage);
}

Enums::Page PageManager::topPage() const
{
    return m_pageStack.last();
}

Enums::Page PageManager::homePage() const
{
    return m_homePage;
}

bool PageManager::isOnTop(Enums::Page page) const
{
    return topPage() == page;
}

bool PageManager::isOnHomePage() const
{
    return topPage() == m_homePage;
}

bool PageManager::isBackToHomePage() const
{
    if (m_pageStack.size() < 2) {
        return false;
    }

    return m_pageStack.at(m_pageStack.size() - 2) == m_homePage;
}
