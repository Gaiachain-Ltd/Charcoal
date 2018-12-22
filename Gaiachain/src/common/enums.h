#ifndef ENUMS_H
#define ENUMS_H

#include <QObject>

namespace Enums {
    Q_NAMESPACE

    enum class CommodityType {
        Timber = 0,
        Charcoal,
        Cocoa
    };
    Q_ENUM_NS(CommodityType)
}

#endif // ENUMS_H
