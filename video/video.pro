QT       += core gui multimedia  multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += D:\qt5.12.9\opencv\build\install\include
LIBS += D:\qt5.12.9\opencv\build\lib\libopencv_*.a


SOURCES += \
    camerathread.cpp \
    cameratread.cpp \
    main.cpp \
    search.cpp \
    widget.cpp

HEADERS += \
    camerathread.h \
    cameratread.h \
    search.h \
    widget.h

FORMS += \
    search.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
