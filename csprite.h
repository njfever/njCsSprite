/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#ifndef CSPRITE_H
#define CSPRITE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QBitmap>
#include <QStyleOptionGraphicsItem>

class CSprite : public QGraphicsItem
{
private:
    QPixmap* m_qPixmap;
    quint32 m_uCRC32;
    QString m_strFilePath;
    QString m_strFileExt;
    QRect m_boundingRect;
    long m_nID;
    bool m_bFixed;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
public:
    explicit CSprite();
    ~CSprite();

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

    void setCRC32(quint32 nCRC32);

    quint32 getCRC32() const;
    quint32 getHeight() const;
    quint32 getWidth() const;
    QString getFilePath() const;
    QString getFileExt() const;

    bool isNull() const;
    bool isFixed() const;
    void setFixed(bool bFixed);

    void setBoundingRect(QRect& rect);
    int loadImgFromFile(const QString& strFilePath);
    const QPixmap* getPixmap() const;

    void setID(long nID);
    long getID();
    
};

#endif // CSPRITE_H
