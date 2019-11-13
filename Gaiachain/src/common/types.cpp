#include "types.h"

#include <QHash>
#include <QPair>
#include <QVariant>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

// All of this, because Qt doesn't allow to reqister converter for non custom types

using TypesPair = QPair<QVariant::Type, int>;
using ConverterFunc = std::function<void(QVariant &)>;

namespace {
const QChar StringListDelimeter = ';';

static const QHash<TypesPair, ConverterFunc> Converters = {
    { { QVariant::DateTime, QVariant::LongLong },
      [](QVariant &value) {
          auto dt = value.toDateTime();
          value.setValue(dt.toSecsSinceEpoch());
      } },
    { { QVariant::LongLong, QVariant::DateTime },
      [](QVariant &value) {
          auto ll = value.toLongLong();
          value.setValue(QDateTime::fromSecsSinceEpoch(ll));
      } },

    { { QVariant::StringList, QVariant::String },
      [](QVariant &value) {
          auto sl = value.toStringList();
          value.setValue(sl.join(StringListDelimeter));
      } },
    { { QVariant::String, QVariant::StringList },
      [](QVariant &value) {
          auto s = value.toString();
          value.setValue(s.split(StringListDelimeter, QString::SkipEmptyParts));
      } },

    { { QVariant::Map, QVariant::ByteArray },
      [](QVariant &value) {
          auto vm = value.toMap();
          value.setValue(QJsonDocument(QJsonObject::fromVariantMap(vm)).toJson());
      } },
    { { QVariant::ByteArray, QVariant::Map },
      [](QVariant &value) {
          auto s = value.toString();
          value.setValue(QJsonDocument::fromJson(s.toLatin1()).object().toVariantMap());
      } },
};
}

bool types::canCustomConvert(const QVariant &value, const int toType)
{
    return Converters.contains({ value.type(), toType });
}

bool types::customConvert(QVariant &value, const int toType)
{
    if (!canCustomConvert(value, toType)) {
        return false;
    }

    auto converter = Converters.value({ value.type(), toType });
    converter(value);
    return true;
}
