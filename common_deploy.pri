# DEPLOY_NAME must be provided by the project/build environment.

isEmpty(DEPLOY_NAME) {
    message("DEPLOY_NAME is not defined")
    DEPLOY_NAME = local
}

DEPLOY_ROOT = /opt/$$lower($$DEPLOY_NAME)

equals(TEMPLATE, lib) {

    target.path = $$DEPLOY_ROOT/lib

    linux-moxa-g++ {
        QMAKE_LFLAGS += "-Wl,-rpath,'\$$ORIGIN'"
    }

    debian13_x86_64-g++ {
        QMAKE_LFLAGS += "-Wl,-rpath,'\$$ORIGIN'"
    }

    INSTALLS += target
}

equals(TEMPLATE, app) {

    target.path = $$DEPLOY_ROOT/bin

    linux-moxa-g++ {
        QMAKE_LFLAGS += "-Wl,-rpath,'\$$ORIGIN/../lib'"
    }

    debian13_x86_64-g++ {
        QMAKE_LFLAGS += "-Wl,-rpath,'\$$ORIGIN/../lib'"
    }

    INSTALLS += target
}