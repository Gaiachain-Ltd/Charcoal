INCLUDEPATH += $$PWD

mac:!ios {
    ICON = $$PWD/template.icns
    QMAKE_INFO_PLIST = $$PWD/Info.plist
}
