#include "androidpermissionshandler.h"

#include <QMap>
#include <QtAndroidExtras/QtAndroid>

namespace {
    const auto AndroidPermissionsList = QMap<Android::PermissionsHandler::Permissions, QStringList>{
        { Android::PermissionsHandler::Permissions::Internet, { "android.permission.INTERNET", } },
        { Android::PermissionsHandler::Permissions::Storage, { "android.permission.READ_EXTERNAL_STORAGE", "android.permission.WRITE_EXTERNAL_STORAGE" } },
        { Android::PermissionsHandler::Permissions::Camera, { "android.permission.CAMERA", } },
        { Android::PermissionsHandler::Permissions::Location, { "android.permission.ACCESS_COARSE_LOCATION", "android.permission.ACCESS_FINE_LOCATION" } }
    };
}

Android::PermissionsHandler &Android::PermissionsHandler::instance()
{
    static Android::PermissionsHandler pm;
    return pm;
}

void Android::PermissionsHandler::requestPermission(const Permissions &permission) const
{
    requestPermissions({ permission, });
}

void Android::PermissionsHandler::requestPermissions(const QList<Permissions> &permissions) const
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
        QtAndroid::requestPermissions(deniedPermissions, [](const QtAndroid::PermissionResultMap &) {});
    }
}

Android::PermissionsHandler::PermissionsHandler()
    : QObject()
{}
