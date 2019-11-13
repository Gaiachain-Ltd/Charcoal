#ifndef TYPES_H
#define TYPES_H

class QVariant;

namespace types {

bool canCustomConvert(const QVariant &value, const int toType);
bool customConvert(QVariant &value, const int toType);

}

#endif // TYPES_H
