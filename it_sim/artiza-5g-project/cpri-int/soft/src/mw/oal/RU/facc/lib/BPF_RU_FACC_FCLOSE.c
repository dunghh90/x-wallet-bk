/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_FCLOSE.c
 *	@brief	File Access Control File Close.
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_FACC /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_i_ru_facc.h"

/** @addtogroup BPF_RU_FACC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  File Access Control File Close.
 *  @note   API function. 
 *  @param  file_descriptor [in]  directory path name
 *  @param  errcd           [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_FCLOSE     file close error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_FACC_FCLOSE(int file_descriptor, int *errcd)
{
    int ret;
    
    /************************************************
    * file unlock processing.                       *
    ************************************************/
    /* 排他制御を外す */
    ret = bpf_ru_facc_fileunlock(file_descriptor); /*ファイルディスクリプタ(I)*/ 

    /************************************************
    * file close processing.                        *
    ************************************************/
    /* システムコール file closeを実行 */
    ret = close(file_descriptor);          /* ファイルディスクリプタ (I) */

    if (ret != BPF_RU_FACC_OK){
        /* file close失敗 */
        *errcd = errno;
        return BPF_RU_FACC_ERR_FCLOSE;
    }

    /* ファイルクローズ正常終了 */
    return BPF_RU_FACC_COMPLETE;
}
/* @} */

/* @} */
