//**************************************************************//
//  Arquivo: STACKDRV.C                                         //
//  Autor: Roberto Bauer                                        //
//  Observacoes:                                                //
//              Controlador da aplicacaoh de exemplo de pilhas. //
//**************************************************************//


#define __STACKDRV_C__

#include "main.h"

LOCAL BOOL Prcd(char, char);
LOCAL int VerifyStrInfix(int, char *[], char **);
LOCAL int MakeStrPostfix(char **, char **);
LOCAL int CalcPostfix(char **, float *);
LOCAL int CreateTableArg(char *, char ***);
LOCAL BOOL VerifyBrackets(char *);


//-----------------------------------------------------------------------------
//
//    Funcao: 
//              CalcInfix
//
//    Proposito: 
//              Verifica e calcula a expressão de entrada
//
//    Parametros:
//              int argc     - total de elementos da linha de comando
//				char *argv[] - parametros da linha de comando
//
//    Retorno:
//				EXIT_SUCESS  - analisou uma expressao valida
//				EXIT_FAILURE - recebeu uma expressaoh invalida, ou falta de recursos
//
//    Descricao:
//              Verifica a expressaoh infixa de entrada
//              Converte de infixa para pos-fixa
//              Calcula a expressaoh pos-fixa
//
//    Historico:                                                     
//               Data            Autor            Comentario
//               30/03/2013      Roberto Bauer    Criou
//
//-----------------------------------------------------------------------------
int CalcInfix(int argc, char *argv[])
{
	char *strInfix;
	char *strPostfix;
	float fRes;
	int i;

    if (argc == 1)
    {
        fprintf(stderr, "Uso: infixa <expressaoh>\n");
        return (EXIT_FAILURE);
    }

	i = VerifyStrInfix(argc, argv, &strInfix);

	if (i == EXIT_SUCCESS)
	{
		printf("%s\n", strInfix);
		i = MakeStrPostfix(&strInfix, &strPostfix);

		if (i == EXIT_SUCCESS)
		{
			printf("%s\n", strPostfix);
			i = CalcPostfix(&strPostfix, &fRes);

			if (i == EXIT_SUCCESS)
			{
				printf("%f\n", fRes);
			}
		}
	}

	return (i);
}

//-----------------------------------------------------------------------------
//
//    Funcao: 
//              VerifyStrInfix
//
//    Proposito: 
//              Verifica se a entrada eh valida e formata a string
//
//    Parametros:
//              int argc        - total de elementos da linha de comando
//				char *argv[]    - parametros da linha de comando
//				char **strInfix - ponteiro para um ponteiro de string 
//
//    Retorno:
//				EXIT_SUCESS  - analisou uma expressao valida, montou string infixa
//					*strInfix - ponteiro para area alocada contendo a expressaoh infixa 
//				EXIT_FAILURE - recebeu uma expressaoh invalida, ou falta de recursos
//
//    Descricao:
//				Recebe-se uma expressaoh infixa digitada pelo usuario atraves da linha de 
//			comando, antes de verificar a validade, monta-se uma string semelhante ao que foi 
//			inserido, a quantidade de strings apontadas por argv, se referem ao total de 
//			ESPACOSH EM BRANCO que separam a string, para facilitar a verificação, serah montada
//			montada uma string aonde os operadores e operandos estaraoh separados por espacoh, 
//			para validacaoh da string, seguir as regras:
//				1 - apenas serah aceito parenteses, digitos e operadores como simbolo
//				2 - operadores '+-/*$' precisam estar entre operandos '01234567689'
//				3 - parenteses '()' tem que estar sempre em pares respeitando a posicaoh
//				4 - operando naoh pode ser seguido de outro operando
//				5 - operadores '+-/*$' naoh podem estar a esquerda de ')'
//				6 - operadores '+-/*$' naoh podem estar a direita de '('
//
//    Historico:                                                     
//               Data            Autor            Comentario
//               30/03/2013      Roberto Bauer    Criou
//
//-----------------------------------------------------------------------------
LOCAL int VerifyStrInfix(int argc, char *argv[], char **strInfix)
{
	int iCnt;
	unsigned int jCnt;
	size_t iSizStr;
	char *ptr;

	for (iCnt = 1, iSizStr = 0; iCnt < argc; ++iCnt)
	{
		iSizStr += strlen(argv[iCnt]);
	}

	if ((*strInfix = malloc(iSizStr * 2 + 1)) == NULL)
	{
        return (EXIT_FAILURE);
	}

	ptr = *strInfix;

	for (iCnt = 1; iCnt < argc; iCnt++)
	{
		for (jCnt = 0; jCnt < strlen(argv[iCnt]); jCnt++)
		{
			*ptr++ = argv[iCnt][jCnt];
			if (!( IsOperator(argv[iCnt][jCnt]) || IsBracket(argv[iCnt][jCnt]) || IsDigit(argv[iCnt][jCnt]) ))
			{
				fprintf(stderr, "expressaoh invalidah, simbolo naoh suportado\n");
				return (EXIT_FAILURE);
			}

			if ((IsOperator(argv[iCnt][jCnt])) ||
				(IsBracket(argv[iCnt][jCnt])) ||
				(IsDigit(argv[iCnt][jCnt]) && IsOperator(argv[iCnt][jCnt+1])) ||
				(IsDigit(argv[iCnt][jCnt]) && IsBracket(argv[iCnt][jCnt+1])) ||
				(IsDigit(argv[iCnt][jCnt]) && argv[iCnt][jCnt+1] == '\0'))
			{
				*ptr++ = ' ';
			}
		}
	}
	ptr--;
	*ptr = '\0';

	ptr = *strInfix;
	while (*ptr)
	{
		if (*ptr == ' ') 
		{
			if ((*(ptr-1) == '(' && IsOperator(*(ptr+1))) ||
				(IsOperator(*(ptr-1)) && *(ptr+1) == ')') ||
				(IsOperator(*(ptr-1)) && IsOperator(*(ptr+1))) ||
				(IsDigit(*(ptr-1)) && IsDigit(*(ptr+1))))
			{
				*(ptr+2) = '\0';
				fprintf(stderr, "%s <- expressaoh invalidah\n", *strInfix);
				return (EXIT_FAILURE);
			}
		}
		ptr++;
	}

	if (VerifyBrackets(*strInfix))
	{
		return (EXIT_SUCCESS);
	}
	else
	{
		fprintf(stderr, "%s <- expressaoh invalidah", *strInfix);
		return (EXIT_FAILURE);
	}
}

//-----------------------------------------------------------------------------
//
//    Funcao: 
//              MakeStrPostfix
//
//    Proposito: 
//              Converte uma expressaoh infixa em pos-fixa
//
//    Parametros:
//				char **strInfix - string com expressaoh infixa
//				char **strPostfix - ponteiro para um ponteiro de string
//
//    Retorno:
//				EXIT_SUCESS  - montou string posfixa
//					*strPostfix - ponteiro para area alocada contendo a expressaoh infixa 
//				EXIT_FAILURE - falta de recursos
//
//    Descricao:
//				A ordem entre os operandos não se altera, apenas os operadores 
//			precisam ser reorganizados, empilha-se o operador para compara-lo 
//			com a proximo a ser encontrado, sendo o topo da pilha de maior ou 
//			igual precedencia, desempilha-se e o inclui na expressaoh, empilhando 
//			o de menor, do contrario, mantem o topo e empilha o novo operador sobre 
//			este, no final, os que estiverem na pilha estaram ordenados, bastando 
//			esvaziar a pilha e inserir na expressaoh.
//
//    Historico:                                                     
//               Data            Autor            Comentario
//               30/03/2013      Roberto Bauer    Criou
//
//-----------------------------------------------------------------------------
LOCAL int MakeStrPostfix(char **strInfix, char **strPostfix)
{
    int argc;
    char **argv;
    int iCnt;
    SSTACK *stkIn;
    size_t iSizStr;
    char *ptrP;
    SDATAIN sdEl;
    SDATAIN *sdTop;


    iSizStr = strlen(*strInfix);
    if ((*strPostfix = malloc(iSizStr + 1)) == NULL)
    {
        return (EXIT_FAILURE);
    }

    ptrP = *strPostfix;
    *ptrP = '\0';

    argc = CreateTableArg(*strInfix, &argv);

    stkIn = inCreateStack(argc);       
    if (stkIn == NULL)
    {
        return (EXIT_FAILURE);
    }

    for (iCnt = 0; iCnt < argc; ++iCnt)
    {
        if ( IsNumber(argv[iCnt]) )
        {
            if (*ptrP == '\0')
                sprintf(ptrP, "%s", argv[iCnt]);
            else
                sprintf(ptrP, "%s %s", ptrP, argv[iCnt]);
        }
        else //IsOperator
        {
            sdTop = inViewTopData(stkIn);

            if (sdTop == NULL) //IsEmpty
            {
                inPushData(stkIn, &argv[iCnt][0]);
            }
            else
            {
                if (argv[iCnt][0] == ')')
                {
                    while (inPopData(stkIn, &sdEl) && sdEl.cOpr != '(')
                    {
                        sprintf(ptrP, "%s %c", ptrP, sdEl.cOpr);
                    }
                }
                else if (argv[iCnt][0] == '(')
                {
				    inPushData(stkIn, &argv[iCnt][0]);
                }
                else if (Prcd(sdTop->cOpr, argv[iCnt][0]))
                {
                    inPopData(stkIn, &sdEl);
                    sprintf(ptrP, "%s %c", ptrP, sdEl.cOpr);
                    inPushData(stkIn, &argv[iCnt][0]);
                }
                else
                {
                    inPushData(stkIn, &argv[iCnt][0]);
                }				
            }
        }
    }

    while (inPopData(stkIn, &sdEl) && sdEl.cOpr != '(')
    {
        sprintf(ptrP, "%s %c", ptrP, sdEl.cOpr);
    }

    DestroyStack(stkIn);
    free(argv);

    return (EXIT_SUCCESS);
}

//-----------------------------------------------------------------------------
//
//    Funcao: 
//              CalcPostfix
//
//    Proposito: 
//              Calcula uma expressaoh pos-fixa
//
//    Parametros:
//				char **strPostfix - string com expressaoh pos-fixa
//				float *fRes	- ponteiro para carregar a resposta
//
//    Retorno:
//				EXIT_SUCESS  - avaliou a expressao
//					fRes - resultado da operação
//					*strPostfix - ponteiro para area alocada contendo a expressaoh infixa 
//				EXIT_FAILURE - falta de recursos
//
//    Descricao:
//
//    Historico:                                                     
//               Data            Autor            Comentario
//               30/03/2013      Roberto Bauer    Criou
//
//-----------------------------------------------------------------------------
LOCAL int CalcPostfix(char **strPostfix, float *fRes)
{
	int argc;
	char **argv;
    SDATAPOST sdEl;
    SSTACK *stkPost;
    int iCnt;

	argc = CreateTableArg(*strPostfix, &argv);

	stkPost = postCreateStack(argc);
    if (stkPost == NULL)
    {
        return (EXIT_FAILURE);
    }

    for (iCnt = 0; iCnt < argc; ++iCnt)
    {
        if ( IsNumber(argv[iCnt]) )
        {
            sdEl.fVal = (float) Atoi(argv[iCnt]);
            if (!postPushData(stkPost, &sdEl)) {
                return (EXIT_FAILURE);
            }
        }
        else if ( IsOperator(argv[iCnt][0]) )
        {
            float fVal1, fVal2;

            postPopData(stkPost, &fVal2);
            postPopData(stkPost, &fVal1);

            switch (argv[iCnt][0])
            {
                case '+':	
                    sdEl.fVal = fVal1 + fVal2;
                    break;

                case '-':	
                    sdEl.fVal = fVal1 - fVal2;
                    break;

                case '/':	
                    sdEl.fVal = fVal1 / fVal2;
                    break;

                case '*':	
                    sdEl.fVal = fVal1 * fVal2;
                    break;

                case '$':	
                    sdEl.fVal = Expon(fVal1, (int)fVal2);
                    break;
            }
            postPushData(stkPost, &sdEl);
        }
    }

	postPopData(stkPost, fRes);

	DestroyStack(stkPost);
	free(argv);


    return (EXIT_SUCCESS);
}



//-----------------------------------------------------------------------------
//
//    Funcao: 
//              ConvPrcd
//
//    Proposito: 
//              Devolve o valor do simbolo do operador na lista de precedencia
//
//    Parametros:
//				char Opr - operador
//
//    Retorno:
//				valor de 0 a 3 baseado no operador
//
//    Descricao:
//			Converte o caracter contendo o simbolo do operador em um valor
//			numerico que representa sua precedencia
//
//    Historico:                                                     
//               Data            Autor            Comentario
//               30/03/2013      Roberto Bauer    Criou
//
//-----------------------------------------------------------------------------
LOCAL int ConvPrcd(char cOpr)
{
	switch (cOpr)
	{
		default:
		case '(':	return 0;
		case '-':
		case '+':	return 1;
		case '/':
		case '*':	return 2;
		case '$':	return 3;
	}
}

//-----------------------------------------------------------------------------
//
//    Funcao: 
//              Prcd
//
//    Proposito: 
//				Comparar a precedencia entre operadores
//				Funcaoh de apoio a rotina MakeStrPostfix
//              Devolve o valor do simbolo do operador na lista de precedencia
//
//    Parametros:
//				char cOpr1 - simbolo do operador 
//				char cOpr2 - simbolo do operador 
//
//    Retorno:
//				TRUE - operador 1 maior ou igual a operador 2
//				FALSE - operador 1 menor que operador 2
//
//    Descricao:
//			Converte o caracter contendo o simbolo do operador em um valor
//			numerico que representa sua precedencia
//
//    Historico:                                                     
//               Data            Autor            Comentario
//               30/03/2013      Roberto Bauer    Criou
//
//-----------------------------------------------------------------------------
LOCAL BOOL Prcd(char cOpr1, char cOpr2)
{
	if (ConvPrcd(cOpr1) >= ConvPrcd(cOpr2))
		return TRUE;
	else
		return FALSE;
}

//-----------------------------------------------------------------------------
//
//    Funcao: 
//              CreateTableArg
//
//    Proposito: 
//				Cria uma tabela que aponta para cada argumento na string
//
//    Parametros:
//				char *str - string com a expressao a ser utilizada na tabela
//				char ***argv - ponteiro para tabela de argumentos a ser criada
//
//    Retorno:
//				argc - total de argumentos que a tabela possui
//				**argv - tabela com apontamentos dos argumentos da expressao
//
//    Descricao:
//				as funcoes para converter infixa em pos-fixa e calcula pos-fixa, recebem 
//			uma string com cada argumento separado por um espacoh, para facilitar 
//			o processo de percorrer a string, cria-se um vetor de ponteiro, onde cada
//			posicaoh aponta para um argumento.
//
//    Historico:                                                     
//               Data            Autor            Comentario
//               30/03/2013      Roberto Bauer    Criou
//
//-----------------------------------------------------------------------------
LOCAL int CreateTableArg(char *str, char ***myargv)
{
	char *pBegin;
	char *pEnd;
	int argc = 0;
	char *pTable;

	pBegin = str;
	while (*pBegin) {
		if (*pBegin++ == ' ')
			argc++;
	}
	argc++;
	
	// save initial position with pTable
	pTable = *myargv = malloc(argc * sizeof(char *));

	if (*myargv == NULL)
	{
        return 0;
	}

	pEnd = pBegin = str;
	while (*pEnd) 
	{
		if (*pEnd == ' ')
		{
			*pEnd = '\0';
			**myargv = pBegin;
			(*myargv)++;
			pBegin = pEnd + 1;
		}
		pEnd++;
	}
	**myargv = pBegin;

	*myargv = pTable;

	return argc;
}

//-----------------------------------------------------------------------------
//
//    Funcao: 
//              VerifyBrackets
//
//    Proposito: 
//				Verifica a correta ordem e quantidade de parenteses
//
//    Parametros:
//				char *cBuf - aponta para a expressao a ser avaliada
//
//    Retorno:
//				TRUE - sequencia correta de parenteses 
//				FALSE - quantidade ou ordem incorreta
//
//    Descricao:
//				Empilha sempre o '(', e o desempilha quando encontra ')', 
//				a indicacaoh de erro ocorre quando temos um ')' e a pilha
//				se encontra vazia, ou ao termino da expressao, a pilha 
//				ainda tem valores armazenados.
//
//    Historico:                                                     
//               Data            Autor            Comentario
//               30/03/2013      Roberto Bauer    Criou
//
//-----------------------------------------------------------------------------
LOCAL BOOL VerifyBrackets(char *cBuf)
{
    SDATA sdEl;
    SSTACK *stk;
    char c;
    int i;
	BOOL bOk = TRUE;

    stk = brcCreateStack((int)strlen(cBuf));
    if (stk == NULL)
    {
        return (FALSE);
    }


    for (i = 0; cBuf[i] != '\0'; ++i)
    {
        switch (c = cBuf[i])
        {
            case '(':
                sdEl.cOpener = c;
                if (!brcPushData(stk, &sdEl))
                {
					bOk = FALSE;
                }
                break;

            case ')':
                if (!brcPopData(stk, &sdEl))
                {
					bOk = FALSE;
                }
                else 
                {
                    if (c == ')' && sdEl.cOpener != '(')
                    {
						bOk = FALSE;
                    }
                }
                break;

            default:
                continue;
        }
    }


    if (brcViewData(stk, 0) != NULL) 
    {
        while (brcPopData(stk, &sdEl) != 0)
        {
			bOk = FALSE;
        }
    }

	DestroyStack(stk);

    return (bOk);
}

