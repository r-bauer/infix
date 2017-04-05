//***********************************************************//
//                                                           //
//  Arquivo: UTILS.H                                         //
//  Autor: Roberto Bauer                                     //
//  Observacoes: Rotinas utlitarias                          //
//                                                           //
//***********************************************************//

#ifndef __UTILS_H__
#define __UTILS_H__

#undef EXTERN
#ifdef __UTILS_C__
    #define EXTERN
#else
    #define EXTERN      extern
#endif


EXTERN BOOL IsDigit(char);
EXTERN BOOL IsNumber(char *);
EXTERN int Atoi(char *);
EXTERN BOOL IsOperator(char);
EXTERN int Abs(int);
EXTERN float Expon(float, int);
EXTERN BOOL IsBracket(char);
   

#endif                                  /* #ifndef __UTILS_H__ */
