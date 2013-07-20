/******************************************************************************
 * Modulo Fonte..........: porting.c                                          *
 *                                                                            *
 * Data de criacao.......: 12/07/2013                                         *
 * Desenvolvedor.........: Henrique Hornos                                    *
 *                                                                            *
 *----------------------------------------------------------------------------*
 * Descricao.............:                                                    *
 * biblioteca que interage com as funcoes de bibliotecas do SO verix V        *
 *----------------------------------------------------------------------------*
 * Alteracoes............:                                                    *
 * Desenvolvedor.........:                                                    *
 * Data da alteracao.....:                                                    *
 *                                                                            *
 * "porting.c, v0.0.1 2013/07/12 12:53:00 id: "Henrique_H1"                   *
 *                                                                            *
 ******************************************************************************/
#include	<stdio.h>
#include	<stdarg.h>
#include	<stdlib.h>
#include	<string.h>
#include    <aclconio.h>


#include    <svc.h>
#include	<printer.h>
#include	<xmodem.h>
#include	<errno.h>

#include	"geral.h"
#include	"app_sys.h"
#include	"porting.h"

/* Globais */
static int				hConsole	= -1;							/* Handle Console */
static int				hPrint		= -1;							/* Handle Impressora interna */
static int				hComModem	= -1;							/* Handle porta serial Modem SDLC */

static STAT				stsSDLC		= 0;							/* Status atual conexao SDLC */
static long				tmoDiscSDLC = 0;							/* Time out tentativa de discagem SDLC */
static long				tmoRespSDLC = 0;							/* Time out de resposta SDLC */

static struct PARM_SDLC	parmSDLC;									/* estrutura que armazena configuracao SDLC */

/******************************************************************************
 * Funcao: resetConsole	                                                      *
 *         "Abre" dispositivos basicos do POS (ex.: display, teclado...)      *
 *                                                                            *
 * Retornos: POS_ERRO                                                         *
 *           POS_SUCESS                                                       *
 ******************************************************************************/
STAT
resetConsole ( void )
{
	/* Abertura disposito */

	if (hConsole >= 0)												/* Console Aberto */
	{
		close (hConsole);											/* fecha */
		hConsole = -1;
	}

	if ((hConsole = open ( DEV_CONSOLE, 0 )) < 0)
		return POS_ERRO;

	return POS_SUCESS;
}

/******************************************************************************
 * Funcao: debugH	                                                          *
 *         Funcao que imprime conteudo de variaveis para auxilio              *
 *         ao desenvolvedor (habilitado quando utilizado define DEBUG_H)      *
 ******************************************************************************/
void
debugH ( char * acFormato, ... )
{
#ifdef DEBUG_H
    char	acMostrar[1024];
    va_list vArgs;

    va_start (vArgs, acFormato);

    vsprintf (acMostrar, acFormato, vArgs);

    va_end (vArgs);

	if (hPrint >= 0)
		p3700_print (hPrint, (uchar *)acMostrar);

	SVC_WAIT (50);
#endif
}

/******************************************************************************
 * Funcao: resetPrint	                                                      *
 *         "Abre" e configura comunicacao impressora interna do POS           *
 *                                                                            *
 * Retornos: POS_ERRO                                                         *
 *           POS_SUCESS                                                       *
 ******************************************************************************/
STAT
resetPrint ( void )
{
	struct Opn_Blk  mblk;											/* estrutura de programacao da porta serial */

	if (hPrint >= 0)												/* Impressora Aberta */
	{
		close (hPrint);												/* fecha */
		hPrint = -1;
	}

	/* Abertura disposito */
	if ((hPrint = open (DEV_COM4, 0)) < 0)
		return POS_ERRO;

	/* configura porta serial */
	memset (&mblk,0,sizeof(mblk));
    mblk.rate = Rt_19200;
    mblk.format = Fmt_A8N1 | Fmt_RTS | Fmt_auto;
    mblk.protocol = P_char_mode;
	set_opn_blk( hPrint, &mblk);

	SVC_WAIT ( 100 );												/* adormece por 100 ms */

    /* Inicializacao da impressora. */
	p3700_init (hPrint, 6);

	SVC_WAIT ( 200 );												/* adormece por 200 ms */

	p3700_select_font (hPrint, 3, 0);								/* seleciona fonte 8x14 X 32 colunas */

	return POS_SUCESS;
}

/******************************************************************************
 * Funcao: beepPOS	                                                          *
 *         Emite sons pelo dispositivo interno do POS                         *
 *                                                                            *
 * *soundType: defines com tipo de som (BEEP_DEFAULT / BEEP_ERRO)			  *
 *                                                                            *
 * Retornos:  POS_SUCESS			                                          *
 ******************************************************************************/
STAT
beepPOS ( char soundType )
{
	if (soundType == BEEP_ERRO)
		error_tone ();
	else
		normal_tone ();

	return POS_SUCESS;
}


/******************************************************************************
 * Funcao: get_parm		                                                      *
 *         "obtem" conteudo de parametros do config.sys POS Verix             *
 *                                                                            *
 * idParm  : ponteiro para nome do parametro                                  *
 * outBuf  : ponteiro para buffer de armazenamento do conteudo do parametro   *
 * szOutBuf: tamanho do buffer de armazenamento                               *
 *                                                                            *
 * Retornos: POS_ERRO                                                         *
 *           POS_SUCESS                                                       *
 ******************************************************************************/
STAT
get_parm ( const char * idParm, char * outBuf, int szOutBuf )
{
	int ret;

	if (!idParm || !outBuf || !szOutBuf)
		return POS_ERRO;

	ret = get_env (idParm, outBuf, szOutBuf);

	if (ret < 0)
		return POS_ERRO;

	outBuf[ret] = '\0';

	return POS_SUCESS;
}

/******************************************************************************
 * Funcao: waitTime		                                                      *
 *         "adormece" pelo tempo determinado em t                             *
 *                                                                            *
 * t       : tempo em milisegundos                                            *
 *                                                                            *
 * Retornos: POS_SUCESS                                                       *
 ******************************************************************************/
STAT
waitTime ( ulong t)
{
	SVC_WAIT (t);
	return POS_SUCESS;
}

/******************************************************************************
 * Funcao: clearDisplay                                                       *
 *         Apaga conteudo do display (nivel porting)                          *
 ******************************************************************************/
void
clearDisplay ( void )
{
	clrscr ();
}

/******************************************************************************
 * Funcao: writeDisp	                                                      *
 *         Apresenta mensagem no display do POS                               *
 *                                                                            *
 * inBuf   : mensagem a ser apresentada                                       *
 * szBuf   : tamanho mensagem                                                 *
 * col     : coluna display                                                   *
 * lin     : linha  display                                                   * 
 * Retornos: >0 = numero de bytes exibidos no display                         *
 ******************************************************************************/
int
writeDisp ( char * inBuf, int szBuf, int col, int lin )
{
	return write_at (inBuf, szBuf, col, lin);
}

/******************************************************************************
 * Funcao: readKeyPos  	                                                      *
 *         "Obtem" tecla digitado no POS                                      *
 *                                                                            *
 * Retornos: tecla digitada de 0 a 9 em ASCII                                 *
 *           0 = nao obteve tecla                                             *
 ******************************************************************************/
char
readKeyPos ( void )
{
	uchar key = 0;

	if (KBHIT())
	{
		key = get_char ();
		debugH ("TECLA OBTIDA:[%c]-[%0.2X]\n", key, key);
	}

	return key;
}

/******************************************************************************
 * Funcao: get_milli_sec  	                                                  *
 *         Retorna o numero de "ticks" de clock decorrido.                    *
 *                                                                            *
 * Retornos: ticks em millisegundos			                                  *
 ******************************************************************************/
ulong
get_milli_sec ( void )
{
        ulong       millitm;
                
		SVC_WAIT ( 10 );
        SVC_TICKS ( 1, (long*)&millitm );

        return ((ulong)(millitm * 1));     /* milezimos */
}

/******************************************************************************
 * Funcao: ATCmdSDLC	                                                      *
 *         envia comando AT ao modem e obtem resposta                         *
 *                                                                            *
 * ind     : indice do comando conforme tabela abaixo ou -1 somente para resp *
 * timeOpt : 0 para todos os ind != -1, timeout para ind == -1	              *
 * Retornos: SDLC_DIALLING                                                    *
 *           SDLC_CONNECTED                                                   *
 *           SDLC_ERRO_MDM                                                    *
 *           SDLC_NO_CARRIER                                                  *
 *           SDLC_NO_DIALTONE                                                 *
 *           SDLC_BUSY_DETECT                                                 *
 *           SDLC_NO_ANSWER                                                   *
 ******************************************************************************/
#define		MAX_CMD_SDLC	8

char	SDLC_CMD[MAX_CMD_SDLC][34]={								/* lista de comandos ATs para SDLC */
				"AT&F",												/* 0 - PRG. MODEM SDLC */
				"ATE0V1",											/* 1 - PRG. MODEM SDLC*/
				"ATW2X4S25=1&D2%C0\\N0+A8E=,,,0",					/* 2 - PRG. MODEM SDLC */
				"AT+MS=v22",										/* 3 - PRG. MODEM SDLC*/
				"AT$F2S17=15",										/* 4 - PRG. MODEM SDLC*/
				"AT+ES=6,,8",										/* 5 - PRG. MODEM SDLC*/
				"AT+ESA=,,,,1",										/* 6 - PRG. MODEM SDLC*/
				"ATDT.....................",				 		/* 7 - Comando de discagem */
};

char	SDLC_T[MAX_CMD_SDLC]={										/* time-out comandos */
				3,													/* 0 - PRG. MODEM SDLC*/
				5,													/* 1 - PRG. MODEM SDLC*/
				5,													/* 2 - PRG. MODEM SDLC*/
				5,													/* 3 - PRG. MODEM SDLC*/
				5,													/* 4 - PRG. MODEM SDLC*/
				5,													/* 5 - PRG. MODEM SDLC*/
				5,													/* 6 - PRG. MODEM SDLC*/
				0,													/* 7 - Comando de discagem */
};

char	SDLC_R[MAX_CMD_SDLC][12]={									/* Respostas esperada */
				"OK", 												/* 0 - PRG. MODEM SDLC*/
				"OK",												/* 1 - PRG. MODEM SDLC*/
				"OK",												/* 2 - PRG. MODEM SDLC*/
				"OK",												/* 3 - PRG. MODEM SDLC*/
				"OK",												/* 4 - PRG. MODEM SDLC*/
				"OK",												/* 5 - PRG. MODEM SDLC*/
				"OK",												/* 6 - PRG. MODEM SDLC */
				"CONNECT",											/* 7 - CONNECTADO */
};

char	SDLC_E[6][12]={												/* Respostas de ERROS possiveis */
				"ERROR",											/* 0 */
				"NO CARRIER",										/* 1 */
				"NO DIALTONE",										/* 2 */
				"BUSY",												/* 3 */
				"NO ANSWER",										/* 4 */
				0,													/* fim lista */
};

STAT
ATCmdSDLC ( int ind , int timeOpt )
{
	char	cmdBuf[64];
	char	respBuf[64];
	char	aux[2];
	int		ct;
	int		nv = 0, idx = 0;
	ulong	timeOut;

	if (ind < -1 && ind >= MAX_CMD_SDLC)
		return SDLC_ERRO_MDM;

	if ( ind != -1 )
	{
		/* envia comando AT para o modem */
		flushMDM ();
		sprintf (cmdBuf, "%s\r", &SDLC_CMD[ind][0]);
		write (hComModem, cmdBuf, strlen (cmdBuf));

		debugH ("cmdEnvMDM:%s\n", cmdBuf);

		if (ind == AT_DISCA_SDLC)
			return POS_SUCESS;
	}

	/* prepara time-out de resposta */
	if (ind == -1)
	{
		if (timeOpt)
			timeOut = (read_ticks ()) + ((ulong)timeOpt * 1000);
		else
			return SDLC_ERRO_MDM;
	}
	else
	{
		timeOut = (read_ticks ()) + ((ulong)SDLC_T[ind] * 1000);
	}

	/* aguarda resposta pelo tempo especificado */

	while (TRUE)
	{
		if ((ct = read (hComModem, aux, 1)) > 0)
		{
			if (*aux == '\n' && nv == 0)
				nv = 1;												/* indica inicio da resposta */

			if (*aux == '\r' && nv == 1)							/* final resposta */
				break;

			if (nv == 1 && idx < (sizeof (respBuf)))				/* cabe no buffer */
			{
				/* verifica se é um caracter valido imprimivel */
				if (*aux < 0x20 || *aux > 0x7E )
					continue;										/* descarta caracter */

				respBuf[idx] = *aux; idx++;							/* copia caracter para buffer de resp*/
			} 
		}
		
		if ((ct < 0))												/* erro recepcao ou timeout... bye bye... */
		{
			debugH ("ERRO RECPCAO AT MDM\n");
			return SDLC_ERRO_MDM;
		}
		
		if (read_ticks () > timeOut)
		{
			debugH ("ERRO RECPCAO timeout\n");
			return SDLC_ATCMD_TMO;
		}
	}


	respBuf[idx] = 0;

	if (ind == -1)
	{
		debugH ("comandoAT->[%s]\n", &SDLC_CMD[AT_DISCA_SDLC][0]);
		debugH ("msgEsp[%s],tam[%d]\n", &SDLC_R[AT_DISCA_SDLC][0], strlen (&SDLC_R[AT_DISCA_SDLC][0]));
	}
	else
	{
		debugH ("comandoAT->[%s]\n", &SDLC_CMD[ind][0]);
		debugH ("msgEsp[%s], ind[%d], tam[%d]\n", &SDLC_R[ind][0], ind, strlen (&SDLC_R[ind][0]));
	}
	debugH ("respCMDAT<-[%s]\n", respBuf);

	/* verifica se eh uma resposta positiva */
	if (ind == -1)													/* discagem */
	{
		if  (memcmp (&SDLC_R[AT_DISCA_SDLC][0], respBuf, strlen (&SDLC_R[AT_DISCA_SDLC][0])) == 0)
		{
			debugH ("RESP OK\n");
			return SDLC_CONNECTED;
		}
	}
	else
	{
		if  (memcmp (&SDLC_R[ind][0], respBuf, strlen (&SDLC_R[ind][0])) == 0)
		{
			debugH ("RESP OK\n");
			return POS_SUCESS;
		}
	}

	/* procura resposta negativa na tabela, se nao encontrar, generico nele */
	for (ct = 0; SDLC_E[ct][0] != 0; ct++)
	{
		if (memcmp (&SDLC_E[ct][0], respBuf, strlen (&SDLC_E[ct][0])) == 0)
		{
			switch (ct)
			{
			case 1	:	return SDLC_NO_CARRIER;
			case 2	:	return SDLC_NO_DIALTONE;
			case 3	:	return SDLC_BUSY_DETECT;
			case 4	:	return SDLC_NO_ANSWER;
			case 0	:	
			default	:	return SDLC_ERRO_MDM;
			}
		}
	}
	return SDLC_ERRO_MDM;
}

/******************************************************************************
 * Funcao: getStatSDLC	                                                      *
 *         retorna o Status Atual do modem SDLC                               *
 *                                                                            *
 * Retornos: Vide porting.h                                                   *
 ******************************************************************************/
STAT
getStatSDLC ( void )
{
	return stsSDLC;
}

/******************************************************************************
 * Funcao: SaveConfSDLC	                                                      *
 *         Popla estrutura de configuracao sdlc do porting                    *
 *                                                                            *
 * Retornos: POS_ERRO                                                         *
 *           POS_SUCESS                                                       *
 ******************************************************************************/
STAT
SaveConfSDLC ( struct PARM * pPARM )
{
	if (!pPARM)
		return POS_ERRO;

	memset (&parmSDLC, 0, sizeof (struct PARM_SDLC));

	memcpy (parmSDLC.pabx, pPARM->pabx,		sizeof (parmSDLC.pabx));
	memcpy (parmSDLC.fone, pPARM->telefone,	sizeof (parmSDLC.fone));

	parmSDLC.pulso		= 'T';
	parmSDLC.ntry		= 3;
	parmSDLC.tmo_idle	= 90;

	return POS_SUCESS;
}

/******************************************************************************
 * Funcao: resetSDLC	                                                      *
 *         "Abre" e configura modem SDLC                                      *
 *                                                                            *
 * Retornos: POS_ERRO                                                         *
 *           POS_SUCESS                                                       *
 ******************************************************************************/
STAT
resetSDLC ( void )
{
	int				i;
	char			aux[16];
	struct Opn_Blk	mblk;

	if (stsSDLC == SDLC_RESETED) return POS_SUCESS;					/* Se dispositivo resetado apenas retorna */

	if (hComModem >= 0)												/* Com modem Aberta */
	{
		get_port_status (hComModem, aux);							/* Obtem status do modem */

		if ((aux[3] & 0x08) != 0)									/* Portadora presente! */
		{
			aux[0] = 0;												/* derruba DTR & RTS */
			set_serial_lines ( hComModem, aux );
		}

		xmdm_hangup (hComModem, -1, 0);								/* fecha antes de programar */
		xmdm_close	(hComModem,  0, 0);

		hComModem = -1;
		SVC_WAIT ( 100 );
	}
	hComModem = open (DEV_COM3, 0);									/* abertura */
	debugH ("handle Modem:[%d]\n", hComModem);

	aux[0] = 0;														/* derruba DTR & RTS */
	set_serial_lines ( hComModem, aux );

	/*
	 * Define: Baud Rate + Paridade + protocolo
	 */

 	mblk.rate = Rt_19200;
	mblk.format = Fmt_A7E1 | Fmt_DTR;
	mblk.protocol = P_char_mode;

	set_opn_blk ( hComModem, &mblk );

	for (i = AT_INIT_SDLC; i <= AT_FIN_SDLC; i++)
	{
		if (ATCmdSDLC (i, 0) != POS_SUCESS)
		{
			debugH ("ERRO ATCMD\n", hComModem);
			return POS_ERRO;
		}
	}

	stsSDLC = SDLC_RESETED;

	return POS_SUCESS;
}

/******************************************************************************
 * Funcao: discaSDLC	                                                      *
 *         "Inicia" discagem SDLC			                                  *
 *                                                                            *
 * Retornos: POS_ERRO                                                         *
 *           POS_SUCESS                                                       *
 ******************************************************************************/
STAT
discaSDLC ( void )
{
	int			idx;

	char		dial_string[28];
	char		mdm_buff[42];

	memset (dial_string, 0, sizeof(dial_string));
	memset (mdm_buff, 0, sizeof(mdm_buff));

	for (idx = 0; idx < sizeof(parmSDLC.fone); idx++)
	{
		if (parmSDLC.fone[idx] < '0' || parmSDLC.fone[idx] > '9')
			parmSDLC.fone[idx] = 0;	
	}

	if (parmSDLC.pabx[0] != 0)										/* se tem pabx copia a string */
	{
		strcat (dial_string, parmSDLC.pabx);
		strcat  (dial_string, ",");
	}

	strcat (dial_string, parmSDLC.fone);							/* numero do telefone	*/

	strcpy (&SDLC_CMD[AT_DISCA_SDLC][4], dial_string);
	debugH ("dial string=[%s]\n", &SDLC_CMD[AT_DISCA_SDLC][0]);

	if (ATCmdSDLC ( AT_DISCA_SDLC, 0) != POS_SUCESS)				/* envia comando de discagem */
		return POS_ERRO;

	stsSDLC = SDLC_DIALLING;
	tmoDiscSDLC = read_ticks () + TMO_DISC_SDLC;					/* inicia timeout de discagem */
	debugH ("DISCANDO SDLC\n");

	return POS_SUCESS;
}

/******************************************************************************
 * Funcao: chk_sdlc	                                                          *
 *         "verifica status da conexao SDLC	                                  *
 *                                                                            *
 * Retornos: POS_ERRO                                                         *
 *           POS_SUCESS                                                       *
 ******************************************************************************/
STAT
chk_sdlc ( void )
{
	struct Opn_Blk	Com3ob;
	int				status;
	int				ct;
	char			mdm_buff[42];
	STAT			ret;

	get_port_status (hComModem, mdm_buff);
	if ((mdm_buff[3] & DCD_ON) == 0)
	{	
		ret = ATCmdSDLC (-1, 2);									/* ainda nao conectado, obtem resposta */

		debugH ("chk_sdlc-ret[0x%0.4X]\n", ret);
		if (ret == SDLC_ATCMD_TMO || ret == SDLC_CONNECTED)
		{
			if (read_ticks () < tmoDiscSDLC)
				return POS_SUCESS;									/* discando... */
			else													/* time-out discagem */
			{
				stsSDLC = SDLC_NO_ANSWER;
				return POS_ERRO;
			}
		}

		stsSDLC = ret;

		debugH ("[chk_sdlc]ERRO_DISC:stsSDLC=0x%0.4X\n", stsSDLC);
		return POS_ERRO;											/* falha */
	}

	debugH ("SDLC_CONECTADO\n");

	/* portadora presente, trata outros sinais */
	ct = 5;
	do {
	   mdm_buff[0] = DTR_RTS_ON;
	   status = set_serial_lines (hComModem, mdm_buff);
	   SVC_WAIT (100);
	} while (status != 0 && --ct > 0);

	ct = 5;
	do {
	   status = get_port_status (hComModem, mdm_buff);
	   SVC_WAIT (20);
	} while ((mdm_buff[3] & CTS_ON) == 0 && --ct > 0);

	Com3ob.rate							= Rt_19200;
	Com3ob.format						= Fmt_SDLC | Fmt_DTR | Fmt_RTS;
	Com3ob.protocol						= P_sdlc_mode;
	Com3ob.trailer.sdlc_parms.address	= 0x30;
	Com3ob.trailer.sdlc_parms.option	= P_sdlc_sec;

	ct = 5;
	do {
	   status = set_opn_blk (hComModem, &Com3ob);
	   SVC_WAIT (50);
	} while (status != 0 && --ct > 0);

	ct = 5;
	do {
	   mdm_buff[0] = DTR_RTS_ON;
	   status = set_serial_lines (hComModem, mdm_buff);
	   SVC_WAIT (100);
	} while (status != 0 && --ct > 0);

	if (status == 0) status = 5;

	stsSDLC = SDLC_CONNECTED;
	debugH ("SDLC_SINCRONIZADO\n");

	return ( POS_SUCESS );											/* conexao OK */
}

/******************************************************************************
 * Funcao: flushMDM	                                                          *
 *         limpa buffer de recepcao interno do modem                          *
 *                                                                            *
 * Retornos:                                                                  *
 ******************************************************************************/
void
flushMDM ( void )
{
	char aux[2];

	while (read (hComModem, aux, 1) > 0);
}

/******************************************************************************
 * Funcao: sndSDLC	                                                          *
 *         envia bytes para o modem SDLC                                      *
 *                                                                            *
 * Retornos: POS_SUCESS - transmissao sem erros                               *
 *           POS_ERRO   - falha transmissao                                   *
 *           POS_CANCEL - timeout trasmissao                                  *
 ******************************************************************************/
STAT
sndSDLC ( char * inBuf, int sz )
{
	char	xbuff[32];
	ulong	tempoCOM;
	int		st;

	tempoCOM = COM_TIMEOUT * 1000;									/* tempo em milisegundos */
	tempoCOM += get_milli_sec ();									/* hora de vencimento do timeout */
	
	flushMDM ();													/* elimina buffer interno de recebimento antes do envio */
	while (sz > 0 && tempoCOM >= get_milli_sec ())
	{
		for (st = 0; get_port_status (hComModem, xbuff) != 0 && st < 50; st++)
		{
			SVC_WAIT ( 20 );
		}

		st = sz;													/* tamanho do fragmento */
		
		if (write (hComModem, inBuf, st) > 0)
	 	{															/* OK. transferencia feita */
	 		inBuf += st;
			sz -= st;
		}
		else														/* FALHA. Houve algum problema */
		{						
			return POS_ERRO;
		}
	}

	if ( sz > 0 )													/* saida por time out */
		return POS_CANCEL;

	tmoRespSDLC = read_ticks () + 60000l;							/* aciona time-out de P2 */

	return POS_SUCESS;
}

/******************************************************************************
 * Funcao: recvSDLC	                                                          *
 *         obtem bytes recebidos pelo modem SDLC					          *
 *                                                                            *
 * Retornos: POS_SUCESS - transmissao sem erros                               *
 *           POS_ERRO   - falha transmissao                                   *
 *           POS_CANCEL - timeout trasmissao                                  *
 ******************************************************************************/
STAT
recvSDLC ( char * outBuf, int * sz )
{
	// Devido ao teste SDLC, funcao de recepcao esta em sua forma mais simples sem considerar tamanho de resposta ou protocolos...
	char	respBuf[1024];
	int		ct = 0;

	if (!outBuf && !sz)
		return POS_ERRO;

	while (TRUE)
	{
		get_port_status ( hComModem, respBuf );						/* status da portadora */

		if ((respBuf[3] & 0x08) == 0)								/* Portadora NAO presente! caiu conexao*/
		{
			debugH ("[recvSDLC]PORTADORA NAO PRESENTE\n");
			return POS_CANCEL;
		}

		/* Verifica se ha dados no buffer fisico da COM. */
		SVC_WAIT (20);

		get_port_status (hComModem, (char *)respBuf );
		
		if (respBuf[0] == 0)										/* NAO ha novos dados */
		{
			if (read_ticks () < tmoRespSDLC)
			{
				continue;
			}
			else
			{
				debugH ("[recvSDLC]OCORREU TIMEOUT P2\n");
				return POS_ERRO;									/* excedeu time-out */
			}
		}
		else
			break;													/* chegou dados */
	}

		/* Ha dados. Transfere para o buffer sincrono. */

		*sz = 0;

		while ((ct = read (hComModem, &outBuf[*sz], 1)) > 0)		/* desconsiderado tratamento de fragmentos */
			*sz += ct;												/* desconsiderado tratamento de lixo */

		if (*sz > 0)
		{
			debugH ("[recvSDLC]chegou [%d]bytes\n", *sz);
			return POS_SUCESS;
		}

		if (ct < 0)
		{
			debugH ("[recvSDLC]falha recepcao\n", *sz);
			return POS_CANCEL;
		}

		return POS_ERRO;
}
