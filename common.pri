greaterThan(QT_MAJOR_VERSION, 4) {
    QT += serialport widgets
} else {
    CONFIG += serialport
    QT += gui
}

debian13_x86_64-g++ {
    OS_SUFFIX = Debian13
    CPU_ARCH = x86_64
    USERNAME = user
    CONFIG += htra_real
    QT -= gui widgets
    QMAKE_RPATHDIR += /opt/qt6/lib
    QMAKE_RPATHDIR += /opt/common_lib/lib/x86_64
    QMAKE_RPATHDIR += /opt/htraapi/lib/x86_64
    HTRA_SDK_PATH = /opt/sdk/sysroot/opt/htraapi
}

linux-moxa-g++ {
    OS_SUFFIX = Moxa
    CPU_ARCH = ARMv7
    USERNAME = moxa
    QT -= gui widgets
    QMAKE_CXXFLAGS += -std=gnu++20
}

win32-g++ {
    OS_SUFFIX = win64
    CPU_ARCH = x86_64
    USERNAME = erikveraksich
}

macx: {
    OS_SUFFIX = macOS
    CPU_ARCH = aarch64
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

QT_PROFILE = Qt_$${replace(QT_VERSION, \\., _)}_for_$${OS_SUFFIX}_$${CPU_ARCH}

PROJECT_BUILD_DIR = $$_PRO_FILE_PWD_/build/$${QT_PROFILE}/$${BUILD_FLAG}
MOC_DIR = $${PROJECT_BUILD_DIR}/moc
OBJECTS_DIR = $${PROJECT_BUILD_DIR}/obj
UI_DIR = $${PROJECT_BUILD_DIR}/ui
RCC_DIR = $${PROJECT_BUILD_DIR}/rcc
equals(TEMPLATE, lib) {
    DESTDIR = $${_PRO_FILE_PWD_}/lib/$${QT_PROFILE}/$${BUILD_FLAG}

    debian13_x86_64-g++ {
        target.path = $$LIB_INSTALL_DIR/x86_64
    }
    linux-moxa-g++ {
        target.path = $$LIB_INSTALL_DIR/armv7
    }

    INSTALLS += target
}
equals(TEMPLATE, app) {
    DESTDIR = $$PROJECT_BUILD_DIR/bin

    debian13_x86_64-g++ {
        target.path = /home/user/apps
    }

    linux-moxa-g++ {
        target.path = /home/moxa/apps
    }

    INSTALLS += target
}

