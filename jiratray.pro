#-------------------------------------------------
#
# Project created by QtCreator 2011-05-07T17:07:48
#
#-------------------------------------------------

QT       += core gui sql

TARGET = jiratray
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    editworklog.cpp \
    worklogdelegate.cpp \
    jira.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    editworklog.h \
    worklogdelegate.h \
    jira_xs.h \
    jira.h \
    settingsdialog.h

FORMS    += mainwindow.ui \
    editworklog.ui \
    settingsdialog.ui

RESOURCES += \
    jiratray.qrc

win32 {
    SOURCES += simplecrypt.cpp
    HEADERS += simplecrypt.h
    INCLUDEPATH += C:/MinGW/msys/1.0/local/include
    LIBS += -L"C:\\MinGW\\msys\\1.0\\local\\lib" -leasysoap.dll
}

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
    QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
    CONFIG += x86 ppc release
    INCLUDEPATH += /usr/local/include
    LIBS += -framework CoreFoundation -framework Security -L/usr/local/lib -leasysoap
}

unix {
    SOURCES += simplecrypt.cpp
    HEADERS += simplecrypt.h
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib -leasysoap
}
