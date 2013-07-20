#ifndef VAR_TYPES_H
#define VAR_TYPES_H

/******************************************************************************
 * Modulo: var_types.h                                                        *
 *                                                                            *
 * Descricao: Contem definicoes de variaveis para utilizacao no projeto       *
 *                                                                            *
 * Historico:                                                                 *
 * -------------                                                              *
 * Henrique H1 = 11/07/2013 - primeira versao do modulo                       *
 *                                                                            *
 ******************************************************************************/

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