#ifndef ANDROIDPERMISSIONSHANDLER_H
#define ANDROIDPERMISSIONSHANDLER_H

#include <QObject>

namespace Android {
class PermissionsHandler final : public QObject
{
    Q_OBJECT

public:
    enum class Permission {
        Unknown = -1,
        Internet,
        Storage,
        Location,
        Camera
    };
    Q_ENUM(Permission)

    static PermissionsHandler &instance();

    Q_INVOKABLE void requestPermission(const Permission &permission) const;
    Q_INVOKABLE void requestPermissions(const QList<Permission> &permissions) const;

signals:
    void permissionGranted(const Permission permission) const;

private:
    PermissionsHandler();

    Permission findPermission(const QString &text) const;
};
}

#endif // ANDROIDPERMISSIONSHANDLER_H
