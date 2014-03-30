/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#ifndef CMYCRC32_H
#define CMYCRC32_H

#include "_mingw.h"

class CMyCRC32
{
public:
    CMyCRC32();

    unsigned long crc32File(const char* name);
    unsigned long crc32Calc(const unsigned char* pBuf, size_t cSize);
    unsigned long crc32CalcFirst(const unsigned char* pBuf, size_t cSize);
    unsigned long crc32CalcNext(unsigned long crc1, const unsigned char* pBuf, size_t cSize);
    unsigned long crc32CalcResult(unsigned long crc1);
private:
    unsigned long m_crcTable[256];
    void crcGenTable( );
};

#endif // CMYCRC32_H
