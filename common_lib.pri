INCLUDEPATH +=  $$PWD \
                $$PWD/cmd \
                $$PWD/algorithm \
                $$PWD/network_transport \
                $$PWD/requesters \
                $$PWD/devices \
                $$PWD/parsers \
                $$PWD/uacs_network_transport \

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/lib/$${QT_PROFILE}/$${BUILD_FLAG} -lcommon_libd
} else {
    LIBS += -L$$PWD/lib/$${QT_PROFILE}/$${BUILD_FLAG} -lcommon_lib
}
