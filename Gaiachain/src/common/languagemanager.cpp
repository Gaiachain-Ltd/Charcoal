#include "languagemanager.h"

#include <QString>
// TODO: save language in database, not in settings?
#include <QSettings>
#include <QCoreApplication>

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDebug>

Language::Language()
{
}

Language::Language(const QLocale::Language lang)
{
    languageValue = lang;
    language = QLocale::languageToString(lang);
    icon = QLatin1String("qrc:/ui/") + QString(lang == QLocale::French? "fr" : "en");
}

LanguageManager::LanguageManager(QObject *parent) : QObject(parent)
{
}

void LanguageManager::setLanguage(const QLocale::Language language)
{
    {
        QSettings settings;
        const QLocale locale(language);
        settings.setValue(m_languageSetting, locale.bcp47Name());
    }

    m_currentLanguage = language;
    load();
}

QLocale::Language LanguageManager::language() const
{
    return m_currentLanguage;
}

void LanguageManager::load()
{
    if (QCoreApplication::applicationName().isEmpty()
            || QCoreApplication::organizationDomain().isEmpty()) {
        qFatal("Cannot load translations - application name and domain is not set");
    }

    QSettings settings;
    qDebug() << "Settings path:" << settings.fileName();
    const QString language = settings.value(m_languageSetting,
                                            QLatin1String("fr")).toString();

    const QLocale locale(language);
    load(locale.language());
}

QVariantList LanguageManager::languages() const
{
    QVariantList result;
    for (const auto &lang : m_languages) {
        result.append(QVariant::fromValue(lang));
    }

    return result;
}

int LanguageManager::currentLanguageIndex() const
{
    int result = 0;
    for (const auto &language : qAsConst(m_languages)) {
        if (m_currentLanguage == language.languageValue) {
            return result;
        }

        result++;
    }

    return -1;
}

void LanguageManager::connectQmlEngine(QQmlApplicationEngine *engine)
{
    if (engine) {
        engine->rootContext()->setContextProperty(QStringLiteral("tr"), this);
    }
}

QString LanguageManager::empty() const
{
    return QString();
}

void LanguageManager::setCurrentLanguageIndex(int currentLanguage)
{
    if (currentLanguageIndex() == currentLanguage)
        return;

    if (currentLanguage < 0 || currentLanguage >= m_languages.length()) {
        qFatal("Invalid language index!");
    }

    setLanguage(m_languages.at(currentLanguage).languageValue);

    emit currentLanguageIndexChanged(currentLanguage);
}

void LanguageManager::load(const QLocale::Language language)
{
    const QLocale locale(language);
    QLocale::setDefault(locale);
    m_translator = std::make_unique<QTranslator>();
    if (m_translator->load(locale,
                           QLatin1String("gaiachain"), QLatin1String("_"),
                           QLatin1String(":/translations"), QLatin1String(".qm")))
    {
        QCoreApplication::installTranslator(m_translator.get());
        m_currentLanguage = language;
        qDebug() << "Installed language:"
                 << language
                 << locale.name()
                 << locale.bcp47Name()
                 << locale.uiLanguages();
    } else {
        QLocale::setDefault(QLocale::Language::English);
        m_currentLanguage = QLocale::English;
        qWarning() << "Cannot load translaction! Language:"
                   << language
                   << locale.name()
                   << locale.bcp47Name()
                   << locale.uiLanguages();
    }

    emit languageChanged();
    emit emptyChanged();
}
