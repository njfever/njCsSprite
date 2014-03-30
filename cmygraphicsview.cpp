/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#include "cmygraphicsview.h"
#include "csprite.h"
#include <QScrollBar>

CMyGraphicsView::CMyGraphicsView(QWidget * parent) :
    QGraphicsView(parent)
{
    setViewportUpdateMode(FullViewportUpdate);

    connect(horizontalScrollBar(), SIGNAL(actionTriggered(int)), this, SLOT(scrollActionTriggered(int)));
    connect(verticalScrollBar(), SIGNAL(actionTriggered(int)), this, SLOT(scrollActionTriggered(int)));
}

void CMyGraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{

    QBrush brush;
    brush.setStyle(Qt::CrossPattern);

    painter->setOpacity(0.2);
    painter->fillRect(rect, brush);

    QGraphicsView::drawBackground(painter, rect);
}
void CMyGraphicsView::scrollActionTriggered(int nAction)
{
    Q_UNUSED(nAction);

    resetCachedContent();
}
