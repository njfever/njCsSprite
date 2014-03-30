/*
  Copyright (C) 2013-2014 njfever@163.com
  For conditions of distribution and use, see copyright notice in dataDefine.h
*/

#include "cmycrc32.h"
#include "stdio.h"

CMyCRC32::CMyCRC32()
{
    crcGenTable();

}

unsigned long CMyCRC32::crc32File(const char* name)
{
    int c;
    FILE* pf = 0;
    register unsigned long crc;

    if (!(pf=fopen(name, "rb")))
    {
        return 0;
    }

    crc = 0xFFFFFFFF;
    while( (c=getc(pf)) != EOF ) {
        crc = ((crc>>8) & 0x00FFFFFF) ^ m_crcTable[ (crc^c) & 0xFF ];
    }
    fclose(pf);
    return( crc^0xFFFFFFFF );
}

unsigned long CMyCRC32::crc32Calc(const unsigned char* pBuf, size_t cSize)
{
    register size_t i;
    register unsigned long crc;

    crc = 0xFFFFFFFF;
    for (i = 0; i < cSize; i++)
    {
        crc = ((crc>>8) & 0x00FFFFFF) ^ m_crcTable[ (crc^pBuf[i]) & 0xFF ];
    }
    return( crc^0xFFFFFFFF );
}

unsigned long CMyCRC32::crc32CalcFirst(const unsigned char* pBuf, size_t cSize)
{
    register size_t i;
    register unsigned long crc;

    crc = 0xFFFFFFFF;
    for (i = 0; i < cSize; i++)
    {
        crc = ((crc>>8) & 0x00FFFFFF) ^ m_crcTable[ (crc^pBuf[i]) & 0xFF ];
    }

    return crc;
}

unsigned long CMyCRC32::crc32CalcNext(unsigned long crc1, const unsigned char* pBuf, size_t cSize)
{
    register size_t i;
    register unsigned long crc;

    crc = crc1;
    for (i = 0; i < cSize; i++)
    {
        crc = ((crc>>8) & 0x00FFFFFF) ^ m_crcTable[ (crc^pBuf[i]) & 0xFF ];
    }
    return crc;
}

unsigned long CMyCRC32::crc32CalcResult(unsigned long crc1)
{
    return ( crc1^0xFFFFFFFF );
}

void CMyCRC32::crcGenTable( )
{
    unsigned long crc, poly;
    int i, j;

    poly = 0xEDB88320L;
    for (i=0; i<256; i++)
    {
        crc = i;
        for (j=8; j>0; j--)
        {
            if (crc&1)
            {
                crc = (crc >> 1) ^ poly;
            }
            else
            {
                crc >>= 1;
            }
        }
        m_crcTable[i] = crc;
    }
}
