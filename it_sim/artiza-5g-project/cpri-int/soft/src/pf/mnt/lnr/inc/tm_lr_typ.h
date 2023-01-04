/********************************************************************************/
/**
 *  @skip		$Id:$
 *  @file		tm_lr_typ.h
 *  @brief	LAN Message Receiving task Common structure
 *  @date   2008/07/29 FFCS)zhengmh create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
  */
/********************************************************************************/


#ifndef TM_LR_TYP
#define TM_LR_TYP

/**
 *  @brief  Error information table tag
 *  @note  Contents :  Error information table tag
 *  
 * 
 */
typedef	struct {					/**< error information table tag	*/
	INT		lr_ebdopen;				/**< return val of ebdopen()*/
	INT		lr_socket;				/**< return val of socket()*/
	INT		lr_setsopt;				/**< return val of setsopt()*/
	INT		lr_bind;				/**< return val of bind()*/
	INT		lr_listen;				/**< return val of listen()*/
	INT		lr_select;				/**< return val of select()*/	
	INT		lr_accept;				/**< return val of accept()*/
	INT		lr_recv;				/**< return val of recv()*/
}LRT_ERRINFO;

#endif/*TM_LR_TYP*/

