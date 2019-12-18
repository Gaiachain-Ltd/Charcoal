#ifndef ANDROIDPERMISSIONSHANDLER_H
#define ANDROIDPERMISSIONSHANDLER_H

#include <QObject>

namespace Android {
class PermissionsHandler final : public QObject
{
    Q_OBJECT
public:
    enum class Permissions {
        Unknown = -1,
        Internet,
        Storage,
        Location,
        Camera
    };
    Q_ENUM(Permissions)

    static PermissionsHandler &instance();

    Q_INVOKABLE void requestPermission(const Permissions &permission) const;
    Q_INVOKABLE void requestPermissions(const QList<Permissions> &permissions) const;

private:
    PermissionsHandler();
};
}

#endif // ANDROIDPERMISSIONSHANDLER_H
