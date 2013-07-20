/******************************************************************************
 * Modulo Fonte..........: aplic.c                                            *
 *                                                                            *
 * Data de criacao.......: 12/07/2013                                         *
 * Desenvolvedor.........: Henrique Hornos                                    *
 *                                                                            *
 *----------------------------------------------------------------------------*
 * Descricao.............:                                                    *
 * contem todas as funcoes e regras de negocio da aplicacao,                  *
 * modulo compartilhado entre todas as plataformas                            *
 *----------------------------------------------------------------------------*
 * Alteracoes............:                                                    *
 * Desenvolvedor.........:                                                    *
 * Data da alteracao.....:                                                    *
 *                                                                            *
 * "aplic.c, v0.0.1 2013/07/12 12:53:00 id: "Henrique_H1"                     *
 *                                                                            *
 ******************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"geral.h"
#include	"aplic.h"
#include	"app_sys.h"
#include	"porting.h"

/* prototipos locais */
STAT	initEnvironment ( void );
STAT	menu			( void );

void	mostraMenu		( void );

/******************************************************************************
 * Funcao: main	                                                              *
 *         Inicio da aplicacao                                                *
 *                                                                            *
 * Retornos: 0                                                                *
 ******************************************************************************/
int
#ifdef ING_TELIUM
main_telium ( void )
#else
main ( void )
#endif
{
	if (initEnvironment ())							/* Inicia dispositivos, variaveis e estruturas do sistema */
	{
		clearDisplay ();
		printfPOS ("ERRO FATAL :("	, 4, 4);
		printfPOS ("Reinicie POS"	, 4, 6);

		while (TRUE) 
		{
			beepPOS (BEEP_ERRO);
			waitTime (5000);						/* 5 segundos */
		}
	}

	while (menu () == POS_SUCESS);					/* Menu aplicacao */

	clearDisplay ();
	printfPOS ("ADEUS MUNDO CRUEL!!!", 0, 4);

	while (TRUE) 
	{
		beepPOS (BEEP_ERRO);
		waitTime (5000);							/* 5 segundos */
	}
}

/******************************************************************************
 * Funcao: initEnvironment                                                    *
 *         Inicia todos os dispositivos e popula estruturas minimas           *
 *         para funcionamento da solucao                                      *
 *                                                                            *
 * Retornos: POS_ERRO                                                         *
 *           POS_SUCESS                                                       *
 ******************************************************************************/
STAT
initEnvironment ( void )
{
	if (resetConsole ())
	{
		clearDisplay ();
		debugH ("FALHOU CONSOLE\n");
		return POS_ERRO;
	}

	clearDisplay ();
	printfPOS (" Teste SDLC VR: 0.01 ", 0, 3);
	printfPOS ("     Aguarde...      ", 0, 5);

	if (resetPrint ())
	{
		clearDisplay ();
		debugH ("FALHOU Impressora\n");
		return POS_ERRO;
	}

	if (loadParm ())
	{
		clearDisplay ();
		debugH ("FALHOU LOAD PARM\n");
		return POS_ERRO;
	}

	if (ResetDispComm ())
	{
		clearDisplay ();
		debugH ("FALHOU RESET MODEM\n");
		return POS_ERRO;
	}

	return POS_SUCESS;
}

/******************************************************************************
 * Funcao: menu                                                               *
 *         Apresenta interface de interacao com o usuario                     *
 *                                                                            *
 * Retornos: POS_ERRO                                                         *
 *           POS_SUCESS                                                       *
 ******************************************************************************/
STAT
menu ( void )
{
	STAT	sts = 0;

	char	key;

	ResetDispComm ();								/* Modem Conectado? Desconecta e reinicia modem */

	mostraMenu ();									/* apresenta menu */

	key = readKeyPos ();							/* Obtem um caracter do teclado */

	if (key == '1')
	{
		debugH ("VOU NO TRANSACIONA\n");
		sts = transaciona ();
		debugH ("transaciona sts:[%s]\n", (sts == POS_SUCESS)?"SUCESSO TRN":"ERRO TRN");

		switch (sts)
		{
			case POS_SUCESS	:	beepPOS (BEEP_DEFAULT);	break;
			case POS_ERRO	:	
			case POS_CANCEL	:	beepPOS (BEEP_ERRO);	break;
		}
		printfPOS	(getStatCom (), 1, 4);
		waitTime	(3000);							/* 3 segundos */
	}

	waitTime (200);									/* 200 milisegundos */
	return POS_SUCESS;
}

void
mostraMenu ( void )
{
	clearDisplay ();
	printfPOS (">>>  TESTE  SDLC  <<<", 1, 1);
	printfPOS (" 1 - INICIAR TESTE   ", 1, 4);
}
