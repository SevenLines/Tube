#-------------------------------------------------
#
# Project created by QtCreator 2014-02-01T18:48:09
#
#-------------------------------------------------

QT       += core gui xml

MOC_DIR = moc
OBJECTS_DIR = obj
 
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tube
TEMPLATE = app

QMAKE_CXXFLAGS = -std=c++0x

win32 {
    # widows resource
    RC_FILE = assets/windows_resource.rc

    OTHER_FILES += \
        assets/windows_resource.rc
}

INCLUDEPATH += tools \

SOURCES += main.cpp\
        mainwindow.cpp \
    tube.cpp \
    tubesmodel.cpp \
    tubesdata.cpp \
    xmlutils.cpp \
    tubeform.cpp \
    qlabelimage.cpp \
    qloadimagethread.cpp \
    portalform.cpp \
    qlabelinfo.cpp \
    tubesdataloader.cpp \
    qwidgethelper.cpp \
    qlabelimagezoom.cpp \
    databaseroadinfodialog.cpp \
    dialogreport.cpp \
    tools/textedit.cpp \

HEADERS  += mainwindow.h \
    Enums.h \
    tube.h \
    tubesmodel.h \
    tubesdata.h \
    xmlutils.h \
    tubeform.h \
    qlabelimage.h \
    qloadimagethread.h \
    portalform.h \
    qlabelinfo.h \
    tubesdataloader.h \
    qwidgethelper.h \
    qlabelimagezoom.h \
    databaseroadinfodialog.h \
    dialogreport.h \
    tools/textedit.h \

FORMS    += mainwindow.ui \
    tubeform.ui \
    portalform.ui \
    databaseroadinfodialog.ui \
    dialogreport.ui

RESOURCES += \
    main.qrc

DISTFILES += \
    config.xml

