/******************************************************************************
 * Modulo Fonte..........: app_sys.h                                          *
 *                                                                            *
 * Data de criacao.......: 12/07/2013                                         *
 * Desenvolvedor.........: Henrique Hornos                                    *
 *                                                                            *
 *----------------------------------------------------------------------------*
 * Descricao.............:                                                    *
 * prototipos de funcoes do modulo app_sys.c                                  *
 *----------------------------------------------------------------------------*
 * Alteracoes............:                                                    *
 * Desenvolvedor.........:                                                    *
 * Data da alteracao.....:                                                    *
 *                                                                            *
 * "app_sys.h, v0.0.1 2013/07/11 20:45:00 id: "Henrique_H1"                   *
 *                                                                            *
 ******************************************************************************/
#ifndef APP_SYS_H
#define APP_SYS_H
#endif

/******************************************************************************
 * Nome:   PARM                                                               *
 *         Aqui contem estrutura com todos os parametros necessarios para a   *
 *         execucao do aplicativo, cada parametro estatico novo devera ser    *
 *         incluso aqui...                                                    *
 ******************************************************************************/

struct PARM
{
	char pabx		[12];
	char telefone	[24];
};

#define		PARM_PABX		"#PABX"
#define		PARM_TELEFONE	"#TELEFONE"

/* Prototipos */
STAT	ResetDispComm	( void );
STAT	loadParm		( void );
STAT	printfPOS		( char *, int, int );
STAT	transaciona		( void );

char *	getStatCom		( void );

