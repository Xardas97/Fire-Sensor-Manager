QT += quick \
      network \
      concurrent \
      sql \
      multimedia

SOURCES += \
        Service/Communication/sensor.cpp \
        Service/Communication/sensorcommunication.cpp \
        Service/Communication/sensordetector.cpp \
        Service/Communication/sensorlist.cpp \
        Service/Map/floormaps.cpp \
        Service/Map/mapentry.cpp \
        Service/Map/mapimageprovider.cpp \
        Service/alarmmanager.cpp \
        Service/database.cpp \
        Service/filteredsensorlistmodel.cpp \
        Service/main.cpp \
        Service/service.cpp \
        Service/usersmodel.cpp \
        Service/warningtracker.cpp

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
    Service/Communication/sensor.h \
    Service/Communication/sensorcommunication.h \
    Service/Communication/sensordetector.h \
    Service/Communication/sensorlist.h \
    Service/Map/floormaps.h \
    Service/Map/mapentry.h \
    Service/Map/mapimageprovider.h \
    Service/alarmmanager.h \
    Service/database.h \
    Service/filteredsensorlistmodel.h \
    Service/service.h \
    Service/usersmodel.h \
    Service/warningtracker.h

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
