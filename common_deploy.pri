include($$PWD/common_platform.pri)


equals(TEMPLATE, lib) {
    LIB_INSTALL_DIR = /opt/lower($$TARGET)/lib
    debian13_x86_64-g++ {
        target.path = $$LIB_INSTALL_DIR/x86_64
    }
    linux-moxa-g++ {
        target.path = $$LIB_INSTALL_DIR/armv7
    }

    INSTALLS += target
}
equals(TEMPLATE, app) {
    DEPLOY_ROOT = /opt/lower($$TARGET)
    contains(QMAKE_SPEC, debian13_x86_64-g++) {
        target.path = $$DEPLOY_ROOT/bin

        QMAKE_LFLAGS += '-Wl,-rpath,$$ORIGIN/../lib'
    }

    contains(QMAKE_SPEC, linux-moxa-g++) {
        target.path = $$DEPLOY_ROOT/bin

        QMAKE_LFLAGS += '-Wl,-rpath,$$ORIGIN/../lib'
    }

    INSTALLS += target
}