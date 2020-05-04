#pragma once

#include <memory>

#include <QObject>
#include <QLocale>
#include <QTranslator>

class QQmlApplicationEngine;

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

    Q_PROPERTY(QList<Language> languages READ languages CONSTANT)
    Q_PROPERTY(int currentLanguageIndex READ currentLanguageIndex WRITE setCurrentLanguageIndex NOTIFY currentLanguageIndexChanged)

public:
    LanguageManager(QObject *parent = nullptr);

    void setLanguage(const QLocale::Language language);
    QLocale::Language language() const;

    void load();

    QList<Language> languages() const;

    int currentLanguageIndex() const;

    void connectQmlEngine(QQmlApplicationEngine *engine);

public slots:
    void setCurrentLanguageIndex(int currentLanguage);

signals:
    void currentLanguageIndexChanged(const int currentLanguage) const;
    void languageChanged() const;

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
