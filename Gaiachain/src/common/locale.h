#ifndef LOCALE_H
#define LOCALE_H

#include <memory>

#include <QMap>
#include <QString>
#include <QLocale>
#include <QTranslator>
#include <QCoreApplication>

// TODO: save language in database, not in settings?
#include <QSettings>
#include <QObject>

#include <QDebug>

class Language {
public:
    void load() {
        QLocale::setDefault(QLocale(m_defaultLanguage));
        m_translator = std::make_unique<QTranslator>();
        if (m_translator->load(QLocale(m_defaultLanguage),
                            QLatin1String("gaiachain"), QLatin1String("_"),
                            QLatin1String(":/translations"), QLatin1String(".qm")))
        {
            QCoreApplication::installTranslator(m_translator.get());
            qWarning() << "Installed language:" << m_defaultLanguage;
        } else {
            QLocale::setDefault(QLocale::Language::English);
            qWarning() << "Cannot load translaction! Language:" << m_defaultLanguage;
        }
    }

private:
    const QLocale::Language m_defaultLanguage = QLocale::French;

    const QVector<QLocale::Language> m_languages = {
        QLocale::French,
        QLocale::English
    };

    std::unique_ptr<QTranslator> m_translator;
};

#endif // LOCALE_H
