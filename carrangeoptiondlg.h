/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#ifndef CARRANGEOPTIONDLG_H
#define CARRANGEOPTIONDLG_H

#include <QDialog>
#include "dataDefine.h"

namespace Ui {
class CArrangeOptionDlg;
}

class CArrangeOptionDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit CArrangeOptionDlg(QWidget *parent = 0);
    ~CArrangeOptionDlg();
    
    void setOptionValue(const ProjectConfig& config);
    void getOptionValue(ProjectConfig& config) const;
private:
    Ui::CArrangeOptionDlg *ui;
    ProjectConfig m_projConfig;
};

#endif // CARRANGEOPTIONDLG_H
