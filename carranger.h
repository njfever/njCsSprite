/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#ifndef CARRANGER_H
#define CARRANGER_H

#include "_mingw.h"
#include "dataDefine.h"
#include <QObject>
#include <QList>
#include <QTime>

typedef struct tagRect
{
    long id;
    long x; // left pos
    long y; // top pos
    long w; // width
    long h; // height
    long s; // state default 0 - not use

    tagRect()
    {
        memset(this, 0x00, sizeof(tagRect));
    }

    tagRect(long id, long x, long y, long w, long h, long s)
    {
        this->id = id;
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        this->s = s;
    }
}tRect, *PtRect;

typedef struct tagNulltRectEdge
{
    long x;
    long y;

    tagNulltRectEdge(long x, long y)
    {
        this->x = x;
        this->y = y;
    }

    tagNulltRectEdge()
    {
        this->x = 0;
        this->y = 0;
    }
}NulltRectEdge, *PNulltRectEdge;

typedef QList<NulltRectEdge> NulltRectEdgeSet;

const long BLOCK_STATUS_NONE = 0;
const long BLOCK_STATUS_USED = 1;


class CArranger : public QObject
{
    Q_OBJECT

private:
    long m_blockWidth;  // in pixel
    long m_blockHeight; // in pixel
    long m_blockSpanHor;// in how much blocks
    long m_blockSpanVer;// in how much blocks
    long m_blockMarginTop;  // in pixel
    long m_blockMarginLeft; // in pixel
    long m_areaWidth;   // in pixel
    long m_areaHeight;  // in pixel
    long** m_map;
    QList<tRect> m_blocks;
    QTime m_timer;
    long m_nBlockRow;   // in how much blocks
    long m_nBlockColumn;// in how much blocks

    bool m_bStop;   // stop arrange
    bool isMapOK(bool bCheckAllItem = false) const;
    void releaseMapEntity();
    int updateMapEntity();
    int resetMap();

    long getRectSquareInMap(const tRect& rect) const;
    long getMinSurroundSquareInMap() const;
    int setMapBytRect(const tRect& rect, long nValue, long nCheckValue, bool bForce = true);
    int updateNulltRectEdgeSet(NulltRectEdgeSet& set, int nType = 0);   // nType 0 - auto(all); 1 - horizontal; 2 - vertical
    int getUsedRate(double& fUsedRate, bool bResetMap);

    void sendArrangeStatus(ArrangerStatusCode::enumArrangerStatusCode statusCode, long nCurItem, long nTotal, long nTimer, const QString& strLastInfo);
public:
    explicit CArranger(QObject * parent = 0);
    ~CArranger();
    int setParameter(long nAreaWidth, long nAreaHeight, long nBlockWidth, long nBlockHeight, long nHorSpan, long nVerSpan, long nMarginLeft, long nMarginTop);
    int setParameter(const ProjectConfig& config);
    int settRect(long id, long x, long y, long w, long h, bool bFixed);
    int arrangetRects(int nType = 0);   // 0 auto; 1 hor; 2 ver
    int gettRectPos(long id, long& x, long& y, long& w, long& h, bool& bFixed) const;
    int addtRect(long id, long x, long y, long w, long h, bool bFixed = false);
    int removetRect(long id);
    void removeAlltRect();
    void setStop(bool bStop);
signals:
    void sendCurArrangeStatus(ArrangerStatusCode::enumArrangerStatusCode statusCode, long nCurItem, long nTotal, long nTimer, const QString& strLastInfo);


};

#endif // CARRANGER_H
