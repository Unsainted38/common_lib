include(common_platform.pri)

INCLUDEPATH +=  \
                $$PWD \
                $$PWD/cmd \
                $$PWD/algorithm \
                $$PWD/modbus \
                $$PWD/network_transport \
                $$PWD/requesters \
                $$PWD/devices \
                $$PWD/parsers \
                $$PWD/uacs_network_transport \

CONFIG(debug, debug|release) {
    COMMON_LIB_PATH = $$clean_path($$PWD/lib/$${QT_PROFILE}/debug)

    LIBS += -L$$COMMON_LIB_PATH -lcommon_libd

    macx {
        QMAKE_RPATHDIR += $$COMMON_LIB_PATH
    }
} else {
    COMMON_LIB_PATH = $$clean_path($$PWD/lib/$${QT_PROFILE}/release)

    LIBS += -L$$COMMON_LIB_PATH -lcommon_lib

    macx {
        QMAKE_RPATHDIR += $$COMMON_LIB_PATH
    }
}
