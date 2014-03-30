/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#include "carrangeoptiondlg.h"
#include "ui_carrangeoptiondlg.h"

CArrangeOptionDlg::CArrangeOptionDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CArrangeOptionDlg)
{
    ui->setupUi(this);
    setLayout(ui->verticalLayout);

    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint | Qt::WindowCloseButtonHint);
}

CArrangeOptionDlg::~CArrangeOptionDlg()
{
    delete ui;
}

void CArrangeOptionDlg::setOptionValue(const ProjectConfig &config)
{
    m_projConfig = config;
    ui->txtAreaWidth->setText(QString("%1").arg(config.nCanvasWidth));
    ui->txtAreaHeight->setText(QString("%1").arg(config.nCanvasHeight));
    ui->txtBlockWidth->setText(QString("%1").arg(config.nBlockWidth));
    ui->txtBlockHeight->setText(QString("%1").arg(config.nBlockHeight));
    ui->txtBlockSpanHor->setText(QString("%1").arg(config.nBlockSpanHor));
    ui->txtBlockSpanVer->setText(QString("%1").arg(config.nBlockSpanVer));
    ui->txtBlockMarginLeft->setText(QString("%1").arg(config.nBlockMarginLeft));
    ui->txtBlockMarginTop->setText(QString("%1").arg(config.nBlockMarginTop));
}

void CArrangeOptionDlg::getOptionValue(ProjectConfig& config) const
{
    config = m_projConfig;
    config.nCanvasWidth = ui->txtAreaWidth->text().toLong();
    config.nCanvasHeight = ui->txtAreaHeight->text().toLong();
    config.nBlockWidth = ui->txtBlockWidth->text().toLong();
    config.nBlockHeight = ui->txtBlockHeight->text().toLong();
    config.nBlockSpanHor = ui->txtBlockSpanHor->text().toLong();
    config.nBlockSpanVer = ui->txtBlockSpanVer->text().toLong();
    config.nBlockMarginLeft = ui->txtBlockMarginLeft->text().toLong();
    config.nBlockMarginTop = ui->txtBlockMarginTop->text().toLong();
}
