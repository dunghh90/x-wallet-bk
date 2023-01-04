#ifndef __INCL_FLDCCOM_H
#define	__INCL_FLDCCOM_H
/************************************************************************/
/*																		*/
/*	FILE-NAME : fldcCom.h									   			*/
/*	FUNCTION  : fldc return value definition table			 			*/
/*	AUTHOR	  : Noriko Satoh											*/
/*	DATE	  : 99/02/01												*/
/*	VERSION   : 1.9 											 		*/
/*																		*/
/*	  All Rights Reserved, Copyright(C) 								*/
/*	  FUJITSU LABORATORIES LTD. 1998									*/
/*																		*/
/************************************************************************/
/*	DATE	: 2008.10.02	FJT)T.Yanagisawa							*/
/*							MBBTS 08A(2008/10/02)î≈Ç©ÇÁéÊÇËèoÇµ			*/
/*																		*/
/*	UPDATE	: 															*/
/*																		*/
/*	All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008				*/
/************************************************************************/
/* for MB	return value */
#define PARA_NG		 9	/* parameter value invalid						*/
/* for fldc return value */
#define NORMAL		 0	/* normal (used in fldc1,2,3) 					*/
#define BADCODE 	 1	/* corrupted encoded data (used in fldc1,2,3) 	*/
#define BUFFERFULL	 2	/* buffuer full (used in fldc1,2) 				*/
#define BUFFEREMPTY  3	/* buffer is empty (used in fldc3) 				*/
#define DetectEOF	 4	/* detect EOF (used in fldc1,2) 				*/
#define ERR_FATAL	 5	/* FATAL error(used in fldc1,2,3)  				*/

#endif
