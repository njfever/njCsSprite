/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#include "csprite.h"

CSprite::CSprite()
{
    m_uCRC32 = 0;
    m_qPixmap = new QPixmap();

    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);

    m_bFixed = false;
}

CSprite::~CSprite()
{
    if(m_qPixmap != NULL)
    {
        delete m_qPixmap;
        m_qPixmap = NULL;
    }
}

QRectF CSprite::boundingRect() const
{
    if(!isNull())
    {
        return QRectF(0, 0, m_boundingRect.width(), m_boundingRect.height());
    }
    else
    {
        return QRectF(0, 0, 0, 0);
    }
}

void CSprite::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(widget);

    if(!isNull())
    {
        painter->setPen(QPen(QBrush(QColor(0, 0, 255), Qt::Dense1Pattern), 4));
        painter->drawPixmap(0, 0, m_boundingRect.width(), m_boundingRect.height(), *m_qPixmap);

        if(option->state & QStyle::State_Selected)
        {
            painter->drawRect(0, 0, m_boundingRect.width(), m_boundingRect.height());
        }
    }

}

void CSprite::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setOpacity(0.7f);
    QGraphicsItem::mousePressEvent(event);
    update();
}

void CSprite::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    update();
}

void CSprite::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setOpacity(1.0f);
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void CSprite::setCRC32(quint32 nCRC32)
{
    m_uCRC32 = nCRC32;
}

quint32 CSprite::getCRC32() const
{
    return m_uCRC32;
}

quint32 CSprite::getHeight() const
{
    if(!isNull())
    {
        return m_qPixmap->height();
    }

    return 0;
}

quint32 CSprite::getWidth() const
{
    if(!isNull())
    {
        return m_qPixmap->width();
    }

    return 0;
}


void CSprite::setID(long nID)
{
    m_nID = nID;
}

long CSprite::getID()
{
    return m_nID;
}

QString CSprite::getFilePath() const
{
    return m_strFilePath;
}

QString CSprite::getFileExt() const
{
    return m_strFileExt;
}

bool CSprite::isNull() const
{
    if(m_qPixmap != NULL)
    {
        return m_qPixmap->isNull();
    }

    return false;
}

bool CSprite::isFixed() const
{
    return m_bFixed;
}

void CSprite::setFixed(bool bFixed)
{
    m_bFixed = bFixed;
}

void CSprite::setBoundingRect(QRect& rect)
{
    m_boundingRect = rect;
}

int CSprite::loadImgFromFile(const QString& strFilePath)
{
    if(!m_qPixmap->load(strFilePath))
    {
        return -1;
    }

    m_strFilePath = strFilePath;
    m_strFileExt = "";

    int i = strFilePath.lastIndexOf("/");
    int j = strFilePath.lastIndexOf("\\");
    int k = strFilePath.lastIndexOf(".");
    int m = i > j ? i : j;
    if( k > m)
    {
        m_strFileExt = strFilePath.mid(k+1);
    }

    return 0;
}

const QPixmap *CSprite::getPixmap() const
{
    return m_qPixmap;
}
