/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    if(translator.load(QString(":/res/lang/njcs_%1.qm").arg(QSettings(QString("njfever"), STR_MY_TITLE).value(STR_MY_LANGUAGE).toString().toLower())))
    {
        qApp->installTranslator(&translator);
    }

    MainWindow w;
    w.show();
    
    return a.exec();
}
