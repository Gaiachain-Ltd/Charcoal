#ifndef LOCALE_H
#define LOCALE_H

#include <QMap>
#include <QString>
#include <QLocale>

#define DEFAULT_LANGUAGE "french"

const QMap<QString, QLocale::Language> LocaleLanguage = {
    {"french", QLocale::French},
    {"english", QLocale::English}
};

#endif // LOCALE_H
