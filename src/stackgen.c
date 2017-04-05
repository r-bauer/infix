/*
  Arquivo: STACKGEN.C
  Autor: Roberto Bauer
  Observacoes:
              Primitivas genericas para operacoes de pilhas.
*/


#define __STACKGEN_C__

#include "main.h"


//-----------------------------------------------------------------------------
//
//    Funcao: 
//              ClearStack
//
//    Proposito: 
//              Limpar a pilha de todos os dados armazenados
//
//    Parametros:
//              SSTACK *pStk - ponteiro p/ a pilha a ser reinicializada
//
//    Retorno:
//            nenhum
//
//    Descricao:
//              Limpa a pilha, ao direcionar a posicaoh atual de dados para um 
//              item invalido; deixando assim a pilha vazia.
//              Indica -1(abaixo da linha de limite) na posicaoh atual dos dados
//
//    Historico:                                                     
//               Data            Autor            Comentario
//               18/09/2008      Roberto Bauer    Adaptou de J.Rex e A.Binstok           
//
//-----------------------------------------------------------------------------
void ClearStack(SSTACK *pStk)
{
    pStk->iTop = -1;
}

//-----------------------------------------------------------------------------
//
//    Funcao: 
//              CreateStack
//
//    Proposito: 
//              Criar a pilha para gerenciamento de dados
//
//    Parametros:
//              int iStackSize - total de dados que podem ser armazenados.
//              int iDataSize  - tamanho do tipo de dado armazenado.
//
//    Retorno:
//              pStk - ponteiro para a pilha recem criada.
//              NULL - falhou, naoh foi possivel criar a pilha.
//
//    Descricao:
//              Aloca a pilha, determina os limites minimos e maximo na 
//              mesma, e certifica que estah vazia.
//
//    Historico:                                                     
//               Data            Autor            Comentario
//               18/09/2008      Roberto Bauer    Adaptou de J.Rex e A.Binstok           
//
//-----------------------------------------------------------------------------
SSTACK *CreateStack(int iStackSize, int iDataSize)
{
    SSTACK *pStk;

    // certifica que o tamanho eh valido
    if (iStackSize > 0) {

        // cria a estrutura de gerenciamento da pilha
        pStk = (SSTACK *) malloc(sizeof(SSTACK));
        if (pStk == NULL) {        // verifica se conseguiu alocar
            return (NULL);         // do contrario indica o erro
        }
        // salva as informacoes de  tamanho da pilha
        pStk->iStackSize = iStackSize;
        pStk->iDataSize = iDataSize;

        // cria a area de dados da pilha
        pStk->pBase = malloc(iStackSize * iDataSize);
        if (pStk->pBase == NULL) {  // verifica se conseguiu alocar
            free(pStk);             // tendo falhado, 
                                    // libera o previamente alocado
            return (NULL);          // do contrario indica o erro
        }

        // determina o minimo e maximo da pilha
        pStk->iMaxStack = iStackSize - 1;

        // inicia a pilha
        ClearStack(pStk);

        // retorna o ponteiro 
        return (pStk);
    }
    else {
        return (NULL);              // do contrario indica o erro
    }
}

//-----------------------------------------------------------------------------
//
//    Funcao: 
//              PopData
//
//    Proposito: 
//              Desempilhar os dados armazenados
//
//    Parametros:
//              SSTACK *pStk - estrutura com as variaveis de controle da pilha
//              void *pData  - aponta para os dados que foi desempilhado
//
//    Retorno:
//              TRUE  - retornou dados validos em pData
//              FALSE - a pilha estah vazia
//
//    Descricao:
//              Recupera um dado da pilha. Se a pilha naoh estiver vazia,
//              copia a informacaoh e decrementa o topo da pilha.
//
//    Historico:                                                     
//               Data            Autor            Comentario
//               18/09/2008      Roberto Bauer    Adaptou de J.Rex e A.Binstok
//
//-----------------------------------------------------------------------------
BOOL PopData(SSTACK *pStk, void *pData)
{
    BYTE *pPos;

    // se pilha vazia
    if (pStk->iTop == -1)
        return (FALSE);        // retorna erro

    // calcula o endereco da posicaoh do topo da pilha
    pPos = (BYTE *) pStk->pBase;
    pPos += (pStk->iTop * pStk->iDataSize);

    // copia a informacaoh do topo da pilha, para pData
    memmove(pData, pPos, pStk->iDataSize);

    --pStk->iTop;

    return (TRUE);
}

//-----------------------------------------------------------------------------
//
//    Funcao: 
//              PushData
//
//    Proposito: 
//              Empilha o dado recebido
//
//    Parametros:
//              SSTACK *pStk - estrutura com as variaveis de controle da pilha
//              void *pData  - aponta para o dado vai ser armazenado
//
//    Retorno:
//              TRUE  - armazenou os dados na pilha
//              FALSE - a pilha estah cheia
//
//    Descricao:
//              Salva dado na pilha. Se a pilha naoh estiver cheia,
//              avancah o topo da lista para o proximo ponto e copia
//              a informacaoh do novo dado.
//
//    Historico:                                                     
//               Data            Autor            Comentario
//               18/09/2008      Roberto Bauer    Adaptou de J.Rex e A.Binstok           
//
//-----------------------------------------------------------------------------
BOOL PushData(SSTACK *pStk, void *pData)
{
    BYTE *pPos;

    // pilha estah lotada
    if (pStk->iTop == pStk->iMaxStack) 
        return (FALSE); // indica que naoh ha espaco 
                        // disponivel p/ novos dados

    ++pStk->iTop;

    // calcula o endereco da posicaoh do topo da pilha
    pPos = (BYTE *) pStk->pBase;
    pPos += (pStk->iTop * pStk->iDataSize);
    memmove(pPos, pData, pStk->iDataSize);

    return (TRUE);
}

//-----------------------------------------------------------------------------
//
//    Funcao: 
//              ViewData
//
//    Proposito:
//              Ver um elemento da pilha
//
//    Parametros:
//              SSTACK *pStk - estrutura com as variaveis de controle da pilha.
//              int iPos     - posicaoh desejada em termos de distancia do topo.
//
//    Retorno:
//              NULL - a pilha estah vazia, ou posicao do dado invalida
//              pPos - aponta para a posicaoh desejada dentro da pilha
//
//    Descricao:
//              Ve um elemento dentro da pilha. Eh passado um valor que especifica
//              a posicaoh do enderecoh na pilha em relacaoh ao topo, 0 eh o topo
//              1 o elemento abaixo do topo e 2 eh o elemento seguinte a este.
//              Se um valor invalido for passado, a funcaoh retorna NULL, do 
//              contrario retorna um ponteiro para posicaoh desejada.
//
//    Historico:                                                     
//               Data            Autor            Comentario
//               18/09/2008      Roberto Bauer    Adaptou de J.Rex e A.Binstok           
//
//-----------------------------------------------------------------------------
void *ViewData(SSTACK *pStk, int iPos)
{
    BYTE *pPos;

    if (pStk->iTop == -1)
        return (NULL);

    if ((pStk->iTop - iPos) < 0)
        return (NULL);

    // calcula o endereco da posicaoh do topo da pilha
    pPos = (BYTE *) pStk->pBase;
    pPos += ((pStk->iTop - iPos) * pStk->iDataSize);

    return ((void *)(pPos));
}

//-----------------------------------------------------------------------------
//
//    Funcao: 
//              DestroyStack
//
//    Proposito: 
//              Destroi a pilha
//
//    Parametros:
//              SSTACK *pStk - estrutura com as variaveis de controle da pilha
//
//    Retorno:
//              TRUE  - desalocou com sucesso a memoria usada pela pilha
//              FALSE - ponteiro de controle de pilha invalido 
//
//    Descricao:
//              Desaloca a pilha.
//
//    Historico:                                                     
//              Data            Autor            Comentario
//              05/11/2008      Roberto Bauer    Criou
//
//-----------------------------------------------------------------------------
BOOL DestroyStack(SSTACK *pStk)
{
    if (pStk->pBase != NULL)
    {
        free(pStk->pBase);
        if (pStk != NULL)
        {
            free(pStk);
            return TRUE;
        }
    }

    return FALSE;
}

//-----------------------------------------------------------------------------
//
//    Funcao: 
//              ViewTopData
//
//    Proposito: 
//              Consultar o topo da pilha
//
//    Parametros:
//              SSTACK *pStk - estrutura com as variaveis de controle da pilha.
//
//    Retorno:
//              NULL - a pilha estah vazia
//              pPos - aponta para a posicaoh desejada dentro da pilha
//
//    Descricao:
//              Retorna um ponteiro para posicaoh do topo.
//
//    Historico:                                                     
//               Data            Autor            Comentario
//               26/04/2013      Roberto Bauer    criou
//
//-----------------------------------------------------------------------------
void *ViewTopData(SSTACK *pStk)
{
    BYTE *pPos;

    if (pStk->iTop == -1)
        return (NULL);

    // calcula o endereco da posicaoh do topo da pilha
    pPos = (BYTE *) pStk->pBase;
    pPos += (pStk->iTop * pStk->iDataSize);

    return ((void *)(pPos));
}
