/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#ifndef CARRANGERCTRLDLG_H
#define CARRANGERCTRLDLG_H

#include <QDialog>
#include <QShowEvent>
#include "cspritemgr.h"
#include "cmythread.h"

namespace Ui {
class CArrangerCtrlDlg;
}

class CArrangerCtrlDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit CArrangerCtrlDlg(QWidget *parent = 0);
    ~CArrangerCtrlDlg();
    
    void updateProgress(long nPercentX1000);
    void updateStatus(const QString& strInfo);
    void setArrangeType(CSpriteMgr *mgr, int nType);

private slots:

    void on_btnStop_clicked();

private:
    Ui::CArrangerCtrlDlg *ui;

    CSpriteMgr* m_spMgr;
    int m_nArrangeType;
    void* m_workThread;


    void showEvent(QShowEvent * event);
};

#endif // CARRANGERCTRLDLG_H
