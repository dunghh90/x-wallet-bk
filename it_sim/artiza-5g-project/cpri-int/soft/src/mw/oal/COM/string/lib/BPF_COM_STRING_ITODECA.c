/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_COM_STRING_ITODECA.c
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
 *  @brief  数値を10進文字に変換
 *  @note   API function. 
 *  @param  num        [in]  変換元数値
 *  @param  strdata    [Out] 変換先文字格納領域
 *  @return tmpsize    変換サイズ
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 */
/********************************************************************************************************************/
int BPF_COM_STRING_ITODECA(unsigned int num, char *strdata)
{
    char tmp;
    int tmpsize;
    int loop;
    int revloop;

    loop = 0;
    do{
        strdata[loop] = ( char )( ( num % 10 ) + '0' );
        loop++;
        num = num /10;
    }while(num>0);
    tmpsize=loop;
    for(loop=0, revloop=(tmpsize-1); loop<revloop; loop++, revloop--){
        tmp = strdata[loop];
        strdata[loop]=strdata[revloop];
        strdata[revloop]=tmp;
    }
    return tmpsize;
}
/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
