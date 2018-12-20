#!/usr/bin/env python3
import os

# Variables for CI integration
API_TOKEN = ''
GIT_URL = ''

# clang config
LLVM_VERSION = '3.8'
FORMAT_TOOL = "clang-format-" + LLVM_VERSION
TIDY_TOOL = "clang-tidy-diff-" + LLVM_VERSION + ".py"

# append clang-format to path
os.environ["PATH"] += ":/usr/lib/llvm-" + LLVM_VERSION + "/bin"

# append qmake to path (maybe we could make this on the system side ?)
os.environ["PATH"] += ":/home/milotoolsdev/development/qt-everywhere-opensource-src-5.7.0/qtbase/bin"
