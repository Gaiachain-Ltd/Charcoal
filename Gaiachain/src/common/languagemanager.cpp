#include "languagemanager.h"

#include <QString>
// TODO: save language in database, not in settings?
#include <QSettings>
#include <QCoreApplication>

#include <QDebug>

Language::Language(const QLocale::Language lang)
{
    languageValue = lang;
    language = QLocale::languageToString(lang);
    icon = QLatin1String(":/ui/") + QString(lang == QLocale::French? "fr" : "en");
}

LanguageManager::LanguageManager(QObject *parent) : QObject(parent)
{
}

void LanguageManager::setLanguage(const QLocale::Language language)
{
    {
        QSettings settings;
        settings.setValue(QLatin1String("language"),
                          QLocale::languageToString(language));
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

QList<Language> LanguageManager::languages() const
{
    return m_languages;
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
        qDebug() << "Installed language:" << language;
    } else {
        QLocale::setDefault(QLocale::Language::English);
        m_currentLanguage = QLocale::English;
        qWarning() << "Cannot load translaction! Language:" << language;
    }
}
