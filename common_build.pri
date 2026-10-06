isEmpty(COMMON_BUILD_PRI_INCLUDED) {
    COMMON_BUILD_PRI_INCLUDED = 1

    include($$PWD/common_platform.pri)

    BUILD_FLAG = debug

    CONFIG(debug, debug|release) {
        BUILD_FLAG = debug
        TARGET = $$join(TARGET,,,d)
    } else {
        BUILD_FLAG = release
    }

    BUILD_ROOT = $$OUT_PWD/$${BUILD_FLAG}

    MOC_DIR = $$BUILD_ROOT/moc
    OBJECTS_DIR = $$BUILD_ROOT/obj
    UI_DIR = $$BUILD_ROOT/ui
    RCC_DIR = $$BUILD_ROOT/rcc

    equals(TEMPLATE, lib) {
        DESTDIR = $$_PRO_FILE_PWD_/lib/$${QT_PROFILE}/$${BUILD_FLAG}
    }
    equals(TEMPLATE, app) {
        DESTDIR = $$BUILD_ROOT/bin
    }

    message("OUT_PWD     = $$OUT_PWD")
    message("BUILD_FLAG  = $$BUILD_FLAG")
    message("BUILD_ROOT  = $$BUILD_ROOT")
    message("OBJECTS_DIR = $$OBJECTS_DIR")
    message("MOC_DIR     = $$MOC_DIR")
    message("DESTDIR     = $$DESTDIR")
}