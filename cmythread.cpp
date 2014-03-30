/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#include "cmythread.h"
#include<QDebug>

CMyThread::CMyThread(QObject *parent) :
    QThread(parent)
{
    m_nType = -1;
    m_mgr = NULL;
    m_ctrlDlg = NULL;
    m_nMaxTake = 0;
}

int CMyThread::setMgr(CSpriteMgr* mgr, int nType)
{
    m_mgr = mgr;
    m_nType = nType;
    return 0;
}

int CMyThread::setCtrlDlg(void *ctrl)
{
    m_ctrlDlg = ctrl;
    return 0;
}

void CMyThread::updateCurArrangeStatus(ArrangerStatusCode::enumArrangerStatusCode statusCode, long nCurItem, long nTotal, long nTimer, const QString &strLastInfo)
{
    long nTotalTime = (long)(1.0f * nTimer * nTotal / nCurItem);
    long nRemainTime = nTotalTime - nTimer;
    QString strInfo;

    if(statusCode == ArrangerStatusCode::BEGIN ||
       statusCode == ArrangerStatusCode::ARRANGING)
    {
        if(nTotalTime > m_nMaxTake) m_nMaxTake = nTotalTime;
        double fTotalTime = m_nMaxTake / 1000.0f;
        double fRemainTime = (nRemainTime > 0 ? nRemainTime : 0) / 1000.0f;
        strInfo = tr("%1/%2\nfull time in %3 s (%4 min)\ntime remain %5 s (%6 min)")
                .arg(nCurItem)
                .arg(nTotal)
                .arg(fTotalTime, 9, 'f', 1)
                .arg(fTotalTime / 60.0f, 3, 'f', 1)
                .arg(fRemainTime, 9, 'f', 1)
                .arg(fRemainTime / 60.0f, 3, 'f', 1);

        long nPercent = (long)(1.0f * nCurItem / nTotal * 1000);
        ((CArrangerCtrlDlg*)m_ctrlDlg)->updateProgress(nPercent);
    }
    else
    {
        if(statusCode == ArrangerStatusCode::END)
        {
            ((CArrangerCtrlDlg*)m_ctrlDlg)->updateProgress(1000);
        }

        strInfo = tr("%1/%2\n%3\ntotal:%4 s (%5 min)")
                .arg(nCurItem)
                .arg(nTotal)
                .arg(strLastInfo)
                .arg(nTimer / 1000.0f, 9, 'f', 1)
                .arg(nTimer / 1000.0f / 60.0f, 3, 'f', 1);
    }

    ((CArrangerCtrlDlg*)m_ctrlDlg)->updateStatus(strInfo);
}

void CMyThread::run()
{
    m_nMaxTake = 0;
    if(m_nType == -1 || m_mgr == NULL || m_ctrlDlg == NULL)
    {
        return;
    }

    CArranger* agr = m_mgr->getArranger();
    connect(agr, SIGNAL(sendCurArrangeStatus(ArrangerStatusCode::enumArrangerStatusCode,long,long,long,QString)),
            this, SLOT(updateCurArrangeStatus(ArrangerStatusCode::enumArrangerStatusCode,long,long,long,QString)));

    m_mgr->arrange(m_nType);

    disconnect(agr, SIGNAL(sendCurArrangeStatus(ArrangerStatusCode::enumArrangerStatusCode,long,long,long,QString)),
               this, SLOT(updateCurArrangeStatus(ArrangerStatusCode::enumArrangerStatusCode,long,long,long,QString)));
}
