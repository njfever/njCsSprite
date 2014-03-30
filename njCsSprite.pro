#-------------------------------------------------
#
# Project created by QtCreator 2013-11-14T20:16:33
#
#-------------------------------------------------

QT       += core gui xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = njCsSprite
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    csprite.cpp \
    cspritemgr.cpp \
    cmycrc32.cpp \
    carranger.cpp \
    csetupprojectdlg.cpp \
    cmytableview.cpp \
    cmygraphicsview.cpp \
    carrangeoptiondlg.cpp \
    cmythread.cpp \
    carrangerctrldlg.cpp

HEADERS  += mainwindow.h \
    dataDefine.h \
    csprite.h \
    cspritemgr.h \
    cmycrc32.h \
    carranger.h \
    csetupprojectdlg.h \
    cmytableview.h \
    cmygraphicsview.h \
    carrangeoptiondlg.h \
    cmythread.h \
    carrangerctrldlg.h

FORMS    += mainwindow.ui \
    csetupprojectdlg.ui \
    carrangeoptiondlg.ui \
    carrangerctrldlg.ui

TRANSLATIONS += res/lang/njcs_en.ts \
                res/lang/njcs_chn.ts
                
RESOURCES += \
    res.qrc

RC_FILE = njCsSprite.rc
