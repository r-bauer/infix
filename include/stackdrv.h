//***************************************************************//
//  Arquivo: STACKDRV.H                                          //
//  Autor: Roberto Bauer                                         //
//  Observacoes:                                                 //
//              Controlador da aplicacaoh de exemplod de pilhas. //
//***************************************************************//

#ifndef __STACKDRV_H__
#define __STACKDRV_H__

#undef EXTERN
#ifdef __STACKDRV_C__
    #define EXTERN
#else
    #define EXTERN  extern
#endif


int CalcInfix(int, char *[]);

#endif  // #define __STACKDRV_H__ 