/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#ifndef CSPRITEMGR_H
#define CSPRITEMGR_H

#include "csprite.h"
#include "cmycrc32.h"
#include "carranger.h"
#include <QGraphicsScene>

#define TOP_SPRITE_Z_VALUE 9999

class CSpriteMgr : public QGraphicsScene
{
    Q_OBJECT

private:
    CArranger m_arranger;
    CMyCRC32 m_myCRC32;
    int m_nZValueCached;
    long m_nSpriteIndexCouunt;

    void drawForeground(QPainter * painter, const QRectF & rect);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public:
    CSpriteMgr(QObject * parent = 0);
    ~CSpriteMgr();

    void updatePosOfSpriteInArranger();
    void getMinRect(QRect& rect);
    void switchFixed(int nType = 0);    // 0 switch; 1 all; 2 none;
    int setArrangerParam(const ProjectConfig& config);
    int addSprite(const QString& strFile, const QPoint& point, bool bFixed = false);
    int arrange(int nType = 0);
    int removeSprite(int nIndex);
    int removeSpriteByID(long nId);
    int reset();
    void stopArrange();
    CArranger* getArranger();
signals:
    void mouseReleased();

public slots:

};

#endif // CSPRITEMGR_H
