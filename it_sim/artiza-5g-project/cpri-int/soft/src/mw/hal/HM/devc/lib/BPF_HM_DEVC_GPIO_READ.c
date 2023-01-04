/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_GPIO_READ.c
 *  @brief  
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_HM_DEVC /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_hm_devc.h"
#include "BPF_COM_LOG.h"

/** @addtogroup BPF_HM_DEVC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  GPIO読み込み
 *  @note   GPIOのREAD関数を具備する。
 *  @param  loglevel[in]   ログレベル
 *  @param  offset  [in]   GPIO読み込みオフセット
 *  @param  data_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/12/12 ALPHA)Yokoyama    docomo SLC/1CHIP対応 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_GPIO_READ( unsigned int loglevel, unsigned int offset, unsigned int *data_p)
{
    volatile int read_offset;                                /* readオフセット         */
    volatile int size;                                       /* サイズ                 */
    volatile int ret;                                        /* 戻り値                 */
    volatile pthread_mutex_t  *mutex;                        /* mutexオブジェクト      */
    volatile char *map_descriptor;                           /* マップディスクリプタ   */
	volatile int access_type;                                /* アクセスタイプ         */

    /* 初期化処理 */
    read_offset = 0; 
    size = 0;
    ret = 0;

    /* Trace log */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_GPIO_READ offset = %08x" ,
                        offset);
    
    /* オフセット情報から、取得対象のデータの情報を取得する */
    ret = bpf_hm_dev_io_info(offset,&read_offset,&size,&map_descriptor,&mutex, &access_type);
    if (ret != BPF_HM_DEVC_COMPLETE)
    {

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "Offset Range NG. offset = %08x",
            offset);

        /* データ取得失敗 */
        return BPF_HM_DEVC_NG;
    }

	/* レジスタREAD関数CALL	*/
	ret = bpf_hm_dev_reg_read( access_type, map_descriptor, read_offset, size, mutex, data_p );
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, offset );
		return BPF_HM_DEVC_NG;
	}
	
	/* Register Access履歴を取得する	*/
	BPF_COM_LOG_REG( (UINT)loglevel, access_type, (UINT)offset, (UINT)*data_p, 0 );
	
    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
