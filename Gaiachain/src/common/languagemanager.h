#pragma once

#include <memory>

#include <QObject>
#include <QLocale>
#include <QTranslator>

class Language
{
    Q_GADGET

    Q_PROPERTY(QString language MEMBER language)
    Q_PROPERTY(QString icon MEMBER icon)

public:
    Language(const QLocale::Language lang);

    QLocale::Language languageValue;
    QString language;
    QString icon;
};

class LanguageManager : public QObject
{
    Q_OBJECT

public:
    LanguageManager(QObject *parent = nullptr);

    void setLanguage(const QLocale::Language language);
    QLocale::Language language() const;

    void load();

    QList<Language> languages() const;

private:
    void load(const QLocale::Language language);

    const QString m_languageSetting = QLatin1String("language");

    const QList<Language> m_languages = {
        Language(QLocale::French),
        Language(QLocale::English)
    };

    QLocale::Language m_currentLanguage = QLocale::French;
    std::unique_ptr<QTranslator> m_translator;
};
