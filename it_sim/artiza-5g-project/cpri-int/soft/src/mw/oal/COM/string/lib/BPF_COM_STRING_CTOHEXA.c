/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_COM_STRING_CTOHEXA.c
 *	@brief	Common String Function.
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_COM_STRING /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "BPF_COM_STRING.h"

/** @addtogroup BPF_COM_STRING
 * @{ */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  バイト数値16進文字変換
 *  @note   API function. 
 *  @param  num        [in]  変換元数値
 *  @param  strdata    [Out] 変換先文字格納領域
 *  @return none
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 */
/********************************************************************************************************************/
void BPF_COM_STRING_CTOHEXA(unsigned char num, char *strdata)
{
    int cnt;
    unsigned char cdata;

    for(cnt=0; cnt<2; cnt++){
        if(cnt==0){
            cdata = (num/0x10);
        }else{
            cdata = (num%0x10);
        }
        if(cdata < 10){
            strdata[cnt] = (cdata+'0');
        }else if(cdata < 16){
            strdata[cnt] = ((cdata-10)+'A');
        }else{
            strdata[cnt] = '-';
        }
    }
}
/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
