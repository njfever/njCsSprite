/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#ifndef CSETUPPROJECTDLG_H
#define CSETUPPROJECTDLG_H

#include <QDialog>

namespace Ui {
class CSetupProjectDlg;
}

class CSetupProjectDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit CSetupProjectDlg(QWidget *parent = 0);
    ~CSetupProjectDlg();
    
    QString getDocName();
    void setDocName(const QString& strDocName);
    void setCanvasSize(long nWidth, long nHeight);
private:
    Ui::CSetupProjectDlg *ui;
};

#endif // CSETUPPROJECTDLG_H
