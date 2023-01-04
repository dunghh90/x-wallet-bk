/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_REG_WRITE.c
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

//static int bpf_hm_dev_reg_pgc_mode = DEVC_FLG_OFF;

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタ書き込み
 *  @note   レジスタのWRITE関数を具備する。
 *  @param  loglevel[in]   LogLevel
 *  @param  offset  [in]   レジスタ書き込みオフセット
 *  @param  data_p  [in]   書き込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/12/12 ALPHA)Kuwamura    docomo SLC/1CHIP対応
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_REG_WRITE(unsigned int loglevel, unsigned int offset, unsigned int *data_p)
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
    ret = bpf_hm_dev_reg_info(offset,&read_offset,&size,&map_descriptor,&mutex, &access_type );
    if (ret != BPF_HM_DEVC_COMPLETE)
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "Offset Range NG. offset = %08x",
            offset);

        /* データ取得失敗 */
        return BPF_HM_DEVC_NG;
    }

	/* レジスタWRITE関数CALL	*/
	ret = bpf_hm_dev_reg_write( access_type, map_descriptor, read_offset, size, mutex, data_p );
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_write_error ret=%d addr=0x%08x", ret, offset );
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
 *  @brief  レジスタ書き込み(FPGA専用)
 *  @note   レジスタのWRITE関数(FPGA専用)を具備する。
 *  @param  offset  [in]   レジスタ書き込みオフセット
 *  @param  data_p  [in]   書き込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/12/12 ALPHA)Kuwamura    docomo SLC/1CHIP対応
 */
/********************************************************************************************************************/
int bpf_hm_devc_reg_write_fpga(unsigned int offset, unsigned int *data_p)
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

	/* レジスタWRITEする	*/
	ret = bpf_hm_dev_reg_write_simple( access_type, map_descriptor, read_offset, size, data_p );
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_write_error ret=%d addr=0x%08x", ret, offset );
		return BPF_HM_DEVC_NG;
	}

	/* PGCへのアクセスは大量にアクセスするため、ログ取得すると性能劣化の懸念があるためログはskipする */
	/* RF-EEPへのアクセスはログ取得処理は呼ぶが、ログレベルはD_RRH_LOG_REG_LV_WRITE_POL(通常運用ではとらないレベル)とする */
//	if(offset == BPF_HM_DEVC_I2C_OFFSET10){
//		if(*data_p == DEVC_FLG_ON){	/* PGCアクセス要求時 */
//			bpf_hm_dev_reg_pgc_mode = DEVC_FLG_ON;
//		}else{						/* PGCアクセス解放時 */
//			bpf_hm_dev_reg_pgc_mode = DEVC_FLG_OFF;
//		}
//	}else{
//		if(bpf_hm_dev_reg_pgc_mode == DEVC_FLG_OFF){	/* PGCアクセス解放中(RF-EEP中) */
//			if( (offset & 0xFFFFF000) == BPF_HM_DEVC_I2C_OFFSET0){
				BPF_COM_LOG_REG( D_RRH_LOG_REG_LV_WRITE_POL, access_type, offset, *data_p, 0 );
//			}
//		}
//	}

	return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタ書き込み(Buffer)
 *  @note   レジスタのWRITE関数を具備する。
 *  @param  loglevel[in]   LogLevel
 *  @param  offset  [in]   レジスタ書き込みオフセット
 *  @param  size    [in]   レジスタ書き込みサイズ
 *  @param  data_p  [in]   書き込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/07/09 ALPHA)Yokoyama create
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_REG_WRITE_BUFFER(unsigned int loglevel, unsigned int offset, unsigned int size, unsigned int *data_p)
{
    volatile int read_offset;                                /* readオフセット         */
    volatile int mapsize;                                    /* サイズ                 */
    volatile int ret;                                        /* 戻り値                 */
    volatile pthread_mutex_t  *mutex;                        /* mutexオブジェクト      */
    volatile char *map_descriptor;                           /* マップディスクリプタ   */
	volatile int count;
	volatile int	writeCnt;
	volatile unsigned int	copy_size;
	volatile unsigned int	temp_data = 0;
	volatile int access_type;                                /* アクセスタイプ         */
	unsigned short	*data_p_short;
	volatile unsigned short	temp_data_short;
	
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
	/* 特殊処理なので共通WRITE関数は使用しない	*/
	/********************************************/
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock((pthread_mutex_t *)mutex);
	if( ret != 0 ){

	    bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
	        "pthread_mutex_lock mutex = %08x, error = %d",
	        mutex,errno);

	    return BPF_HM_DEVC_NG;
	}
		
	if( BPF_HM_DEVC_ACCESS_4BYTE == access_type )
	{
		/* 端数に関しては後程設定	*/
		writeCnt = size/4;
		
		for( count = 0; count < writeCnt; count++ )
		{
			/* データを設定する */
		    *((unsigned *)(map_descriptor+read_offset+(count*4))) = data_p[count];
			
		}
		
		/* 端数分を設定する	*/
		if( ( size%4 ) != 0 )
		{
			copy_size = size%4;
			memcpy( (char*)&temp_data, &data_p[count], copy_size );
			/* レジスタアクセスは4byteアクセス	*/
			/* データを設定する */
		    *((unsigned *)(map_descriptor+read_offset+(count*4))) = temp_data;
		}
	}
	else if( BPF_HM_DEVC_ACCESS_2BYTE == access_type )
	{
		/* 端数に関しては後程設定	*/
		writeCnt = size/2;
		data_p_short = (unsigned short *)data_p;
		
		for( count = 0; count < writeCnt; count++ )
		{
			/* データを設定する */
		    *((unsigned short*)(map_descriptor+read_offset+(count*2))) = data_p_short[count];
			
		}
		
		/* 端数分を設定する	*/
		if( ( size%2 ) != 0 )
		{
			copy_size = size%2;
			memcpy( (char*)&temp_data_short, &data_p_short[count], copy_size );
			/* レジスタアクセスは2byteアクセス	*/
			/* データを設定する */
		    *((unsigned *)(map_descriptor+read_offset+(count*2))) = temp_data_short;
		}
	}
	else
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "register access type ng = %d", access_type);
		/* mutex解除	*/
		pthread_mutex_unlock((pthread_mutex_t *)mutex);
		return BPF_HM_DEVC_NG;
	}
		
	/* 同期する	*/
	msync( (void*)map_descriptor, mapsize, MS_SYNC );
	
	/* Mutex Unlock */
	pthread_mutex_unlock((pthread_mutex_t *)mutex);
	
	/* Register Access履歴を取得する	*/
	BPF_COM_LOG_REG( loglevel, access_type, offset, data_p[0], 0 );
	
    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタ書き込み(Buffer)
 *  @note   レジスタのWRITE関数を具備する。
 *  @param  loglevel[in]   LogLevel
 *  @param  offset  [in]   レジスタ書き込みオフセット
 *  @param  size    [in]   レジスタ書き込みサイズ
 *  @param  data_p  [in]   書き込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/12/25 FJT)Taniguchi create
 *  @date   2014/12/12 ALPHA)Kuwamura    docomo SLC/1CHIP対応
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_REG_WRITE_BUFFER_END(unsigned int loglevel, unsigned int offset, unsigned int size, unsigned int *data_p)
{
    volatile int read_offset;                                /* readオフセット         */
    volatile int mapsize;                                    /* サイズ                 */
    volatile int ret;                                        /* 戻り値                 */
    volatile pthread_mutex_t  *mutex;                        /* mutexオブジェクト      */
    volatile char *map_descriptor;                           /* マップディスクリプタ   */
	volatile int count;
	volatile int	writeCnt;
	volatile unsigned int	copy_size;
	volatile unsigned int	temp_data = 0;
	volatile int access_type;                                /* アクセスタイプ         */
	unsigned short	*data_p_short;
	volatile unsigned short	temp_data_short;

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
	/* 特殊処理なので共通WRITE関数は使用しない	*/
	/********************************************/
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock((pthread_mutex_t *)mutex);
	if( ret != 0 ){

	    bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
	        "pthread_mutex_lock mutex = %08x, error = %d",
	        mutex,errno);

	    return BPF_HM_DEVC_NG;
	}
		
	if( BPF_HM_DEVC_ACCESS_4BYTE == access_type )
	{
		/* 端数に関しては後程設定	*/
		writeCnt = size/4;
		
		for( count = 0; count < writeCnt; count++ )
		{
			/* データを設定する */
		    *((unsigned *)(map_descriptor+read_offset+(count*4))) = htonl(data_p[count]);
		}

		/* 端数分を設定する	*/
		if( ( size%4 ) != 0 )
		{
			copy_size = size%4;
			memcpy( (char*)&temp_data, &data_p[count], copy_size );
			/* レジスタアクセスは4byteアクセス	*/
			/* データを設定する */
		    *((unsigned *)(map_descriptor+read_offset+(count*4))) = htonl(temp_data);
		}
	}
	else if( BPF_HM_DEVC_ACCESS_2BYTE == access_type )
	{
		/* 端数に関しては後程設定	*/
		writeCnt = size/2;
		data_p_short = (unsigned short *)data_p;
		
		for( count = 0; count < writeCnt; count++ )
		{
			/* データを設定する */
		    *((unsigned short*)(map_descriptor+read_offset+(count*2))) = htons(data_p_short[count]);
		}

		/* 端数分を設定する	*/
		if( ( size%2 ) != 0 )
		{
			copy_size = size%2;
			memcpy( (char*)&temp_data_short, &data_p_short[count], copy_size );
			/* レジスタアクセスは2byteアクセス	*/
			/* データを設定する */
		    *((unsigned *)(map_descriptor+read_offset+(count*2))) = htons(temp_data_short);
		}
	}
	else
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "register access type ng = %d", access_type);
		/* mutex解除	*/
		pthread_mutex_unlock((pthread_mutex_t *)mutex);
		return BPF_HM_DEVC_NG;
	}
	
	/* 同期する	*/
	msync( (void*)map_descriptor, mapsize, MS_SYNC );
	
	/* Mutex Unlock */
	pthread_mutex_unlock((pthread_mutex_t *)mutex);
	
	/* Register Access履歴を取得する	*/
	BPF_COM_LOG_REG( loglevel, access_type, offset, data_p[0], 0 );
	
    return BPF_HM_DEVC_COMPLETE;
}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタ書き込み(Multi)
 *  @note   レジスタのWRITE関数を具備する。
 *  @param  loglevel  [in]   LogLevel
 *  @param  reg_cnt   [in]   レジスタ書き込み数
 *  @param  reg_addr  [in]   レジスタ書き込みアドレス
 *  @param  data_p    [in]   書き込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/11/07 ALPHA)Yokoyama create
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_REG_WRITE_MULTI(unsigned int loglevel, unsigned int reg_cnt, unsigned int *reg_addr, unsigned int *data_p)
{
    volatile int read_offset;                                /* readオフセット         */
    volatile int mapsize;                                    /* サイズ                 */
    volatile int ret;                                        /* 戻り値                 */
    volatile pthread_mutex_t  *mutex;                        /* mutexオブジェクト      */
    volatile char *map_descriptor;                           /* マップディスクリプタ   */
	volatile int count;
	volatile int access_type;                                /* アクセスタイプ         */

    /* 初期化処理 */
    read_offset = 0; 
    mapsize = 0;
    ret = 0;

	for( count = 0; count < reg_cnt; count++ )
	{
	    /* オフセット情報から、取得対象のデータの情報を取得する */
	    ret = bpf_hm_dev_reg_info(reg_addr[count],&read_offset,&mapsize,&map_descriptor,&mutex, &access_type);
	    if (ret != BPF_HM_DEVC_COMPLETE)
	    {
	        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
	            "Offset Range NG. offset = %08x",
	            reg_addr[count]);
	    	
		    pthread_mutex_unlock((pthread_mutex_t *)mutex);
	        /* データ取得失敗 */
	        return BPF_HM_DEVC_NG;
	    }
		
		/* レジスタWRITE関数CALL	*/
		ret = bpf_hm_dev_reg_write( access_type, map_descriptor, read_offset, mapsize, mutex, &data_p[count] );
		if( ret != 0 ){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_write_error ret=%d addr=0x%08x", ret, data_p[count] );
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
 *  @brief  レジスタ書き込み(BIT ON)
 *  @note   レジスタのWRITE関数を具備する。
 *  @param  loglevel[in]   LogLevel
 *  @param  offset  [in]   レジスタ書き込みオフセット
 *  @param  bitinfo [in]   ONするBit
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/11/07 ALPHA)Yokoyama create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_REG_WRITE_BITON(unsigned int loglevel, unsigned int offset, unsigned int bitifno )
{
    volatile int read_offset;                                /* readオフセット         */
    volatile int size;                                       /* サイズ                 */
    volatile int ret;                                        /* 戻り値                 */
    volatile pthread_mutex_t  *mutex;                        /* mutexオブジェクト      */
    volatile char *map_descriptor;                           /* マップディスクリプタ   */
	volatile unsigned int   temp_data, temp_data_before;
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
	
	/*******************************************/
	/* 特殊処理なので共通関数は使用しない		*/
	/*******************************************/
    /* Mutex lock */
    ret = bpf_hm_devc_mutex_lock((pthread_mutex_t *)mutex);
    if( ret != 0 ){

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "pthread_mutex_lock mutex = %08x, error = %d",
            mutex,errno);

        return BPF_HM_DEVC_NG;
    }
	
	/* レジスタREADする	*/
	ret = bpf_hm_dev_reg_read_simple( access_type, map_descriptor, read_offset, size,(unsigned int *)&temp_data );
	temp_data_before = temp_data;
	temp_data |= bitifno;
	/* レジスタWRITEする	*/
	ret |= bpf_hm_dev_reg_write_simple( access_type, map_descriptor, read_offset, size,(unsigned int *)&temp_data );
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_write_error ret=%d addr=0x%08x", ret, offset );
		/* Mutex Unlock */
		pthread_mutex_unlock((pthread_mutex_t *)mutex);
		return BPF_HM_DEVC_NG;
	}
	
	
	/* 同期する	*/
	msync( (void*)map_descriptor, size, MS_SYNC );
	
    /* Mutex Unlock */
    pthread_mutex_unlock((pthread_mutex_t *)mutex);
	
	/* Register Access履歴を取得する	*/
	BPF_COM_LOG_REG( loglevel, access_type, offset, temp_data_before, (temp_data | bitifno) );
	
    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタ書き込み(BIT OFF)
 *  @note   レジスタのWRITE関数を具備する。
 *  @param  loglevel[in]   LogLevel
 *  @param  offset  [in]   レジスタ書き込みオフセット
 *  @param  bitinfo [in]   OFFするBit
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/11/07 ALPHA)Yokoyama create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_REG_WRITE_BITOFF(unsigned int loglevel, unsigned int offset, unsigned int bitifno )
{
    volatile int read_offset;                                /* readオフセット         */
    volatile int size;                                       /* サイズ                 */
    volatile int ret;                                        /* 戻り値                 */
    volatile pthread_mutex_t  *mutex;                        /* mutexオブジェクト      */
    volatile char *map_descriptor;                           /* マップディスクリプタ   */
	volatile unsigned int   temp_data, temp_data_before;
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

	/*******************************************/
	/* 特殊処理なので共通関数は使用しない		*/
	/*******************************************/
    /* Mutex lock */
    ret = bpf_hm_devc_mutex_lock((pthread_mutex_t *)mutex);
    if( ret != 0 ){

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "pthread_mutex_lock mutex = %08x, error = %d",
            mutex,errno);

        return BPF_HM_DEVC_NG;
    }

	/* レジスタREADする	*/
	ret = bpf_hm_dev_reg_read_simple( access_type, map_descriptor, read_offset, size, (unsigned int *)&temp_data );
	temp_data_before = temp_data;
	temp_data &= ~bitifno;
	/* レジスタWRITEする	*/
	ret |= bpf_hm_dev_reg_write_simple( access_type, map_descriptor, read_offset, size, (unsigned int *)&temp_data );
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_write_error ret=%d addr=0x%08x", ret, offset );
		/* Mutex Unlock */
		pthread_mutex_unlock((pthread_mutex_t *)mutex);
		return BPF_HM_DEVC_NG;
	}
	
	/* 同期する	*/
	msync( (void*)map_descriptor, size, MS_SYNC );
	
    /* Mutex Unlock */
    pthread_mutex_unlock((pthread_mutex_t *)mutex);
        
	/* Register Access履歴を取得する	*/
	BPF_COM_LOG_REG( loglevel, access_type, offset, temp_data_before, (temp_data & ~bitifno) );
	
    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
