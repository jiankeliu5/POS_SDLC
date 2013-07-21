/******************************************************************************
 * Modulo Fonte..........: app_sys.c                                          *
 *                                                                            *
 * Data de criacao.......: 12/07/2013                                         *
 * Desenvolvedor.........: Henrique Hornos                                    *
 *                                                                            *
 *----------------------------------------------------------------------------*
 * Descricao.............:                                                    *
 * biblioteca de funcoes responsaveis pelo tratamento de dados para aplicacao *
 * e interacao com os modulos de porting                                      *
 *----------------------------------------------------------------------------*
 * Alteracoes............:                                                    *
 * Desenvolvedor.........:                                                    *
 * Data da alteracao.....:                                                    *
 *                                                                            *
 * "app_sys.c, v0.0.1 2013/07/12 12:53:00 id: "Henrique_H1"                   *
 *                                                                            *
 ******************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"geral.h"
#include	"app_sys.h"
#include	"porting.h"

char			retCom[24];

struct PARM		parm;										/* estrutura de parametros da aplicacao */

static char		isoBufP1	[1024];							/* buffer de envio de mensagens ISO */
static char		isoBufP2	[1024];							/* buffer de resposta de mensagens ISO */

static int		contIsoBufP1 = 0;							/* contabiliza bytes isoBufP1 */
static int		contIsoBufP2 = 0;							/* contabiliza bytes isoBufP2 */

/******************************************************************************
 * Funcao: ResetDispComm                                                      *
 *         Inicia o dispositivo de comunicacao pretendido                     *
 *                                                                            *
 * Retornos: POS_ERRO                                                         *
 *           POS_SUCESS                                                       *
 ******************************************************************************/
STAT
ResetDispComm ( void )
{
	// AQUI DEVE-SE ESCOLHER A COMUNICACAO NESTE CASO APENAS SDLC
	return resetSDLC ();
}

/******************************************************************************
 * Funcao: discaPos		                                                      *
 *         Inicia discagem pelo dispositivo pretendido (DIAL, TCP)            *
 *                                                                            *
 * Retornos: POS_ERRO                                                         *
 *           POS_SUCESS                                                       *
 ******************************************************************************/
STAT
discaPos ( void )
{
	// AQUI DEVE-SE ESCOLHER A COMUNICACAO NESTE CASO APENAS SDLC

	/* DISCA SDLC */
	if (discaSDLC () == POS_ERRO)
		return POS_ERRO;

	return POS_SUCESS;
}

/******************************************************************************
 * Funcao: makeISO		                                                      *
 *         monta mensagem iso no buffer de p1 (buffer de envio)               *
 *                                                                            *
 * Retornos: POS_CANCEL                                                       *
 *           POS_SUCESS                                                       *
 ******************************************************************************/
STAT
makeISO ( void )
{
	// aqui era para incluir logica da isolib (biblioteca ISO8583)
	// devido ao teste sdlc, apenas copiar mensagem simples, ainda falta definir protocolo junto a equipe.
	sprintf (isoBufP1,"0100A238040120C1844800000000100000443031000712115013005060115013071202115000000000000001376361179999999016=99017001440056200100WT_PO1041000000016557390251306191200000            0760095280474310654001000890C455AB13WTNET01.26.2203600000100000000000000000000000000000002600000000000000000000000000\r");
	contIsoBufP1 = strlen (isoBufP1);
	return POS_SUCESS;
}

/******************************************************************************
 * Funcao: sendISO		                                                      *
 *         transmite mensagem iso contida no buffer de p1                     *
 *                                                                            *
 * Retornos: POS_CANCEL                                                       *
 *           POS_SUCESS                                                       *
 ******************************************************************************/
STAT
sendISO ( void )
{
	STAT	ret;

	// DEVE CONTER LOGICA PARA SELECIONAR TIPO DE COMUNICACAO NESTE CASO APENAS SDLC

	ret = sndSDLC (isoBufP1, contIsoBufP1);

	if (ret != POS_SUCESS)
	{
		strcpy (retCom, " QUEDA CONEXAO SDLC  ");
	}
	return ret;
}

/******************************************************************************
 * Funcao: recvISO		                                                      *
 *         Aguarda o recebimento de mensagem iso no buffer de p2              *
 *                                                                            *
 * Retornos: POS_CANCEL                                                       *
 *           POS_SUCESS                                                       *
 ******************************************************************************/
STAT
recvISO ( void )
{
	STAT	ret;

	// DEVE CONTER LOGICA PARA SELECIONAR TIPO DE COMUNICACAO e adicao protocolo quando houver(OSI por exemplo), NESTE CASO APENAS SDLC

	ret = recvSDLC (isoBufP2, &contIsoBufP2);
	if (ret != POS_SUCESS)
	{
		strcpy (retCom, " QUEDA CONEXAO SDLC  ");
	}
	return ret;
}

/******************************************************************************
 * Funcao: procP2		                                                      *
 *         realiza acoes necessarias com a mensagem ISO recebida              *
 *                                                                            *
 * Retornos: POS_CANCEL                                                       *
 *           POS_SUCESS                                                       *
 ******************************************************************************/
STAT
procP2 ( void )
{
	// TESTE SDLC APENAS MOSTRA DEBUG DO QUE CHEGOU
	isoBufP2[contIsoBufP2] = 0;
	debugH ("recebido:[bytes=%d]\n[%c][%0.2X]\n", contIsoBufP2, isoBufP2[0], isoBufP2[0]);
	return POS_SUCESS;
}

/******************************************************************************
 * Funcao: loadParm                                                           *
 *         Carrega todos os parametros necessarios para funcionamento da      *
 *         aplicacao                                                          *
 *                                                                            *
 * Retornos: POS_ERRO                                                         *
 *           POS_SUCESS                                                       *
 ******************************************************************************/
STAT
loadParm ( void )
{
	int		ret;

	memset (&parm, 0, sizeof (struct PARM));

	ret = get_parm (PARM_PABX,		&parm.pabx[0],		sizeof (parm.pabx)		);	if (ret == POS_ERRO) { return ret; }
	ret = get_parm (PARM_TELEFONE,	&parm.telefone[0],	sizeof (parm.telefone)	);	if (ret == POS_ERRO) { return ret; }

	/* popula estruturas de parametros de comunicacao do porting */
	SaveConfSDLC ( &parm );									/* neste sample,somente SDLC */

	return POS_SUCESS;
}

/******************************************************************************
 * Funcao: printfPOS                                                          *
 *         Apresenta mensagem simples no display do POS                       *
 *                                                                            *
 * *str  : Ponteiro para a mensagem a ser apresentada (considerar '\0')       *
 * col   : coluna do display (eixo X)                                         *
 * lin   : linha  do display (eixo Y)                                         *
 *                                                                            *
 * Retornos:  POS_SUCESS			                                          *
 *			  POS_ERRO                                                        *
 ******************************************************************************/
STAT
printfPOS (char * str, int col, int lin)
{
	if (!str)
		return POS_ERRO;

	if (writeDisp (str, strlen (str), lin, col) <= 0)
		return POS_ERRO;

	return POS_SUCESS;
}

/******************************************************************************
 * Funcao: isConnected                                                        *
 *         verifica se pos esta conectado ao host destino                     *
 *                                                                            *
 * Retornos:  TRUE			                                                  *
 *			  FALSE                                                           *
 ******************************************************************************/
BOOL
isConnected ( void )
{
	STAT ret;
	//aqui seleciona tipo conexao, neste caso apenas SDLC
	while ((ret = chk_sdlc ()) == POS_SUCESS && getStatSDLC () == SDLC_DIALLING);	/* Ainda discando, aguarda */

	if (ret == POS_SUCESS)
		return TRUE;
	else
		return FALSE;
}

/******************************************************************************
 * Funcao: getStatCom                                                         *
 *         verifica statuas atual do dispositivo e retorna mensagem para a    *
 *         aplicacao                                                          *
 *                                                                            *
 * Retornos:  ponteiro para o buffer de mensagen atualizado                   *
 ******************************************************************************/
char *
getStatCom ( void )
{
	//aqui seleciona tipo comunicacao e retorna sts atual do dispositivo para a aplicacao
	switch (getStatSDLC ())
	{
	case SDLC_NO_CARRIER	:	strcpy (retCom, " TEL NAO SINCRONIZA  "); break;
	case SDLC_NO_DIALTONE	:	strcpy (retCom, " POS SEM LINHA TEL.  "); break;
	case SDLC_BUSY_DETECT	:	strcpy (retCom, " TELEFONE OCUPADO    "); break;
	case SDLC_NO_ANSWER		:	strcpy (retCom, " TEL NAO ATENDE      "); break;
	case SDLC_CONNECTED		:	strcpy (retCom, " TRANSACAO OK        "); break;	// substituir posteriormente por outro status para identificacao de envio e recebimento completo
	default					:	strcpy (retCom, " ERRO MODEM POS      "); break;
	}

	return retCom;
}

/******************************************************************************
 * Funcao: transaciona                                                        *
 *         Inicia o processo automatica de conexao, coleta de dados, envio e  *
 *         recebimento                                                        *
 *                                                                            *
 * Retornos: POS_ERRO                                                         *
 *           POS_SUCESS                                                       *
 ******************************************************************************/
STAT
transaciona ( void )
{
	/*  inicializa buffers de envio e recepcao*/
	memset (isoBufP1, 0, sizeof (isoBufP1));
	memset (isoBufP2, 0, sizeof (isoBufP2));

	printfPOS	(" DISCANDO...         ", 1, 4);
	if (discaPos () == POS_ERRO)
	{
		strcpy (retCom, " TEL NAO SINCRONIZA  ");
		debugH ("discaPos=POS_ERRO\n");
		return POS_ERRO;									/* ERRO CONEXAO */
	}

	if (!isConnected ())
	{
		debugH("STATUS_SDLC:[0x%0.4X]\n", getStatCom ());
		return POS_ERRO;
	}

	if (makeISO () == POS_ERRO)								/* monta mensagem no buffer de envio */
	{
		strcpy (retCom, " OPERACAO CANCELADA  ");
		return POS_CANCEL;
	}

	printfPOS	(" TRANSMITINDO...     ", 1, 4);
	if (sendISO () == POS_ERRO)
		return POS_CANCEL;

	printfPOS	(" RECEBENDO...        ", 1, 4);
	if (recvISO () == POS_ERRO)
		return POS_CANCEL;

	printfPOS	(" PROCESSANDO...      ", 1, 4);
	if (procP2 () == POS_ERRO)
		return POS_CANCEL;

	return POS_SUCESS;
}
