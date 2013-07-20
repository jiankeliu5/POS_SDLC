/******************************************************************************
 * Modulo Fonte..........: geral.h                                            *
 *                                                                            *
 * Data de criacao.......: 11/07/2013                                         *
 * Desenvolvedor.........: Henrique Hornos                                    *
 *                                                                            *
 *----------------------------------------------------------------------------*
 * Descricao.............:                                                    *
 * definicoes gerais para funcionamento do nivel de aplicacao                 *
 *----------------------------------------------------------------------------*
 * Alteracoes............:                                                    *
 * Desenvolvedor.........:                                                    *
 * Data da alteracao.....:                                                    *
 *                                                                            *
 * "geral.h, v0.0.1 2013/07/11 20:45:00 id: "Henrique_H1"                     *
 *                                                                            *
 ******************************************************************************/
#ifndef VAR_TYPES_H
#define VAR_TYPES_H
#endif

/* prototipos gerais */
void	debugH ( char *, ... );

/* definicoes de tipo de variaveis */

#ifndef		uchar
#define		uchar		unsigned char
#endif

#ifndef		ulong
#define		ulong		unsigned long
#endif

#ifdef		BOOL
#undef		BOOL
#endif
#define		BOOL		unsigned char

#ifdef		STAT
#undef		STAT
#endif		
#define		STAT		long

#ifndef		ushort
#define		ushort		unsigned short
#endif

#ifndef		uint
#define		uint		unsigned int
#endif

#ifndef		TRUE
#define		TRUE		1
#endif

#ifndef		FALSE
#define		FALSE		0
#endif

/* definicoes de STATUS de retorno funcoes da aplicacao (nivel aplic) */

#define		POS_SUCESS		0x0000							/* SUCESSO */
#define		POS_ERRO		0x0001							/* ERRO */
#define		POS_CANCEL		0x0002							/* CANCELAMENTO */
