isEmpty(COMMON_PLATFORM_PRI_INCLUDED) {
COMMON_PLATFORM_PRI_INCLUDED = 1

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += serialport widgets
} else {
    CONFIG += serialport
    QT += gui
}

debian13_x86_64-g++ {
    USERNAME = user
    BUILD_PLATFORM = Debian13_x86_64
    CONFIG += htra_real
    QT -= gui widgets
    QMAKE_RPATHDIR += /opt/qt6/lib
    QMAKE_RPATHDIR += /opt/common_lib/lib
    QMAKE_RPATHDIR += /opt/htraapi/lib
    HTRA_SDK_PATH = /opt/sdk/sysroot/opt/htraapi
}

linux-moxa-g++ {
    BUILD_PLATFORM = Moxa_ARMv7
    USERNAME = moxa
    QT -= gui widgets
    QMAKE_CXXFLAGS += -std=gnu++20
}

win32-g++ {
    BUILD_PLATFORM = windows_x86_64
    OS_SUFFIX = win64
    CPU_ARCH = x86_64
    USERNAME = erikveraksich
}

macx {
    BUILD_PLATFORM = macOS_aarch64
    USERNAME = erikveraksich
    QMAKE_LFLAGS -= -single_module
    QMAKE_LFLAGS_SHLIB -= -single_module
    QMAKE_CXXFLAGS += -fsanitize=address,undefined
    QMAKE_CXXFLAGS += -fno-omit-frame-pointer
    QMAKE_LFLAGS += -fsanitize=address,undefined
}

isEmpty(BUILD_PLATFORM) {
    error("BUILD_PLATFORM is not defined by SDK")
}

QT_VERSION_SAFE = $$replace(QT_VERSION, \\., _)
QT_PROFILE = Qt_$${QT_VERSION_SAFE}_for_$${BUILD_PLATFORM}

message("BUILD_PLATFORM = $$BUILD_PLATFORM")
message("QT_PROFILE     = $$QT_PROFILE")
}