/* RRH-001 MD alpha)yokoyama add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_TEMP_READ.c
 *  @brief  
 *  @date   2015/04/08 ALPHA)yokoyama create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2015
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
 *  @brief  温度情報読み込み
 *  @note   BPF_HM_DEVC_TEMP_READ
 *  @param  temp_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2015/04/08 ALPHA)yokoyama create
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_TEMP_READ(signed int *temp_p)
{
	int				ret;								/* 戻り値                 */
	unsigned char	buf[2];
	int				iTemp_data;
	double			dTemp_data;
	
	/* Trace log */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_TEMP_READ");

    memset(buf,0,sizeof(buf));
	
	ret = bpf_hm_i2c_read(	BPF_HM_DEVC_I2C_0,				/* 種別				*/
							BPF_HM_DEVC_I2C_BASE_ADD_TEMP,	/* setpage			*/
							0,								/* オフセット		*/
							2,								/* 読み込みサイズ	*/
							BPF_HM_DEVC_ACCESS_2BYTE,		/* アクセスタイプ	*/
							buf );							/* 読み込みデータ	*/
	
	if (ret != BPF_HM_DEVC_COMPLETE)
	{
		/* ioctl失敗 */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "bpf_hm_i2c_read NG = %d", ret );
		return BPF_HM_DEVC_NG;
	}
	
	/* 計算して返す	*/
	/* 符号有りであるから符号BITを最上位に設定するようにし、	*/
	/* その後、右BITシフトさせる								*/
	iTemp_data = ((buf[0] << 24) | (buf[1] << 16));
	iTemp_data >>= 20;
	
	/* 計算	*/
	dTemp_data = (double)iTemp_data;
	dTemp_data *= 0.0625;
	if(dTemp_data < 0)
	{
		dTemp_data -= 0.5;
	}
	else
	{
		dTemp_data += 0.5;
	}
	
	/* 整数だけ返す	*/
	*temp_p = (int)dTemp_data;
	
	BPF_COM_PLOG_I2C(E_I2C_LV_READ,0,(unsigned int)(*temp_p),0,1);	/* packet log */

	return BPF_HM_DEVC_COMPLETE;
}

/* @} */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  温度情報読み込み(4TRxRRE から踏襲)
 *  @note   BPF_HM_DEVC_TEMP2_READ
 *  @param  temp_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/03/01 KCN)fukushima create
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_TEMP2_READ(signed int *temp_p)
{
	int				ret;								/* 戻り値                 */
	unsigned char	buf[2];
	int				iTemp_data;
	double			dTemp_data;
	int				Temp_work1;	/* 整数部の算出用 */
	int				Temp_work2;	/* 10倍した値算出用 */
	
	/* Trace log */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_TEMP_READ");

    memset(buf,0,sizeof(buf));
	
	ret = bpf_hm_i2c_read(	BPF_HM_DEVC_I2C_0,				/* 種別				*/
							BPF_HM_DEVC_I2C_BASE_ADD_TEMP,	/* setpage			*/
							0,								/* オフセット		*/
							2,								/* 読み込みサイズ	*/
							BPF_HM_DEVC_ACCESS_2BYTE,		/* アクセスタイプ	*/
							buf );							/* 読み込みデータ	*/
	
	if (ret != BPF_HM_DEVC_COMPLETE)
	{
		/* ioctl失敗 */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "bpf_hm_i2c_read NG = %d", ret );
		return BPF_HM_DEVC_NG;
	}
	
	/* 計算して返す	*/
	/* 符号有りであるから符号BITを最上位に設定するようにし、	*/
	/* その後、右BITシフトさせる								*/
	iTemp_data = ((buf[0] << 24) | (buf[1] << 16));
	iTemp_data >>= 20;
	
	/* 計算	*/
	dTemp_data = (double)iTemp_data;
	dTemp_data *= 0.0625;
	if(dTemp_data < 0)
	{						/* この加減算は例えて言うと9.75～10.24を10.0とし、10.25～10.75を10.5とする */
							/* という結果になるように、0.5単位の四捨五入的な算出をするためのものです */
		dTemp_data -= 0.25;
	}
	else
	{
		dTemp_data += 0.25;
	}
	
	/* 2倍して小数第一を桁上げする	*/
	/* 例1：10.49の場合、*2で20.98(=20)となり	*/
	/* 例2：10.51の場合、*2で21.02(=21)となり	*/
	Temp_work1 = (int)(dTemp_data * 2);

	/* 2×5=10倍して小数第一を整数化 */
	/* 例1：10.49の場合、20*5で100(=10.0)となる	*/
	/* 例2：10.51の場合、21*5で105(=10.5)となる	*/
	Temp_work2 = (int)(Temp_work1 * 5);
	*temp_p = Temp_work2;

	BPF_COM_PLOG_I2C(E_I2C_LV_READ,0,(unsigned int)(*temp_p),0,1);	/* packet log */

	return BPF_HM_DEVC_COMPLETE;
}

/* @} */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  温度初期化
 *  @note   BPF_HM_DEVC_TEMP_INIT
 *  @param  val1  [in]  1st val
 *  @param  val2  [in]  2nd val
 *  @return N/A
 *  @date   2016/09/07 FJT)Taniguchi
 */
/********************************************************************************************************************/
void BPF_HM_DEVC_TEMP_INIT( void )
{
	unsigned char	buf[2];

	buf[0] = 0x60;
	buf[1] = 0x20;

	(void)bpf_hm_i2c_write(	BPF_HM_DEVC_I2C_0,BPF_HM_DEVC_I2C_BASE_ADD_TEMP,0,buf,BPF_HM_DEVC_ACCESS_2BYTE );

	BPF_COM_PLOG_I2C(E_I2C_LV_WRITE,0,((unsigned int)(buf[0]) << 8) | buf[1] ,0,1);	/* packet log */

	return;
}
/* @} */




/* @} */
/* RRH-001 MD alpha)yokoyama add end */
