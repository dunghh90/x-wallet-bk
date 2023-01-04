/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_COM_STRING.h
 *  @brief  Definitions for BPF Internal Common String
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_COM_STRING_H_
#define _BPF_COM_STRING_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_COM.h"

/* Function prototype */
char* BPF_COM_STRING_CMBCHAR(char *orgdata, char *adddata);
void BPF_COM_STRING_CTOHEXA(unsigned char num, char *strdata);
int BPF_COM_STRING_ITODECA(unsigned int num, char *strdata);

/********************************************* EOF ******************************************************************/
#endif /* _BPF_COM_STRING_H__ */