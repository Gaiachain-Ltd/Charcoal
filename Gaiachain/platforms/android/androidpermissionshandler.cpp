#include "androidpermissionshandler.h"

#include <QMap>
#include <QDebug>
#include <QtAndroidExtras/QtAndroid>

namespace {
    const auto AndroidPermissionsList = QMap<Android::PermissionsHandler::Permission, QStringList>{
        { Android::PermissionsHandler::Permission::Internet, { "android.permission.INTERNET", } },
        { Android::PermissionsHandler::Permission::Storage, { "android.permission.READ_EXTERNAL_STORAGE", "android.permission.WRITE_EXTERNAL_STORAGE" } },
        { Android::PermissionsHandler::Permission::Camera, { "android.permission.CAMERA", } },
        { Android::PermissionsHandler::Permission::Location, { "android.permission.ACCESS_COARSE_LOCATION", "android.permission.ACCESS_FINE_LOCATION" } }
    };
}

Android::PermissionsHandler &Android::PermissionsHandler::instance()
{
    static Android::PermissionsHandler pm;
    return pm;
}

void Android::PermissionsHandler::requestPermission(const Permission &permission) const
{
    requestPermissions({ permission, });
}

void Android::PermissionsHandler::requestPermissions(const QList<Permission> &permissions) const
{
    auto permissionsList = QStringList{};
    std::for_each(permissions.constBegin(), permissions.constEnd(),
                  [&permissionsList](const auto &permission) {
        permissionsList.append(AndroidPermissionsList.value(permission));
    });

    auto deniedPermissions = QStringList{};
    std::copy_if(permissionsList.constBegin(), permissionsList.constEnd(),
                 std::back_inserter(deniedPermissions), [](const auto &permission) {
        return QtAndroid::checkPermission(permission) == QtAndroid::PermissionResult::Denied;
    });

    if (!deniedPermissions.isEmpty()) {
        QtAndroid::requestPermissions(
            deniedPermissions, [this](const QtAndroid::PermissionResultMap &map) {
                qDebug() << "Permission request result" << map.keys();
                for (const QString &permission : map.keys()) {
                    const QtAndroid::PermissionResult result = map.value(permission);
                    qDebug() << "P:" << permission << "result:" << int(result);
                    if (result == QtAndroid::PermissionResult::Granted) {
                        const auto permEnum = findPermission(permission);
                        if (permEnum == Permission::Unknown) {
                            qWarning() << "UNKNOWN permission was granted!"
                                       << permEnum << permission;
                        }
                        emit permissionGranted(permEnum);
                    }
                }
            });
    }
}

Android::PermissionsHandler::PermissionsHandler()
    : QObject()
{}

Android::PermissionsHandler::Permission Android::PermissionsHandler::findPermission(const QString &text) const
{
    for (const Permission permission : AndroidPermissionsList.keys()) {
        const QStringList values(AndroidPermissionsList.value(permission));
        for (const QString &value : values) {
            if (value == text) {
                return permission;
            }
        }
    }

    return Permission::Unknown;
}
