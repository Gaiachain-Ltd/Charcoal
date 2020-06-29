#include "abstractdatamanager.h"
#include "common/logs.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(dataManager, "data.manager")

AbstractDataManager::AbstractDataManager(QObject *parent)
    : AbstractManager(parent)
{
    m_processingThread.start();
}

AbstractDataManager::~AbstractDataManager()
{
    m_processingThread.quit();
    m_processingThread.wait();
}

void AbstractDataManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("dataManager"), this);
}

void AbstractDataManager::updateUserData(const UserData &userData)
{
    qDebug() << "Update user data:" << userData.email << userData.type;
    m_userData = userData;
}

void AbstractDataManager::sendOfflineActions()
{
    qWarning() << RED("AbstractDataManager cannot send offline actions!");
}

void AbstractDataManager::setupDatabase(const QString &dbPath)
{
    qWarning() << RED("AbstractDataManager cannot setup database!") << dbPath;
}

QVariantMap AbstractDataManager::createSummaryItem(
    const QString &headerText,
    const QVariant &value,
    const QString &inputIconSource,
    const QString &suffix,
    const QColor &highlightColor,
    const QColor &decorationColor,
    const QColor &secondaryTextColor,
    const Enums::DelegateType delegateType) const
{
    qDebug() << "Data is:" << headerText << value << inputIconSource << suffix << highlightColor << decorationColor << secondaryTextColor << delegateType;

    return {
        { "headerValue", headerText },
        { "value", value },
        { "inputIconSource", inputIconSource },
        { "suffixValue", suffix },
        { "delegateType", int(delegateType) },
        { "highlightColor", highlightColor },
        { "decorationColor", decorationColor },
        { "secondaryTextColor", secondaryTextColor },
        { "isHighlighted", (highlightColor.isValid() == true) }
    };
}


