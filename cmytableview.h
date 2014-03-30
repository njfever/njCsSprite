/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#ifndef CMYTABLEVIEW_H
#define CMYTABLEVIEW_H

#include <QTableView>

class CMyTableView : public QTableView
{
    Q_OBJECT

private:
    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);

public:
    explicit CMyTableView(QWidget* parent = 0);
    
signals:
    void rowSelectionChanged();

public slots:
    
};

#endif // CMYTABLEVIEW_H
