/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_COM_OUTPUT_LOG.c
 *  @brief  
 *  @date   2013/05/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_COM /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_l_com.h"
#include "bpf_i_com.h"

/** @addtogroup BPF_COM
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  MWログ出力
 *  @note   BPF_HM_COM_OUTPUT_LOG
 *  @param  buffer     [in]  バッファポインタ
 *  @param  max_size   [in]  最大サイズ
 *  @param  wite_size  [out] 書き込みサイズ
 *  @return r-
 *  @date   2013/05/30 ALPHA)Matsuhashi create
 
 */
/********************************************************************************************************************/
void BPF_HM_COM_OUTPUT_LOG(unsigned char *buffer , unsigned int max_size ,unsigned int *wite_size)
{

    /* アサイン未の場合は処理終了 */
    if(di_log_assign_flag == LOG_FLG_OFF){
        *wite_size = 0;
        return;
    }

    /* 指定されたデータ長書き込む */
    *wite_size = sizeof(t_bpf_ru_log_data);
    if (*wite_size > max_size)
    {
        /* 指定サイズより大きい場合は最大サイズに変更 */
        *wite_size = max_size;
    }
    
    memcpy(buffer,di_log_data_p,*wite_size);

    return;
}

/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
