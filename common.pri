greaterThan(QT_MAJOR_VERSION, 4) {
    QT += serialport widgets
} else {
    CONFIG += serialport
    QT += gui
}

debian13_x86_64-g++ {
    OS_SUFFIX = Debian13
    USERNAME = user
}

linux-moxa-g++ {
    OS_SUFFIX = Moxa
    USERNAME = moxa
    QT -= gui widgets
    QMAKE_CXXFLAGS += -std=gnu++20
}

win32-g++ {
    OS_SUFFIX = win64
    USERNAME = erikveraksich
}

macx: {
    OS_SUFFIX = macOS
    USERNAME = erikveraksich
    QMAKE_LFLAGS -= -single_module
    QMAKE_LFLAGS_SHLIB -= -single_module
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

