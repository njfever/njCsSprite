/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#ifndef CMYTHREAD_H
#define CMYTHREAD_H

#include <QThread>
#include "cspritemgr.h"
#include "carrangerctrldlg.h"

class CMyThread : public QThread
{
    Q_OBJECT
private:
    void run();

    int m_nType;
    long m_nMaxTake;
    CSpriteMgr* m_mgr;
    void* m_ctrlDlg;
    unsigned long m_uTimer;
public:
    explicit CMyThread(QObject *parent = 0);

    int setMgr(CSpriteMgr* mgr, int m_nType);
    int setCtrlDlg(void* ctrl);
signals:

public slots:
    void updateCurArrangeStatus(ArrangerStatusCode::enumArrangerStatusCode statusCode, long nCurItem, long nTotal, long nTimer, const QString& strLastInfo);

};

#endif // CMYTHREAD_H
