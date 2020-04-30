#ifndef LOCALE_H
#define LOCALE_H

#include <QMap>
#include <QString>
#include <QLocale>
#include <QTranslator>

// TODO: save language in database, not in settings?
#include <QSettings>
#include <QObject>

#define DEFAULT_LANGUAGE "french"

class Language {

public:
    void load() {
        QLocale::setDefault(QLocale(LocaleLanguage["english"]));
        QTranslator translator;
        if (!translator.load(QLocale(), QLatin1String(DEFAULT_LANGUAGE), QLatin1String(),
                             QLatin1String(":/translations"), QLatin1String(".qm"))) {
            qCWarning(coreMain) << "Cannot load translaction! Language:" << DEFAULT_LANGUAGE;
        } else {
            QLocale::setDefault(QLocale(LocaleLanguage[DEFAULT_LANGUAGE]));
            QCoreApplication::installTranslator(&translator);
        }
    }


private:
const QMap<QString, QLocale::Language> m_localeLanguage = {
    {"french", QLocale::French},
    {"english", QLocale::English}
};
};

#endif // LOCALE_H
