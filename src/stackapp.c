//***********************************************************//
//  Arquivo: STACKAPP.C                                      //
//  Autor: Roberto Bauer                                     //
//  Observacoes:                                             //
//              Dados especificos para aplicacoes de pilhas. //
//***********************************************************//

#define __STACKAPP_C__

#include "main.h"


SDATA *brcViewData( SSTACK *pStk, int iPos)
{
    return ((SDATA *) ViewData(pStk, iPos));
}

SDATAPOST *postViewData( SSTACK *pStk, int iPos)
{
    return ((SDATAPOST *) ViewData(pStk, iPos));
}

SDATAIN *inViewData( SSTACK *pStk, int iPos)
{
    return ((SDATAIN *) ViewData(pStk, iPos));
}

SDATAIN *inViewTopData( SSTACK *pStk)
{
    return ((SDATAIN *) ViewTopData(pStk));
}
