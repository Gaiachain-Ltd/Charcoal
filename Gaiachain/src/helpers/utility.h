#ifndef UTILITY_H
#define UTILITY_H

#include <QObject>
#include <QString>
#include <QMetaEnum>
#include <QDate>

#include "typetraits.h"
#include "../common/enums.h"

class Utility : public QObject
{
    Q_OBJECT
public:
    static Utility *instance();

    template<typename QEnum>
    static QString enumToQString(QEnum value, const char *enumName)
    {
        const QMetaObject smo = Enums::staticMetaObject;
        const int indexOfType = smo.indexOfEnumerator(enumName);
        const QMetaEnum enumerator = smo.enumerator(indexOfType);
        return QString(enumerator.valueToKey(static_cast<int>(value)));
    }

    Q_INVOKABLE qreal scaleByDpi(qreal num) const;
    Q_INVOKABLE qreal scaleRoundByDpi(qreal num) const;

    Q_INVOKABLE qreal clamp(qreal v, qreal min, qreal max) const;

    Q_INVOKABLE int parseInt(const QString &num) const;
    Q_INVOKABLE QDate convertDateString(const QString &dateStr, const QString &dateFormat = QString()) const;
    Q_INVOKABLE QString defaultDateFormat() const;

    Q_INVOKABLE bool validateId(const QString &id) const;
    Q_INVOKABLE bool validateEmail(const QString &email) const;

    Q_INVOKABLE int getScannedIdLength() const;

    Q_INVOKABLE bool isWeekend(const QDate &date) const;

    Q_INVOKABLE QString formatRawId(QString id) const;

    Q_INVOKABLE bool useCombobox() const;
    Q_INVOKABLE bool fakeData() const;

    template <typename C, std::enable_if_t<is_qt_array_type<C>::value, int> = 0>
    static QVariantList toVariantList(const C &arrayType, QMetaType::Type converToType = QMetaType::Void)
    {
        auto variantList = QVariantList{};
        for (const auto &type : arrayType) {
            auto variantValue = QVariant::fromValue(type);
            if (converToType != QMetaType::Void) {
                variantValue.convert(converToType);
            }
            variantList.append(variantValue);
        }
        return variantList;
    }

    template <typename C, std::enable_if_t<is_qt_dictionary_type<C>::value, int> = 0>
    static QVariantMap toVariantsMap(const C &dictionaryType,
                                     QMetaType::Type converToTypeKey = QMetaType::Void, QMetaType::Type converToTypeValue = QMetaType::Void)
    {
        auto variantsMap = QVariantMap{};
        for (const auto &key : dictionaryType.keys()) {
            auto variantKey = QVariant::fromValue(key);
            if (converToTypeKey != QMetaType::Void) {
                variantKey.convert(converToTypeKey);
            }
            auto variantValue = QVariant::fromValue(dictionaryType.value(key));
            if (converToTypeValue != QMetaType::Void) {
                variantValue.convert(converToTypeValue);
            }

            variantsMap.insert(variantKey.toString(), variantValue);
        }
        return variantsMap;
    }

private:
    Utility();

    qreal setupDpiScale();

    qreal m_dpiScale;

    QString m_dateFormat = QStringLiteral("yyyy/MM/dd");
    QRegExp m_emailRegex;
};

#endif // UTILITY_H
