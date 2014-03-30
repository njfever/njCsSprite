/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#include "cmytableview.h"

CMyTableView::CMyTableView(QWidget *parent) :
    QTableView(parent)
{
}

void CMyTableView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);

    viewport()->update();
    emit(rowSelectionChanged());
}
