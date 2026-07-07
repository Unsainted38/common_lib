greaterThan(QT_MAJOR_VERSION, 4) {
    QT += serialport widgets
} else {
    CONFIG += serialport
    QT += gui
}


macx: {
    OS_SUFFIX = macOS
    USERNAME = erikveraksich
}

linux: {
    OS_SUFFIX = linux
    USERNAME = user
}

linux_qt5-g++: {
    OS_SUFFIX = linux_qt5
    USERNAME = user
    QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/../lib.$${OS_SUFFIX}/
    QMAKE_LFLAGS += -Wl,--rpath=/home/user/develop/target/linux_qt5/lib.linux_qt5
    QMAKE_CXXFLAGS += -std=gnu++17
}

linux-g++: {
    OS_SUFFIX = linux
    USERNAME = user
    QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/../lib.$${OS_SUFFIX}/
    QMAKE_LFLAGS += -Wl,--rpath=/home/user/develop/target/linux/lib.linux
    QMAKE_CFLAGS_WARN_ON += -Wno-class-memaccess
    QMAKE_CXXFLAGS_WARN_ON += -Wno-class-memaccess
    QMAKE_CFLAGS += -Wno-class-memaccess
    QMAKE_CXXFLAGS += -Wno-class-memaccess
    QMAKE_CXXFLAGS += -std=gnu++17
#    DEFINES += nullptr=0
#    QMAKE_CXXFLAGS += -std=gnu++11
}

BUILD_FLAG = debug

CONFIG(debug, debug|release) {
    BUILD_FLAG = debug
    TARGET = $$join(TARGET,,,d)
} else {
    BUILD_FLAG = release
}

QT_PROFILE = Qt_$${replace(QT_VERSION, \\., _)}_for_$${OS_SUFFIX}

OUT_PWD = $$_PRO_FILE_PWD_/build/$${QT_PROFILE}/$${BUILD_FLAG}
MOC_DIR = $${OUT_PWD}/moc
OBJECTS_DIR = $${OUT_PWD}/obj
UI_DIR = $${OUT_PWD}/ui
RCC_DIR = $${OUT_PWD}/rcc
if (equals(TEMPLATE, lib)) {
    DESTDIR = $${_PRO_FILE_PWD_}/lib/$${QT_PROFILE}/$${BUILD_FLAG}
}
if (equals(TEMPLATE, app)) {
    DESTDIR = $$OUT_PWD/bin
}


