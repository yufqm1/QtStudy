QT += core gui widgets gui-private

CONFIG += c++17

win32-msvc*:QMAKE_CXXFLAGS += /utf-8
# QMAKE_LFLAGS += "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""

SOURCES += \
    $$files($$PWD/src/*.cpp) \
    src/drawdashboard.cpp \
    src/wxmodel.cpp

HEADERS += \
    $$files($$PWD/src/*.h) \
    src/drawdashboard.h \
    src/wxmodel.h

FORMS += $$PWD/src/Test.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
