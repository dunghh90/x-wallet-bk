/********************************************************************************/
/**
 *  @skip		$Id:$
 *  @file		tm_ls_typ.h
 *  @brief	LAN Message Sending task Common structure
 *  @date   2008/07/29 FFCS)zhengmh create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-
  */
/********************************************************************************/


#ifndef TM_LS_TYP
#define TM_LS_TYP

/**
 *  @brief  Error information table tag
 *  @note  Contents :  Error information table tag
 *  
 * 
 */

typedef	struct {			/**< error information table tag	*/
	INT		ls_send;		/**< return val of send()	*/
	INT		ls_close;		/**< return val of close()*/
}LST_ERRINFO;

#endif/*TM_LS_TYP*/

