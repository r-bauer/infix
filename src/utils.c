//***********************************************************//
//                                                           //
//  Arquivo: UTILS.C                                         //
//  Autor: Roberto Bauer                                     //
//  Observacoes: Rotinas utlitarias                          //
//                                                           //
//***********************************************************//

#define __UTILS_C__


#include "main.h"


BOOL IsDigit(char c)
{
    return (c >= '0' && c <= '9');
}

BOOL IsNumber(char *pStr)
{
    if (*pStr == '\0' || pStr == NULL)
        return FALSE;

    while (*pStr) {
        if (!IsDigit(*pStr)) {
            return FALSE;
        }
        pStr++;
    }
    return TRUE;
}

int Atoi(char *pStr)
{
    int iRet = 0;

    while (*pStr) {
        iRet *= 10;
        iRet += (*pStr++ & 0xF);
    }
    return (iRet);
}

BOOL IsOperator(char c)
{
    char cOp[] = {"+-/*$"};
    char *ptr;

    ptr = cOp;
    while (*ptr) {
        if (*ptr++ == c)
            return TRUE;
    }
    return FALSE;
}

int Abs(int i)
{
    if (i < 0)
    {
        i *= -1;
    }

    return i;
}

float Expon(float fBas, int iExp)
{
    float fRes;
    int iAbsExp;

    iAbsExp = Abs(iExp);

    fRes = fBas;
    while (--iAbsExp)
        fRes *= fBas;

    if (iExp < 0)
    {
        fRes = 1 / fRes;
    }

    return fRes;
}

BOOL IsBracket(char c)
{
    char cOp[] = {"()"};
    char *ptr;

    ptr = cOp;
    while (*ptr) {
        if (*ptr++ == c)
            return TRUE;
    }
    return FALSE;
}

