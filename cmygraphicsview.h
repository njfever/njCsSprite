/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#ifndef CMYGRAPHICSVIEW_H
#define CMYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QDebug>

class CMyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CMyGraphicsView(QWidget * parent = 0);

private:

    void drawBackground(QPainter * painter, const QRectF & rect);

signals:
    
public slots:
    void scrollActionTriggered(int nAction);
};

#endif // CMYGRAPHICSVIEW_H
