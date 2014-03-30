/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#include "cspritemgr.h"

#include "mainwindow.h"
#include <QDebug>

CSpriteMgr::CSpriteMgr(QObject *parent):
    QGraphicsScene(parent)
{
    m_nSpriteIndexCouunt = 0;
}

CSpriteMgr::~CSpriteMgr()
{
    reset();
}

void CSpriteMgr::updatePosOfSpriteInArranger()
{
    QRect rect;

    if(items().count() < 1)
    {
        return;
    }


    getMinRect(rect);

    m_arranger.removeAlltRect();

    CSprite* sp = NULL;
    for(int i = 0; i < items().count(); i++)
    {
        sp = (CSprite*)items()[i];

        m_arranger.addtRect(sp->getID(), sp->x() - rect.x(), sp->y() - rect.y(), sp->getWidth(), sp->getHeight(), sp->isFixed());
    }
}

void CSpriteMgr::getMinRect(QRect &rect)
{
    long nMinX = -1, nMinY = -1, nMaxX = -1, nMaxY = -1;
    CSprite* sp = NULL;
    QList<QGraphicsItem *>	sps = items();

    if(sps.count() < 1)
    {
        rect = QRect();
        return;
    }

    sp = (CSprite*)sps[0];
    nMinX = sp->x();
    nMinY = sp->y();
    nMaxX = sp->x() + sp->getWidth();
    nMaxY = sp->y() + sp->getHeight();

    for(int i = 1; i < sps.count(); i++)
    {
        sp = (CSprite*)sps[i];

        if(sp->x() < nMinX)
        {
            nMinX = sp->x();
        }

        if(sp->y() < nMinY)
        {
            nMinY = sp->y();
        }

        if(sp->x() + sp->getWidth() > nMaxX)
        {
            nMaxX = sp->x() + sp->getWidth();
        }

        if(sp->y() + sp->getHeight() > nMaxY)
        {
            nMaxY = sp->y() + sp->getHeight();
        }
    }

    long h = abs(nMaxY - nMinY);
    long w = abs(nMaxX - nMinX);
    rect = QRect(nMinX, nMinY, w, h);
}

void CSpriteMgr::switchFixed(int nType)
{
    if(nType == 0)
    {
        for(int i = 0; i < selectedItems().length(); i++)
        {
            CSprite* ps = (CSprite *)selectedItems()[i];

            ps->setFixed(!ps->isFixed());
        }
    }
    else
    {
        for(int i = 0; i < items().length(); i++)
        {
            CSprite* ps = (CSprite *)items()[i];

            if(nType == 1)
            {
                ps->setFixed(true);
            }
            else if(nType == 2)
            {
                ps->setFixed(false);
            }
        }
    }

    emit(mouseReleased());
}

void CSpriteMgr::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);

    for(int i = 0; i < selectedItems().length(); i++)
    {
        CSprite* ps = (CSprite *)selectedItems()[i];

        ps->setZValue(ps->zValue() + TOP_SPRITE_Z_VALUE);
    }
}

void CSpriteMgr::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    update();
}

void CSpriteMgr::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);

    for(int i = 0; i < selectedItems().length(); i++)
    {
        CSprite* ps = (CSprite *)selectedItems()[i];

        ps->setZValue(ps->zValue() - TOP_SPRITE_Z_VALUE);

        if(event->button() == Qt::RightButton)
        {
            ps->setFixed(!ps->isFixed());
        }
    }

    update();

    emit(mouseReleased());
}

int CSpriteMgr::setArrangerParam(const ProjectConfig& config)
{
    int err = 0;

    err = m_arranger.setParameter(config);

    return err;
}

int CSpriteMgr::addSprite(const QString& strFile, const QPoint& point, bool bFixed /*= false*/)
{
    unsigned long crc = m_myCRC32.crc32File(strFile.toLocal8Bit().constData());

    if(0 == crc)
    {
        return -1;
    }

    for(int i = 0; i < items().count(); i++)
    {
        if(((CSprite*)items()[i])->getCRC32() == crc)
        {
            return i + 1;
        }
    }

    CSprite* sprite = new CSprite();
    if(NULL == sprite)
    {
        return -2;
    }

    sprite->setCRC32(crc);
    sprite->setFixed(bFixed);

    if(sprite->loadImgFromFile(strFile) != 0 || sprite->isNull())
    {
        delete sprite;
        return -3;
    }

    QRect rect;
    rect.setLeft(0);
    rect.setTop(0);
    rect.setRight(sprite->getWidth());
    rect.setBottom(sprite->getHeight());

    sprite->setID(++m_nSpriteIndexCouunt);
    sprite->setPos(point);
    sprite->setBoundingRect(rect);
    addItem(sprite);
    m_arranger.addtRect(sprite->getID(), point.x(), point.y(), rect.width(), rect.height(), bFixed);
    return 0;
}

int CSpriteMgr::removeSprite(int nIndex)
{
    if(nIndex < 0 || nIndex > items().count() - 1)
    {
        return -1;
    }

    CSprite* ps = (CSprite*)items()[nIndex];
    items().removeAt(nIndex);
    m_arranger.removetRect(ps->getID());
    delete ps;
    return 0;
}

int CSpriteMgr::removeSpriteByID(long nId)
{
    CSprite* sp = NULL;
    for(int i = 0; i < items().count(); i++)
    {
        sp = (CSprite*)items()[i];
        if(sp->getID() == nId)
        {
            m_arranger.removetRect(nId);
            removeItem(sp);
            return 0;
        }
    }
    return -1;
}

int CSpriteMgr::reset()
{
    while(items().count() > 0)
    {
        CSprite* ps = (CSprite*)items()[0];
        items().removeAt(0);
        delete ps;
    }

    m_arranger.removeAlltRect();
    update();
    return 0;
}

void CSpriteMgr::stopArrange()
{
    m_arranger.setStop(true);
}

CArranger *CSpriteMgr::getArranger()
{
    return &m_arranger;
}

int CSpriteMgr::arrange(int nType)
{
    int err = 0;
    CSprite* ps = NULL;
    long param[4];
    bool bFixed;

    m_arranger.setStop(false);
    updatePosOfSpriteInArranger();

    err = m_arranger.arrangetRects(nType);

    for(int i = 0; i < items().count(); i++)
    {
        ps = (CSprite*)items()[i];
        m_arranger.gettRectPos(ps->getID(), param[0], param[1], param[2], param[3], bFixed);
        ps->setPos(param[0], param[1]);
    }

    return err;
}

void CSpriteMgr::drawForeground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    QRect t_rect;
    getMinRect(t_rect);
    painter->drawRect(t_rect);
    QGraphicsScene::drawForeground(painter, t_rect);
}
