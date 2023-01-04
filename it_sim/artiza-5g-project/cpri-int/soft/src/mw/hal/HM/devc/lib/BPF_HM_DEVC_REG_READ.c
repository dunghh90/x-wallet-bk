/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_REG_READ.c
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
 *  @brief  レジスタ読み込み
 *  @note   レジスタのREAD関数を具備する。
 *  @param  loglevel[in]   LogLevel
 *  @param  offset  [in]   レジスタ読み込みオフセット
 *  @param  data_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/12/12 ALPHA)Kuwamura    docomo SLC/1CHIP対応
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_REG_READ(unsigned int loglevel, unsigned int offset, unsigned int *data_p)
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

    /* デバイスの初期化が完了する前は処理スキップする */
    if(di_devc_assign_flag != DEVC_FLG_ON){
        return BPF_HM_DEVC_NG;
    }

    /* オフセット情報から、取得対象のデータの情報を取得する */
    ret = bpf_hm_dev_reg_info(offset,&read_offset,&size,&map_descriptor,&mutex, &access_type);
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
	BPF_COM_LOG_REG( loglevel, access_type, offset, *data_p, 0 );
	
    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタ読み込み(FPGA専用)
 *  @note   レジスタのREAD関数(FPGA専用)を具備する。
 *  @param  offset  [in]   レジスタ読み込みオフセット
 *  @param  data_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/12/12 ALPHA)Kuwamura    docomo SLC/1CHIP対応
 */
/********************************************************************************************************************/
int bpf_hm_devc_reg_read_fpga(unsigned int offset, unsigned int *data_p)
{
    volatile int read_offset;                                /* readオフセット         */
    volatile int size;                                       /* サイズ                 */
    volatile int ret;                                        /* 戻り値                 */
    volatile char *map_descriptor;                           /* マップディスクリプタ   */
    volatile pthread_mutex_t  *mutex;                        /* mutexオブジェクト      */
	volatile int access_type;                                /* アクセスタイプ         */

    /* 初期化処理 */
    read_offset = 0; 
    size = 0;
    ret = 0;
    
    /* オフセット情報から、取得対象のデータの情報を取得する */
    ret = bpf_hm_dev_reg_info(offset,&read_offset,&size,&map_descriptor,&mutex, &access_type);
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
	BPF_COM_LOG_REG( D_RRH_LOG_REG_LV_READ, access_type, offset, *data_p, 0 );
	
    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
	*  @brief  レジスタ読み込み(Buffer)
 *  @note   レジスタのREAD関数を具備する。
 *  @param  loglevel[in]   LogLevel
 *  @param  offset  [in]   レジスタ読み込みオフセット
 *  @param  size    [in]   レジスタ読み込みサイズ
 *  @param  data_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/07/09 ALPHA)Yokoyama create
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_REG_READ_BUFFER(unsigned int loglevel, unsigned int offset, unsigned int size, unsigned int *data_p)
{
    volatile int read_offset;                       /* readオフセット         */
    volatile int mapsize;                           /* サイズ                 */
    volatile int ret;                               /* 戻り値                 */
    volatile pthread_mutex_t  *mutex;               /* mutexオブジェクト      */
    volatile char *map_descriptor;                  /* マップディスクリプタ   */
	volatile int count;
	volatile int readCnt;
	volatile unsigned int temp_data;
	volatile unsigned int copy_size;
	volatile int access_type;                       /* アクセスタイプ         */
	volatile short			temp_data_short;
	unsigned short *data_p_short;
	
    /* 初期化処理 */
    read_offset = 0; 
    mapsize = 0;
    ret = 0;

    /* オフセット情報から、取得対象のデータの情報を取得する */
    ret = bpf_hm_dev_reg_info(offset,&read_offset,&mapsize,&map_descriptor,&mutex, &access_type);
    if (ret != BPF_HM_DEVC_COMPLETE)
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "Offset Range NG. offset = %08x",
            offset);

        /* データ取得失敗 */
        return BPF_HM_DEVC_NG;
    }
	
	/********************************************/
	/* 特殊処理なので共通READ関数は使用しない	*/
	/********************************************/
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock((pthread_mutex_t *)mutex);
	if( ret != 0 ){

	        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
	            "pthread_mutex_lock mutex = %08x, error = %d",
	            mutex,errno);

	    return BPF_HM_DEVC_NG;
	}
	
	/* 同期する	*/
	msync( (void*)map_descriptor, mapsize, MS_SYNC );
	
	if( BPF_HM_DEVC_ACCESS_4BYTE == access_type )
	{
		/* READ回数を取得				*/
		/* 端数については別途取得する	*/
		readCnt = size/4;
		
		for( count = 0; count < readCnt; count++ )
		{
		    /* データを設定する */
		    data_p[count] = *((unsigned *)(map_descriptor+read_offset+(count*4)));
		}
		
		/* 端数がある場合は一旦ローカル変数に落としてから、端数分のサイズをコピーする	*/
		if( ( size%4 ) != 0 )
		{
			copy_size = size%4;
			temp_data = *((unsigned *)(map_descriptor+read_offset+(count*4)));
			memcpy( &data_p[count], (char*)&temp_data, copy_size );
		}
	}
	else if( BPF_HM_DEVC_ACCESS_2BYTE == access_type )
	{
		/* READ回数を取得				*/
		/* 端数については別途取得する	*/
		readCnt = size/2;
		data_p_short = (unsigned short *)data_p;
		
		for( count = 0; count < readCnt; count++ )
		{
		    /* データを設定する */
		    data_p_short[count] = *((unsigned short*)(map_descriptor+read_offset+(count*2)));
		}
		
		/* 端数がある場合は一旦ローカル変数に落としてから、端数分のサイズをコピーする	*/
		if( ( size%2 ) != 0 )
		{
			copy_size = size%2;
			temp_data_short = *((unsigned short*)(map_descriptor+read_offset+(count*2)));
			memcpy( &data_p_short[count], (char*)&temp_data_short, copy_size );
		}
	}
	else
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "register access type ng = %d", access_type);
		/* mutex解除	*/
		pthread_mutex_unlock((pthread_mutex_t *)mutex);
		return BPF_HM_DEVC_NG;
	}
	
	/* mutex解除	*/
	pthread_mutex_unlock((pthread_mutex_t *)mutex);
	
	/* Register Access履歴を取得する	*/
	BPF_COM_LOG_REG( loglevel, access_type, offset, data_p[0], 0 );

    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
	*  @brief  レジスタ読み込み(Buffer)
 *  @note   レジスタのREAD関数を具備する。
 *  @param  loglevel[in]   LogLevel
 *  @param  offset  [in]   レジスタ読み込みオフセット
 *  @param  size    [in]   レジスタ読み込みサイズ
 *  @param  data_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/12/25 FJT)Taniguchi create
 *  @date   2014/12/12 ALPHA)Kuwamura    docomo SLC/1CHIP対応
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_REG_READ_BUFFER_END(unsigned int loglevel, unsigned int offset, unsigned int size, unsigned int *data_p)
{
    volatile int read_offset;                       /* readオフセット         */
    volatile int mapsize;                           /* サイズ                 */
    volatile int ret;                               /* 戻り値                 */
    volatile pthread_mutex_t  *mutex;               /* mutexオブジェクト      */
    volatile char *map_descriptor;                  /* マップディスクリプタ   */
	volatile int count;
	volatile int readCnt;
	volatile unsigned int regval;
	volatile unsigned int temp_data;
	volatile unsigned int copy_size;
	volatile int access_type;                       /* アクセスタイプ         */
	volatile unsigned short regval_short;
	volatile unsigned short temp_data_short;
	unsigned short *data_p_short;
	
    /* 初期化処理 */
    read_offset = 0; 
    mapsize = 0;
    ret = 0;

    /* オフセット情報から、取得対象のデータの情報を取得する */
    ret = bpf_hm_dev_reg_info(offset,&read_offset,&mapsize,&map_descriptor,&mutex, &access_type);
    if (ret != BPF_HM_DEVC_COMPLETE)
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "Offset Range NG. offset = %08x",
            offset);

        /* データ取得失敗 */
        return BPF_HM_DEVC_NG;
    }
	
	/********************************************/
	/* 特殊処理なので共通READ関数は使用しない	*/
	/********************************************/
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock((pthread_mutex_t *)mutex);
	if( ret != 0 ){

	    bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
	        "pthread_mutex_lock mutex = %08x, error = %d",
	        mutex,errno);

	    return BPF_HM_DEVC_NG;
	}
	
	/* 同期する	*/
	msync( (void*)map_descriptor, mapsize, MS_SYNC );
	
	
	if( BPF_HM_DEVC_ACCESS_4BYTE == access_type )
	{
		/* READ回数を取得				*/
		/* 端数については別途取得する	*/
		readCnt = size/4;
		
		for( count = 0; count < readCnt; count++ )
		{
		    /* データを設定する */
		    regval = *((unsigned *)(map_descriptor+read_offset+(count*4)));
	        data_p[count] = ntohl(regval);
		}
		
		/* 端数がある場合は一旦ローカル変数に落としてから、端数分のサイズをコピーする	*/
		if( ( size%4 ) != 0 )
		{
			copy_size = size%4;
			regval = *((unsigned *)(map_descriptor+read_offset+(count*4)));
			temp_data = ntohl(regval);
			memcpy( &data_p[count], (char*)&temp_data, copy_size );
		}
	}
	else if( BPF_HM_DEVC_ACCESS_2BYTE == access_type )
	{
		/* READ回数を取得				*/
		/* 端数については別途取得する	*/
		readCnt = size/2;
		data_p_short = (unsigned short *)data_p;
		
		for( count = 0; count < readCnt; count++ )
		{
		    /* データを設定する */
		    regval_short = *((unsigned short*)(map_descriptor+read_offset+(count*2)));
	        data_p_short[count] = ntohs(regval_short);
		}
		
		/* 端数がある場合は一旦ローカル変数に落としてから、端数分のサイズをコピーする	*/
		if( ( size%2 ) != 0 )
		{
			copy_size = size%2;
			regval_short = *((unsigned short*)(map_descriptor+read_offset+(count*2)));
			temp_data_short = ntohs(regval_short);
			memcpy( &data_p_short[count], (char*)&temp_data_short, copy_size );
		}
	}
	else
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "register access type ng = %d", access_type);
		/* mutex解除	*/
		pthread_mutex_unlock((pthread_mutex_t *)mutex);
		return BPF_HM_DEVC_NG;
	}
	
	/* mutex解除	*/
	pthread_mutex_unlock((pthread_mutex_t *)mutex);
	
	/* Register Access履歴を取得する	*/
	BPF_COM_LOG_REG( loglevel, access_type, offset, data_p[0], 0 );

    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
	*  @brief  レジスタ読み込み(Multi)
 *  @note   レジスタのREAD関数を具備する。
 *  @param  loglevel   [in]   LogLevel
 *  @param  reg_cnt    [in]   読み込みレジスタ数
 *  @param  *reg_addr  [in]   読み込みレジスタ
 *  @param  *data_p    [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/07/09 ALPHA)Yokoyama create
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_REG_READ_MULTI(unsigned int loglevel, unsigned int reg_cnt, unsigned int *reg_addr, unsigned int *data_p)
{
    volatile int read_offset;                                /* readオフセット         */
    volatile int mapsize;                                       /* サイズ                 */
    volatile int ret;                                        /* 戻り値                 */
    volatile pthread_mutex_t  *mutex;                        /* mutexオブジェクト      */
    volatile char *map_descriptor;                           /* マップディスクリプタ   */
	volatile int count;
	volatile int access_type;                                /* アクセスタイプ         */
	
    /* 初期化処理 */
    read_offset = 0; 
    mapsize = 0;
    ret = 0;

	/* 同一機能部内の飛び飛びのレジスタデータを取得する	*/
	for( count = 0; count < reg_cnt; count++ )
	{
		/* 先頭のレジスタアドレスから、取得対象の機能部を決定する */
		ret = bpf_hm_dev_reg_info(reg_addr[count],&read_offset,&mapsize,&map_descriptor,&mutex, &access_type);
		if (ret != BPF_HM_DEVC_COMPLETE)
		{
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"bpf_hm_dev_reg_info ret=%d addr=0x%08x", ret, reg_addr[count] );
			return BPF_HM_DEVC_NG;
		}
		
		/* レジスタREAD関数CALL	*/
		ret = bpf_hm_dev_reg_read( access_type, map_descriptor, read_offset, mapsize, mutex, &data_p[count] );
		if( ret != 0 ){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, reg_addr[count] );
			return BPF_HM_DEVC_NG;
		}
		
		/* Register Access履歴を取得する	*/
		BPF_COM_LOG_REG( loglevel, access_type, reg_addr[count], data_p[count], 0 );
		
	}

    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタ読み込み(指定Bit)
 *  @note   レジスタのREAD関数を具備する。
 *  @param  loglevel   [in]   LogLevel
 *  @param  offset     [in]   レジスタ読み込みオフセット
 *  @param  check_bit  [in]   チェックビット
 *  @param  data_p     [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/11/07 ALPHA)Yokoyama create
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_REG_READ_BIT(unsigned int loglevel, unsigned int offset, unsigned int check_bit, unsigned int *data_p)
{
    volatile int read_offset;                                /* readオフセット         */
    volatile int size;                                       /* サイズ                 */
    volatile int ret;                                        /* 戻り値                 */
    volatile pthread_mutex_t  *mutex;                        /* mutexオブジェクト      */
    volatile char *map_descriptor;                           /* マップディスクリプタ   */
	volatile unsigned int tempdat;
	volatile int access_type;                                /* アクセスタイプ         */

    /* 初期化処理 */
    read_offset = 0; 
    size = 0;
    ret = 0;

    /* デバイスの初期化が完了する前は処理スキップする */
    if(di_devc_assign_flag != DEVC_FLG_ON){
        return BPF_HM_DEVC_NG;
    }

    /* オフセット情報から、取得対象のデータの情報を取得する */
    ret = bpf_hm_dev_reg_info(offset,&read_offset,&size,&map_descriptor,&mutex, &access_type);
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
	
	/* BITを落とす前のデータ取得(ログ用)	*/
	tempdat = *data_p;
	
	/* 指定BIT以外は落とす	*/
	*data_p &= check_bit;
	
	/* Register Access履歴を取得する	*/
	BPF_COM_LOG_REG( loglevel, access_type, offset, tempdat, *data_p );
	
    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタ読み込み nomutex
 *  @note   レジスタのREAD関数を具備する。nomutex版
 *  @param  loglevel[in]   LogLevel
 *  @param  offset  [in]   レジスタ読み込みオフセット
 *  @param  data_p  [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/12/12 ALPHA)Kuwamura    docomo SLC/1CHIP対応
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_REG_READ_NOMUTEX(unsigned int loglevel, unsigned int offset, unsigned int *data_p)
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

    /* デバイスの初期化が完了する前は処理スキップする */
    if(di_devc_assign_flag != DEVC_FLG_ON){
        return BPF_HM_DEVC_NG;
    }

    /* オフセット情報から、取得対象のデータの情報を取得する */
    ret = bpf_hm_dev_reg_info(offset,&read_offset,&size,&map_descriptor,&mutex, &access_type);
    if (ret != BPF_HM_DEVC_COMPLETE)
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "Offset Range NG. offset = %08x",
            offset);

        /* データ取得失敗 */
        return BPF_HM_DEVC_NG;
    }

	/* レジスタREAD関数CALL	*/
	/* mutexは取得しない */
	ret = bpf_hm_dev_reg_read_simple( access_type, map_descriptor, read_offset, size, data_p );
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, offset );
		return BPF_HM_DEVC_NG;
	}
	
	/* Register Access履歴を取得する	*/
	BPF_COM_LOG_REG( loglevel, access_type, offset, *data_p, 0 );
	
    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
