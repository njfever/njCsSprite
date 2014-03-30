/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#include "csetupprojectdlg.h"
#include "ui_csetupprojectdlg.h"

CSetupProjectDlg::CSetupProjectDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSetupProjectDlg)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint | Qt::WindowCloseButtonHint);
}

CSetupProjectDlg::~CSetupProjectDlg()
{
    delete ui;
}

QString CSetupProjectDlg::getDocName()
{
    return ui->txtDocName->text();
}

void CSetupProjectDlg::setDocName(const QString &strDocName)
{
    ui->txtDocName->setText(strDocName);
}

void CSetupProjectDlg::setCanvasSize(long nWidth, long nHeight)
{
    ui->txtCanvasWidth->setText(QString("%1 px").arg(nWidth));
    ui->txtCanvasHeight->setText(QString("%1 px").arg(nHeight));
}
