//***********************************************************//
//  Arquivo: STACKAPP.H                                      //
//  Autor: Roberto Bauer                                     //
//  Observacoes:                                             //
//              Dados especificos para aplicacoes de pilhas. //
//***********************************************************//

#ifndef __STACKAPP_H__
#define __STACKAPP_H__

#undef EXTERN
#ifdef __STACKAPP_C__
    #define EXTERN
#else
    #define EXTERN  extern
#endif


typedef struct tagDATA
{
    char    cOpener;

} SDATA;

#define brcCreateStack(x)       CreateStack((x), sizeof(SDATA))
#define brcPopData(x, y)        PopData((x), ((void *)(y)))
#define brcPushData(x, y)       PushData((x), ((void *)(y)))

EXTERN SDATA * brcViewData( SSTACK *, int );



typedef struct tagDATAPOST
{
    float     fVal;

} SDATAPOST;

#define postCreateStack(x)       CreateStack((x), sizeof(SDATAPOST))
#define postPopData(x, y)        PopData((x), ((void *)(y)))
#define postPushData(x, y)       PushData((x), ((void *)(y)))

EXTERN SDATAPOST * postViewData( SSTACK *, int );


typedef struct tagDATAIN
{
    char     cOpr;

} SDATAIN;

#define inCreateStack(x)       CreateStack((x), sizeof(SDATAIN))
#define inPopData(x, y)        PopData((x), ((void *)(y)))
#define inPushData(x, y)       PushData((x), ((void *)(y)))

EXTERN SDATAIN * inViewData( SSTACK *, int );
EXTERN SDATAIN *inViewTopData( SSTACK * );


#endif  // #define __STACKAPP_H__ 


