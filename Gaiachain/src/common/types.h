#ifndef TYPES_H
#define TYPES_H

#include <QVariant>

namespace types {

void convert(QVariant &value, const QMetaType::Type toType);
QVariant convert(const QVariant &value, const QMetaType::Type toType);

bool canCustomConvert(const QVariant &value, const QMetaType::Type toType);
bool customConvert(QVariant &value, const QMetaType::Type toType);

}

#endif // TYPES_H
