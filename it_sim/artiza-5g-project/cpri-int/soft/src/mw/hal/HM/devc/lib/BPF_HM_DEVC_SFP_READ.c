/* RRH-001 MD alpha)yokoyama add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_SFP_READ.c
 *  @brief  
 *  @date   2013/11/27 ALPHA)yokoyama create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013-2015
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_HM_DEVC /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_hm_devc.h"

/** @addtogroup BPF_HM_DEVC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  I2C(SFP)読み込み
 *  @note   BPF_HM_DEVC_SFP_READ
 *  @param  kind    [in]   種別
 *  @param  offset  [in]   SFP読み込みオフセット
 *  @param  data_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/11/27 ALPHA)yokoyama create
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_SFP_READ( unsigned int kind, unsigned short offset, unsigned char *data_p)
{
	int setpage;                                    /* デバイスアドレス       */
	int ret;                                        /* 戻り値                 */

	/* Trace log */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_SFP_READ offset = %04x" ,offset);

	/* DeviceによってOffsetが必要	*/
	switch( kind )	{
		case BPF_HM_DEVC_SFP_A0:
			offset |= BPF_HM_DEVC_SFP_BASE_ADD_A0;	/* primary a0 */
			break;
		case BPF_HM_DEVC_SFP_A2:
			offset |= BPF_HM_DEVC_SFP_BASE_ADD_A2;	/* primary a2 */
			break;
		default:
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "kind NG = %d", kind );
			return BPF_HM_DEVC_NG;
			break;
	}

	setpage = offset >> 8;

	ret = bpf_hm_i2c_read(	BPF_HM_DEVC_I2C_1,				/* 種別				*/
							setpage,						/* setpage			*/
							offset,							/* オフセット		*/
							1,								/* 読み込みサイズ	*/
							BPF_HM_DEVC_ACCESS_1BYTE,		/* アクセスタイプ	*/
							data_p );						/* 読み込みデータ	*/
	
	if (ret != BPF_HM_DEVC_COMPLETE)
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "bpf_hm_i2c_read NG = %d", ret );
		/* ioctl失敗 */
		return BPF_HM_DEVC_NG;
	}

	BPF_COM_PLOG_I2C(E_I2C_LV_READ,offset,(unsigned int)*data_p,1,kind);

	return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  I2C(SFP)読み込み(BUFFER)
 *  @note   BPF_HM_DEVC_SFP_READ
 *  @param  kind    [in]   種別
 *  @param  offset  [in]   SFP読み込みオフセット
 *  @param  count   [in]   読み込み数
 *  @param  data_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/12/02 ALPHA)yokoyama create
 *  @date   2015/07/27 ALPHA)ueda modify for TDD-SRE(Zynq)
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_SFP_READ_BUFFER( unsigned int kind, unsigned short offset,  unsigned int count, unsigned char *data_p )
{
	int setpage;                                    /* デバイスアドレス       */
	int ret;                                        /* 戻り値                 */
	pthread_mutex_t  *mutex;                        /* mutexオブジェクト      */
	unsigned int readcount = 0;

	/* Trace log */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_SFP_READ_BUFFER offset = %04x" ,offset);
	
	/* DeviceによってOffsetが必要	*/
	switch( kind )	{
		case BPF_HM_DEVC_SFP_A0:
			offset |= BPF_HM_DEVC_SFP_BASE_ADD_A0;	/* primary a0 */
			break;
		case BPF_HM_DEVC_SFP_A2:
			offset |= BPF_HM_DEVC_SFP_BASE_ADD_A2;	/* primary a2 */
			break;
		default:
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "kind NG = %d", kind );
			return BPF_HM_DEVC_NG;
			break;
	}

	setpage = offset >> 8;
	/* Mutex lock */
	mutex = &di_devc_data_p->info[ BPF_HM_DEVC_I2C_1 ].mutex;          /* mutexオブジェクト      */
	ret = bpf_hm_devc_mutex_lock(mutex);

	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
			"pthread_mutex_lock mutex = %08x, error = %d",
			mutex,errno);

		return BPF_HM_DEVC_MUTEX_NG;
	}

	for( readcount = 0; readcount < count; readcount++, offset += 1, data_p += 1)
	{
		ret = bpf_hm_i2c_read_no_mutex(	BPF_HM_DEVC_I2C_1,				/* 種別				*/
										setpage,						/* setpage			*/
										offset,							/* オフセット		*/
										1,								/* 読み込みサイズ	*/
										BPF_HM_DEVC_ACCESS_1BYTE,		/* アクセスタイプ	*/
										data_p );						/* 読み込みデータ	*/
		
		if (ret != BPF_HM_DEVC_COMPLETE)
		{
			/* Mutex Unlock */
			pthread_mutex_unlock(mutex);
			
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "bpf_hm_i2c_read_no_mutex NG = %d", ret );
			
			/* ioctl失敗 */
			return BPF_HM_DEVC_NG;
		}
	}

	/* Mutex Unlock */
	pthread_mutex_unlock(mutex);
	
	BPF_COM_PLOG_I2C(E_I2C_LV_READ,offset,count,1,kind);		/* data_pは複数のデータがはいるので、カウント数のみをログに取る */

	return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @} */
/* RRH-001 MD alpha)yokoyama add end */
