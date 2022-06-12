QT += quick \
      network \
      concurrent \
      sql

SOURCES += \
        Communication/sensor.cpp \
        Communication/sensorcommunication.cpp \
        Communication/sensordetector.cpp \
        Communication/sensorlist.cpp \
        Map/mapimageprovider.cpp \
        database.cpp \
        filteredsensorlistmodel.cpp \
        main.cpp \
        service.cpp

RESOURCES += resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Communication/sensor.h \
    Communication/sensorcommunication.h \
    Communication/sensordetector.h \
    Communication/sensorlist.h \
    Map/mapimageprovider.h \
    database.h \
    filteredsensorlistmodel.h \
    service.h

DISTFILES +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Common/release/ -lCommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Common/debug/ -lCommon
else:unix: LIBS += -L$$OUT_PWD/../Common/ -lCommon

INCLUDEPATH += $$PWD/../Common
DEPENDPATH += $$PWD/../Common

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Common/release/libCommon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Common/debug/libCommon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Common/release/Common.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Common/debug/Common.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../Common/libCommon.a
