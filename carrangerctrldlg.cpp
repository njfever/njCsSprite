/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#include "carrangerctrldlg.h"
#include "ui_carrangerctrldlg.h"
#include <QDebug>


CArrangerCtrlDlg::CArrangerCtrlDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CArrangerCtrlDlg)
{
    ui->setupUi(this);
    setLayout(ui->verticalLayout);

    m_spMgr = NULL;
    m_nArrangeType = -1;
    m_workThread = new CMyThread();

    ui->progressBar->setRange(0, 1000);
    ui->progressBar->setValue(0);

    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint  | Qt::WindowSystemMenuHint);
}

CArrangerCtrlDlg::~CArrangerCtrlDlg()
{
    delete ui;
}

void CArrangerCtrlDlg::updateProgress(long nPercentX1000)
{
    ui->progressBar->setValue(nPercentX1000);
}

void CArrangerCtrlDlg::updateStatus(const QString &strInfo)
{
    ui->labStatus->setText(strInfo);
}

void CArrangerCtrlDlg::setArrangeType(CSpriteMgr *mgr, int nType)
{
    m_spMgr = mgr;
    m_nArrangeType = nType;
}

void CArrangerCtrlDlg::on_btnStop_clicked()
{
    CMyThread* workThread = (CMyThread*)m_workThread;

    if(workThread->isRunning())
    {
        m_spMgr->stopArrange();
        workThread->wait(3 * 1000);
        workThread->terminate();
    }
    close();
}

void CArrangerCtrlDlg::showEvent(QShowEvent *event)
{
    CMyThread* workThread = (CMyThread*)m_workThread;

    qDebug()<<workThread->isRunning();
    qDebug()<<event->type();

    if(!workThread->isRunning() && event->type() == QEvent::Show/*QEvent::ApplicationActivated*/)
    {
        workThread->setCtrlDlg(this);
        workThread->setMgr(m_spMgr, m_nArrangeType);
        workThread->start();
    }
}
