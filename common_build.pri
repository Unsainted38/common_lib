include(common_platform.pri)

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

