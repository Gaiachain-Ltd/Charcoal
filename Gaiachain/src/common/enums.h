#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

namespace Enums {
    Q_NAMESPACE

    enum class CommodityType {
        Timber = 0,
        Charcoal,
        Cocoa,
        Count
    };
    Q_ENUM_NS(CommodityType)

    inline uint qHash(CommodityType key, uint seed)
    {
        return ::qHash(static_cast<int>(key), seed);
    }
}


#endif // ENUMS_H
