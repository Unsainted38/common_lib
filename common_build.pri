include(common_platform.pri)

PROJECT_BUILD_DIR = $$_PRO_FILE_PWD_/build/$${QT_PROFILE}/$${BUILD_FLAG}
MOC_DIR = $${PROJECT_BUILD_DIR}/moc
OBJECTS_DIR = $${PROJECT_BUILD_DIR}/obj
UI_DIR = $${PROJECT_BUILD_DIR}/ui
RCC_DIR = $${PROJECT_BUILD_DIR}/rcc
equals(TEMPLATE, lib) {
    DESTDIR = $${_PRO_FILE_PWD_}/lib/$${QT_PROFILE}/$${BUILD_FLAG}
}
equals(TEMPLATE, app) {
    DESTDIR = $$PROJECT_BUILD_DIR/bin
}

