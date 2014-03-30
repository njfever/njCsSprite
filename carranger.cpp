/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#include "carranger.h"
#include <QTime>
#include <QDebug>

CArranger::CArranger(QObject *parent):
    QObject(parent)
{
    qRegisterMetaType<ArrangerStatusCode::enumArrangerStatusCode>();

    m_map = NULL;
    m_blockWidth = 4;
    m_blockHeight = 4;
    m_blockSpanHor = 1;
    m_blockSpanVer = 1;
    m_areaWidth = 100;
    m_areaHeight = 100;

    m_blockMarginTop = 1;
    m_blockMarginLeft = 1;

    m_nBlockRow = 0;
    m_nBlockColumn = 0;

    m_blocks.clear();
}

CArranger::~CArranger()
{
    m_blocks.clear();
    releaseMapEntity();
}

bool CArranger::isMapOK(bool bCheckAllItem) const
{
    if(m_map != NULL && m_nBlockRow != 0 && m_nBlockColumn != 0)
    {
        if(bCheckAllItem)
        {
            for(int i = 0; i < m_nBlockRow; i++)
            {
                if(m_map[i] == NULL)
                {
                    return false;
                }
            }

            return true;
        }
        else
        {
            return true;
        }
    }

    return false;
}

void CArranger::releaseMapEntity()
{
    if(m_map != NULL)
    {
        for(int i = 0; i < m_nBlockRow; i++)
        {
            if(m_map[i] != NULL)
            {
                delete m_map[i];
                m_map[i] = NULL;
            }
        }

        delete m_map;
        m_map = NULL;

        m_nBlockRow = 0;
        m_nBlockColumn= 0;
    }
}

int CArranger::updateMapEntity()
{
    releaseMapEntity();

    if(m_blockWidth < 1 || m_blockHeight < 1 ||
       m_blockMarginLeft < 1 || m_blockMarginTop < 1)
    {
        return -1;
    }

    if(m_areaWidth < m_blockWidth || m_areaHeight < m_blockHeight)
    {
        return -2;
    }

    m_nBlockRow = (m_areaHeight + m_blockHeight - 1) / m_blockHeight;   // =
    m_nBlockColumn = (m_areaWidth + m_blockWidth - 1) / m_blockWidth;   // ||


    m_map = new long*[m_nBlockRow];

    if(m_map == NULL)
    {
        m_nBlockRow = 0;
        m_nBlockColumn = 0;
        return -3;
    }

    memset(m_map, 0x00, sizeof(long*) * m_nBlockRow);

    int err = 0;
    for(int i = 0; i < m_nBlockRow; i++)
    {
        m_map[i] = new long[m_nBlockColumn];
        if(m_map[i] == NULL)
        {
            err = -1;
            break;
        }

        memset(m_map[i], 0x00, sizeof(long) * m_nBlockColumn);
    }

    if(err != 0)
    {
        releaseMapEntity();
        return -4;
    }

    return 0;
}

int CArranger::resetMap()
{
    int err = -1;

    if(m_map != NULL)
    {
        err = 0;
        for(int i = 0; i < m_nBlockRow; i++)
        {
            if(m_map == NULL)
            {
                err = -2;
                break;
            }
            else
            {
                memset(m_map[i], 0x00, sizeof(long) * m_nBlockColumn);
            }
        }
    }

    return err;
}


long CArranger::getRectSquareInMap(const tRect& rect) const
{
    long s = ((rect.w + m_blockWidth - 1) / m_blockWidth) *
              ((rect.h + m_blockHeight - 1) / m_blockHeight);
    return s;
}

long CArranger::getMinSurroundSquareInMap() const
{
    long nMinLeft = -1, nMinTop = -1, nMinRight = -1, nMinBottom = -1;

    for(int y = 0; y < m_nBlockRow; y++)
    {
        for(int x = 0; x < m_nBlockColumn; x++)
        {
            if(m_map[y][x] != 0)
            {
                if(nMinLeft == -1)
                {
                    nMinLeft = x;
                }
                else if(nMinLeft > x)
                {
                    nMinLeft = x;
                }

                if(nMinTop == -1)
                {
                    nMinTop = y;
                }
                else if(nMinTop > y)
                {
                    nMinTop = y;
                }

                if(nMinRight == -1)
                {
                    nMinRight = x;
                }
                else if(nMinRight < x)
                {
                    nMinRight = x;
                }

                if(nMinBottom == -1)
                {
                    nMinBottom = y;
                }
                else if(nMinBottom < y)
                {
                    nMinBottom = y;
                }
            }
        }
    }

    if(nMinLeft != -1)
    {
        long s = (nMinRight - nMinLeft + 1) * (nMinBottom - nMinTop + 1);
        return s;
    }

    return 0;
}

int CArranger::setMapBytRect(const tRect& rect, long nValue, long nCheckValue, bool bForce)
{
    int err = 0;
    long nInMapLeft = 0, nInMapTop = 0, nInMapRight = 0, nInMapBottom = 0;
    nInMapLeft = rect.x;
    nInMapTop = rect.y;
    nInMapRight = rect.x + (rect.w + m_blockWidth - 1) / m_blockWidth;
    nInMapBottom = rect.y + (rect.h +  m_blockHeight - 1) / m_blockHeight;

    if(nInMapLeft < 0 || nInMapTop < 0 || nInMapRight > m_nBlockColumn - 1 || nInMapBottom > m_nBlockRow - 1)
    {
        err = -1;
    }

    if(err == 0)
    {
        if(!bForce)
        {
            for(int y = nInMapTop; y <= nInMapBottom; y++)
            {
                for(int x = nInMapLeft; x <= nInMapRight; x++)
                {
                    if(m_map[y][x] != nCheckValue)
                    {
                        err = -2;
                        break;
                    }
                }

                if(err != 0)
                {
                    break;
                }
            }
        }
    }

    if(err == 0)
    {
        for(int y = nInMapTop; y <= nInMapBottom; y++)
        {
            for(int x = nInMapLeft; x <= nInMapRight; x++)
            {
                m_map[y][x] = nValue;
            }
        }
    }

    return err;
}

int CArranger::updateNulltRectEdgeSet(NulltRectEdgeSet& set, int nType)
{
    int err = 0;
    tRect rect;
    set.clear();

    resetMap();

    for(int i = 0; i < m_blocks.count(); i++)
    {
        rect = m_blocks[i];
        if(rect.s != BLOCK_STATUS_NONE)
        {
            rect.w += (m_blockSpanHor + 1) * m_blockWidth;
            rect.h += (m_blockSpanVer + 1) * m_blockHeight;
            setMapBytRect(rect, rect.id, 0, true);
        }
    }

    for(int i = 0; i < m_blocks.count(); i++)
    {
        rect = m_blocks[i];
        if(rect.s != BLOCK_STATUS_NONE)
        {
            rect.w += m_blockSpanHor * m_blockWidth;
            rect.h += m_blockSpanVer * m_blockHeight;
            setMapBytRect(rect, 0, 0, true);
        }
    }

    for(int y = 0; y < m_nBlockRow; y++)
    {
        for(int x = 0; x < m_nBlockColumn; x++)
        {
            if(m_map[y][x] != 0)
            {
                if(nType == 1 && y != 0)
                {
                    continue;
                }
                else if(nType == 2 && x != 0)
                {
                    continue;
                }

                set.push_back(NulltRectEdge(x, y));
            }
        }
    }

    return err;
}

int CArranger::getUsedRate(double& fUsedRate, bool bResetMap/* = false*/)
{
    int err = -1;
    int nRealSquare = 0;
    fUsedRate = 0.0f;

    if(m_bStop)
    {
        return 1;
    }

    if(bResetMap)
    {
        resetMap();
    }

    for(int i = 0; i < m_blocks.count(); i++)
    {
        if(m_blocks[i].s == BLOCK_STATUS_USED)
        {
            nRealSquare += ((m_blocks[i].w + m_blockWidth - 1) / m_blockWidth) *
                    ((m_blocks[i].h +m_blockHeight - 1) / m_blockHeight);
            if(bResetMap)
            {
                if(setMapBytRect(m_blocks[i], m_blocks[i].id, 0, false) != 0)
                {
                    return -1;
                }
            }
        }
    }

    if(nRealSquare > 0)
    {
        long nMinSurroundSquare = getMinSurroundSquareInMap();
        if(nMinSurroundSquare <= 0)
        {
            err = - 2;
        }
        else
        {
            fUsedRate = nRealSquare / (double)nMinSurroundSquare;
            err = 0;
        }
    }

    return err;
}

void CArranger::sendArrangeStatus(ArrangerStatusCode::enumArrangerStatusCode statusCode, long nCurItem, long nTotal, long nTimer, const QString &strLastInfo)
{
    emit sendCurArrangeStatus(statusCode, nCurItem, nTotal, nTimer, strLastInfo);
}

int CArranger::arrangetRects(int nType)
{
    int err = 0;
    int nCalced = 0;
    int nTotaltRect = -1;
    int nBesttRect = 0, nBestPosX = 0, nBestPosY = 0;
    double fUsedRate = -1.0f;
    double fMaxUsedRate = -1.0f;
    bool bFirst = true;

    NulltRectEdgeSet set;

    nTotaltRect = m_blocks.count();

    nCalced = 0;
    if(nTotaltRect < 1)
    {
        sendArrangeStatus(ArrangerStatusCode::END, nCalced, nTotaltRect, 0, tr("None rectange to be computed."));
        return 0;
    }
    else if(resetMap() != 0)
    {
        sendArrangeStatus(ArrangerStatusCode::ERR, nCalced, nTotaltRect, 0, tr("Reset mask map failed."));
        return -1;
    }

    sendArrangeStatus(ArrangerStatusCode::BEGIN, 0, nTotaltRect, 0, tr("Begin arrange"));

    for (int i = 0; i < nTotaltRect; i++)
    {
        if(m_blocks[i].s == BLOCK_STATUS_USED)
        {
            nCalced++;
            setMapBytRect(m_blocks[i], m_blocks[i].id, 0, true);
        }
    }

    m_timer.restart();
    while (err == 0 && !m_bStop && nCalced < nTotaltRect )
    {
        fUsedRate = -1.0f;
        fMaxUsedRate = fUsedRate;

        updateNulltRectEdgeSet(set, nType);

        if(bFirst)
        {
            bFirst = false;
            set.push_back(NulltRectEdge(0, 0));
        }

        for (int i = 0; !m_bStop && i < nTotaltRect; i++)
        {
            if(m_blocks[i].s == BLOCK_STATUS_NONE)
            {
                m_blocks[i].s = BLOCK_STATUS_USED;  // set to try

                for(int j = 0; !m_bStop && j < set.count(); j++)
                {
                    m_blocks[i].x = set[j].x;
                    m_blocks[i].y = set[j].y;


                    if(getUsedRate(fUsedRate, true) != 0)   // incorrect postion ?
                    {                                       // this place can be improved
                        continue;                           // eg: save block's status before setMapBytRect(), and restore after incorrect or complete calculation
                    }

                    sendArrangeStatus(ArrangerStatusCode::ARRANGING, nCalced, nTotaltRect, m_timer.elapsed(), tr("index:%1 calculated.").arg(nBesttRect));

                    if(fMaxUsedRate < 0.0f)
                    {
                        fMaxUsedRate = fUsedRate;
                        nBesttRect = i;
                        nBestPosX = set[j].x;
                        nBestPosY = set[j].y;
                    }
                    else if(fUsedRate > fMaxUsedRate)
                    {
                        fMaxUsedRate = fUsedRate;
                        nBesttRect = i;
                        nBestPosX = set[j].x;
                        nBestPosY = set[j].y;
                    }

                }

                m_blocks[i].s = BLOCK_STATUS_NONE;  // restore
            }
        }

        // check result, every big loop must compute an block
        if(m_bStop)
        {
            sendArrangeStatus(ArrangerStatusCode::END, nCalced, nTotaltRect, m_timer.elapsed(), tr("Stopped by user."));
        }
        else if(fMaxUsedRate > 0.0f)
        {
            nCalced++;
            m_blocks[nBesttRect].s = BLOCK_STATUS_USED;
            m_blocks[nBesttRect].x = nBestPosX;
            m_blocks[nBesttRect].y = nBestPosY;
            sendArrangeStatus(ArrangerStatusCode::ARRANGING, nCalced, nTotaltRect, m_timer.elapsed(), tr("Index:%1 calculated.").arg(nBesttRect));
        }
        else
        {
            sendArrangeStatus(ArrangerStatusCode::ERR, nCalced, nTotaltRect, m_timer.elapsed(), tr("Blocks overlapped or no enough blocks."));
            err = -3;   // found an can not be calculated block
        }
    }

    if(err == 0)
    {
        sendArrangeStatus(ArrangerStatusCode::END, nCalced, nTotaltRect, m_timer.elapsed(), tr("All rectanges are computed."));
    }

    return err;
}

//
// public
//
int CArranger::setParameter(long nAreaWidth, long nAreaHeight, long nBlockWidth, long nBlockHeight, long nHorSpan, long nVerSpan, long nMarginLeft, long nMarginTop)
{
    m_blockWidth = nBlockWidth;
    m_blockHeight = nBlockHeight;
    m_blockSpanHor = nHorSpan;
    m_blockSpanVer = nVerSpan;
    m_areaWidth = nAreaWidth;
    m_areaHeight = nAreaHeight;
    m_blockMarginLeft = nMarginLeft % m_blockWidth;
    m_blockMarginTop = nMarginTop % m_blockHeight;
    return updateMapEntity();
}

int CArranger::setParameter(const ProjectConfig& config)
{
    return setParameter(config.nCanvasWidth,
                        config.nCanvasHeight,
                        config.nBlockWidth,
                        config.nBlockHeight,
                        config.nBlockSpanHor,
                        config.nBlockSpanVer,
                        config.nBlockMarginLeft,
                        config.nBlockMarginTop);
}

int CArranger::addtRect(long id, long x, long y, long w, long h, bool bFixed /*= false*/)
{
    if(id <= 0)
    {
        return -1;
    }

    x = (x - m_blockMarginLeft + m_blockWidth - 1) / m_blockWidth;
    y = (y - m_blockMarginTop + m_blockHeight - 1) / m_blockHeight;

    if(x < 0) x = 0;
    if(y < 0) y = 0;

    m_blocks.push_back(tRect(id, x, y, w, h, bFixed?BLOCK_STATUS_USED:BLOCK_STATUS_NONE));
    return 0;
}

int CArranger::removetRect(long id)
{
    int err = -1;
    for(int i = 0; i < m_blocks.count(); i++)
    {
        if(m_blocks[i].id == id)
        {
            err = 0;
            m_blocks.removeAt(i);
            break;
        }
    }

    return err;
}

void CArranger::removeAlltRect()
{
    m_blocks.clear();
}

void CArranger::setStop(bool bStop)
{
    m_bStop = bStop;
}

int CArranger::settRect(long id, long x, long y, long w, long h, bool bFixed)
{
    int err = -1;

    for(int i = 0; i < m_blocks.count(); i++)
    {
        if(m_blocks[i].id == id)
        {
            err = 0;
            m_blocks[i].x = x;
            m_blocks[i].y = y;
            m_blocks[i].w = w;
            m_blocks[i].h = h;
            m_blocks[i].s = bFixed?BLOCK_STATUS_USED:BLOCK_STATUS_NONE;
            break;
        }
    }

    return err;
}

int CArranger::gettRectPos(long id, long& x, long& y, long& w, long& h, bool& bFixed) const
{
    int err = -1;
    tRect rect;

    for(int i = 0; i < m_blocks.count(); i++)
    {
        rect = m_blocks[i];
        if(rect.id == id)
        {
            err = 0;
            x = rect.x * m_blockWidth + m_blockMarginLeft;
            y = rect.y * m_blockHeight + m_blockMarginTop;
            w = rect.w;
            h = rect.h;
            bFixed = rect.s==BLOCK_STATUS_USED?true:false;
            break;
        }
    }

    return err;
}
