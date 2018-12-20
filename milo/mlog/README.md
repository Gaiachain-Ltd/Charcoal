[TOC]

Milo Code DB main ([online](https://docs.milosolutions.com/milo-code-db/main/) | [offline](\ref milodatabasemain))

[Source code](https://github.com/milosolutions/mlog)

# Description

MLog is the reference logger class for Qt projects.

# Usage

MLog is intended to be used as a part of a main project. It should be enough
to add:
```
include(mlog.pri)
```
in to the project \*.pro file.

After activating logging to file:
```
logger()->enableLogToFile("Test log");
```
there is no need to include MLog in files.
All qDebug, qInfo(), etc will be logged automaticaly to file.

# Features

Main features:
1. Supports writing log messages to a file
2. Seamlessly integrates with qDebug(), qCDebug(), qInfo() etc.
3. Supports Qt categorised logging
4. Maintains 2 separate log files: current one and backup of the previous log file
5. Lightweight
6. Convenient, minimalistic API

For example usage, see example-log/main.cpp file. This project is fully
docummented - please check doxygen docs or see the comments directly in the
source file(s).

# Examples 

**example-log** - shows the simplest way to include and use MLog.

# License

This project is licensed under the MIT License - see the LICENSE-MiloCodeDB.txt file for details
