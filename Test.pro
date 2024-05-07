QT += core gui widgets gui-private charts

CONFIG += c++17

win32-msvc*:QMAKE_CXXFLAGS += /utf-8
# QMAKE_LFLAGS += "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""

SOURCES += \
    $$files($$PWD/src/*.cpp) \
    src/chartwin.cpp \
    src/drawdashboard.cpp \
    src/histogramview.cpp \
    src/wxmodel.cpp

HEADERS += \
    $$files($$PWD/src/*.h) \
    src/chartwin.h \
    src/drawdashboard.h \
    src/histogramview.h \
    src/wxmodel.h

FORMS += $$PWD/src/Test.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
