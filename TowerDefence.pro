QT += core gui
#QT += multimediawidgets
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    affix.cpp \
    bullet.cpp \
    enemy.cpp \
    importmap.cpp \
    main.cpp \
    mainwindow.cpp \
    operator.cpp \
    startfrom.cpp

HEADERS += \
    affix.h \
    bullet.h \
    enemy.h \
    importmap.h \
    mainwindow.h \
    operator.h \
    startfrom.h \
    utility.h

FORMS += \
    importmap.ui \
    mainwindow.ui \
    startfrom.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
