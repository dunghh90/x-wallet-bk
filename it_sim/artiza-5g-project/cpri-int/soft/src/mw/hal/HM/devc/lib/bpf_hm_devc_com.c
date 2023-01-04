/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_hm_devc_com.c
 *  @brief  
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013-2015
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_HM_DEVC /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_hm_devc.h"
#include "BPF_COM_LOG.h"
#include <linux/types.h>
#include <linux/spi/spidev.h>

/** @addtogroup BPF_HM_DEVC
 * @{ */


/********************************************************************************************************************/
/**
 *  @brief  I2C-1 Controller Status register check process
 *  @note   check i2c controller status. if status Bus active bit(b8) is set, reset i2c controller and 
 *          set configration again.
 *          Note: workaround process for hardware issue. (issue: i2c controller status stacking data transfer)
 *  @param  -
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2015/10/15 samejid M-RRU-ZSYS-01680対応(B3流用(BPF_HM_DEVC_I2C_STAT_CHK_WA)）<TDD3.5G-SRE>
 *  @date   2015/10/27 miyazaki DCM-35G_ハードソフト仕様書(Zynq版).docx 1.13対応
 *                              6-3-6 I2C-1 PORT制御とSFP制御(ESD試験時のI2C-1アクセスNG救済フロー追記)
 */
/********************************************************************************************************************/
int bpf_hm_devc_i2c_stat_chk_wa(unsigned int kind)
{
	unsigned int	regAddr;
	unsigned int	regData;
	unsigned int	bitifno;
	
	switch(kind){
		case BPF_HM_DEVC_I2C_0:
			regAddr = BPF_HM_DEVC_REG_I2C0_BASE;
			bitifno = 0x200;
			break;
		case BPF_HM_DEVC_I2C_1:
			regAddr = BPF_HM_DEVC_REG_I2C1_BASE;
			bitifno = 0x400;
			break;
		default:
			return BPF_HM_DEVC_COMPLETE;
			break;
	}

	(void)BPF_HM_DEVC_REG_READ(0,regAddr + 0x0004, &regData);
	if((regData & 0x100) != 0){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "I2C Busy!!! EEROR 0x%x", regData );

		(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,regAddr, &regData);

		BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, BPF_HM_DEVC_REG_RST_LPD_IOU2, bitifno );

		BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, BPF_HM_DEVC_REG_RST_LPD_IOU2, bitifno );

		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,regAddr, &regData);

		(void)BPF_HM_DEVC_REG_READ(0,regAddr + 0x0004, &regData);

		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Last status 0x%x", regData );

	}

	return BPF_HM_DEVC_COMPLETE;
}
/* @} */




/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  読み込み範囲チェック（レジスタ）
 *  @note   オフセットの範囲をチェックし、ファイルポインタ等の情報を取得する。
 *  @param  offset         [in]   レジスタ読み込みオフセット
 *  @param  read_offset    [out]  readオフセット
 *  @param  size           [out]  サイズ
 *  @param  map_discriptor [out]  マップディスクリプタ
 *  @param  mutex          [out]  mutexオブジェクト
 *  @param  type           [out]  アクセスタイプ
 *  @param  
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/12/12 ALPHA)Yokoyama    docomo SLC/1CHIP対応
 */
/********************************************************************************************************************/
int bpf_hm_dev_reg_info(	volatile unsigned int offset,
							volatile int* read_offset, 
							volatile int* size, 
							volatile char** map_descriptor,
							volatile pthread_mutex_t  **mutex,
							volatile int* access_type )
{
	int							count;
	t_bpf_hm_devc_fd_info		*info;
	t_bpf_hm_devc_info			*info2;
	
	for( count = BPF_HM_DEVC_COM_LAST ; count < (sizeof(di_devc_data_fd)/sizeof(t_bpf_hm_devc_fd_info)) ; count++ )
	{
		/* 装置で有効か否か判断	*/
		/* Startオフセットが0の場合もSKIP	*/
		if( ( 0 == di_devc_data_fd.info[count].validflg) ||
			( 0 == di_devc_data_fd.info[count].startoffset ) )
		{
			continue;
		}
		
		info = &di_devc_data_fd.info[count];
		info2 =&di_devc_data_p->info[count];
		
		/* 指定OFFSETの場所のデータを取得する	*/
		if( (info->startoffset <= offset) && ((info->startoffset + info->filesize) > offset) )
		{
			if((offset >= BPF_HM_DEVC_PCI_AREA_START) && (offset <= BPF_HM_DEVC_PCI_AREA_END)){
				if(di_devc_data_p->pl_config == DEVC_FLG_OFF){
					bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "this offset(0x%08x) cannot access because PCI init not finish yet", offset );
					return BPF_HM_DEVC_NG;
				}
			}
			*size = info->filesize;
			*read_offset = offset - info->startoffset;
			*map_descriptor = info->map;
			*mutex = &info2->mutex;
			*access_type = info->access_type;
			
			return BPF_HM_DEVC_COMPLETE;
		}
	}
	
	bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "this offset(0x%08x) cannot access", offset );
	return BPF_HM_DEVC_NG;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  読み込み範囲チェック（I/O Peripheral）
 *  @note   オフセットの範囲をチェックし、ファイルポインタ等の情報を取得する。
 *  @param  offset         [in]   レジスタ読み込みオフセット
 *  @param  read_offset    [out]  readオフセット
 *  @param  size           [out]  サイズ
 *  @param  map_discriptor [out]  マップディスクリプタ
 *  @param  mutex          [out]  mutexオブジェクト
 *  @param  type           [out]  アクセスタイプ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/12/12 ALPHA)Yokoyama    docomo SLC/1CHIP対応 
 */
/********************************************************************************************************************/
int bpf_hm_dev_io_info( volatile unsigned int offset, 
						volatile int* read_offset,
						volatile int* size,
						volatile char** map_descriptor,
						volatile pthread_mutex_t  **mutex,
						volatile int* access_type )
{
	/* bpf_hm_dev_reg_infoに統合する	*/
	return bpf_hm_dev_reg_info( offset, read_offset, size, map_descriptor, mutex, access_type );
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタREAD(2byte)
 *  @note   指定レジスタの情報を読み出す
 *  @param  map_discriptor [in]   マップディスクリプタ
 *  @param  offset         [in]   オフセット
 *  @param  size           [in]   サイズ
 *  @param  data_p         [out]  読み出しデータ
 *  @return none
 *  @date   2014/12/04 ALPHA)Yokoyama create
 */
/********************************************************************************************************************/
void bpf_hm_dev_reg_read_2byte(	volatile char			*map_descriptor,
								volatile int			offset,
								volatile int			size,
								unsigned int			*data_p )
{
	unsigned short			*data_p_s;
	
	*data_p = 0;
	data_p_s = (unsigned short *)data_p;
	
	/* 同期する	*/
	msync( (void*)map_descriptor, size, MS_SYNC );
	/* 2byteアクセス	*/
	/* データを設定する */
	*data_p_s = *((unsigned short*)(map_descriptor+offset));
	
	return;
}

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタREAD(4byte)
 *  @note   指定レジスタの情報を読み出す
 *  @param  map_discriptor [in]   マップディスクリプタ
 *  @param  offset         [in]   オフセット
 *  @param  size           [in]   サイズ
 *  @param  data_p         [out]  読み出しデータ
 *  @return none
 *  @date   2014/12/10 ALPHA)Yokoyama create
 */
/********************************************************************************************************************/
void bpf_hm_dev_reg_read_4byte(	volatile char			*map_descriptor,
								volatile int			offset,
								volatile int			size,
								unsigned int			*data_p )
{
	/* 同期する	*/
	msync( (void*)map_descriptor, size, MS_SYNC );
	/* 4byteアクセス	*/
	/* データを設定する */
	*data_p = *((unsigned *)(map_descriptor+offset));
	
	return;
}

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタREAD
 *  @note   指定レジスタの情報を読み出す
 *  @param  access_type    [in]   アクセスタイプ
 *  @param  map_discriptor [in]   マップディスクリプタ
 *  @param  offset         [in]   オフセット
 *  @param  size           [in]   サイズ
 *  @param  data_p         [out]  読み出しデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2014/12/10 ALPHA)Yokoyama create
 */
/********************************************************************************************************************/
int bpf_hm_dev_reg_read_simple(	volatile int			access_type,
								volatile char			*map_descriptor,
								volatile int			offset,
								volatile int			size,
								unsigned int			*data_p )
{
	if( BPF_HM_DEVC_ACCESS_4BYTE == access_type )
	{
		/* 4byteアクセス	*/
		/* データを設定する */
		bpf_hm_dev_reg_read_4byte( map_descriptor, offset, size, data_p );
	}
	else if( BPF_HM_DEVC_ACCESS_2BYTE == access_type )
	{
		/* 2byteアクセス	*/
		bpf_hm_dev_reg_read_2byte( map_descriptor, offset, size, data_p );
	}
	else
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "register access type ng = %d", access_type);
		return BPF_HM_DEVC_NG;
	}
	
	return BPF_HM_DEVC_COMPLETE;
}

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタREAD
 *  @note   指定レジスタの情報を読み出す
 *  @param  access_type    [in]   アクセスタイプ
 *  @param  map_discriptor [in]   マップディスクリプタ
 *  @param  offset         [in]   オフセット
 *  @param  size           [in]   サイズ
 *  @param  mutex          [in]   mutexオブジェクト
 *  @param  data_p         [out]  読み出しデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2014/12/04 ALPHA)Yokoyama create
 */
/********************************************************************************************************************/
int bpf_hm_dev_reg_read(	volatile int				access_type,
							volatile char				*map_descriptor,
							volatile int				offset,
							volatile int				size,
							volatile pthread_mutex_t	*mutex,
							unsigned int				*data_p )
{
	int					ret = BPF_HM_DEVC_COMPLETE;
	
	ret = bpf_hm_devc_mutex_lock( (pthread_mutex_t *)mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", mutex,errno);
	    return BPF_HM_DEVC_MUTEX_NG;
	}
	
	/* レジスタREADする	*/
	ret = bpf_hm_dev_reg_read_simple( access_type, map_descriptor, offset, size, data_p );
	
	/* Mutex Unlock */
	pthread_mutex_unlock((pthread_mutex_t *)mutex);
	
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタWRITE(2byte)
 *  @note   指定レジスタへ情報を書き込む
 *  @param  map_discriptor [in]   マップディスクリプタ
 *  @param  offset         [in]   オフセット
 *  @param  size           [in]   サイズ
 *  @param  data_p         [in]   設定データ
 *  @return none
 *  @date   2014/12/10 ALPHA)Yokoyama create
 */
/********************************************************************************************************************/
void bpf_hm_dev_reg_write_2byte(	volatile char		*map_descriptor,
									volatile int		offset,
									volatile int		size,
									unsigned int		*data_p )
{
	/* 2byteアクセス	*/
	/* データを設定する */
	*((unsigned short*)(map_descriptor+offset)) = (unsigned short)*data_p;
	
	/* 同期する	*/
	msync( (void*)map_descriptor, size, MS_SYNC );
	
	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタWRITE(4byte)
 *  @note   指定レジスタへ情報を書き込む
 *  @param  map_discriptor [in]   マップディスクリプタ
 *  @param  offset         [in]   オフセット
 *  @param  size           [in]   サイズ
 *  @param  data_p         [in]   設定データ
 *  @return none
 *  @date   2014/12/10 ALPHA)Yokoyama create
 */
/********************************************************************************************************************/
void bpf_hm_dev_reg_write_4byte(	volatile char		*map_descriptor,
									volatile int		offset,
									volatile int		size,
									unsigned int		*data_p )
{
	/* 4byteアクセス	*/
	/* データを設定する */
	*((unsigned *)(map_descriptor+offset)) = *data_p;
	
	/* 同期する	*/
	msync( (void*)map_descriptor, size, MS_SYNC );
	
	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタWRITE
 *  @note   指定レジスタへ情報を書き込む
 *  @param  access_type    [in]   アクセスタイプ
 *  @param  map_discriptor [in]   マップディスクリプタ
 *  @param  offset         [in]   オフセット
 *  @param  size           [in]   サイズ
 *  @param  data_p         [in]   設定データ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2014/12/04 ALPHA)Yokoyama create
 */
/********************************************************************************************************************/
int bpf_hm_dev_reg_write_simple(	volatile int		access_type,
									volatile char		*map_descriptor,
									volatile int		offset,
									volatile int		size,
									unsigned int		*data_p )
{
	if( BPF_HM_DEVC_ACCESS_4BYTE == access_type )
	{
		/* 4byteアクセス	*/
		bpf_hm_dev_reg_write_4byte( map_descriptor, offset, size, data_p );
	}
	else if( BPF_HM_DEVC_ACCESS_2BYTE == access_type )
	{
		/* 2byteアクセス	*/
		bpf_hm_dev_reg_write_2byte( map_descriptor, offset, size, data_p );
	}
	else
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "register access type ng = %d", access_type);
		return BPF_HM_DEVC_NG;
	}
	
	return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタWRITE
 *  @note   指定レジスタへ情報を書き込む
 *  @param  access_type    [in]   アクセスタイプ
 *  @param  map_discriptor [in]   マップディスクリプタ
 *  @param  offset         [in]   オフセット
 *  @param  size           [in]   サイズ
 *  @param  mutex          [in]   mutexオブジェクト
 *  @param  data_p         [in]   設定データ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2014/12/04 ALPHA)Yokoyama create
 */
/********************************************************************************************************************/
int bpf_hm_dev_reg_write(	volatile int				access_type,
							volatile char				*map_descriptor,
							volatile int				offset,
							volatile int				size,
							volatile pthread_mutex_t	*mutex,
							unsigned int				*data_p )
{
	int				ret;
	
	ret = bpf_hm_devc_mutex_lock( (pthread_mutex_t *)mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", mutex,errno);
	    return BPF_HM_DEVC_MUTEX_NG;
	}
	
	/* レジスタWRITEする	*/
	ret = bpf_hm_dev_reg_write_simple( access_type, map_descriptor, offset, size, data_p );
	
	/* Mutex Unlock */
	pthread_mutex_unlock((pthread_mutex_t *)mutex);
	
	return ret;
}

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  読み込み範囲チェック（IRQ）
 *  @note   オフセットの範囲をチェックし、ファイルポインタ等の情報を取得する。
 *  @param  dev     [in]   デバイス名
 *  @param  fd      [out]  ファイルディスクリプタ
 *  @param  data_p  [out]  mutexオブジェクト
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/12/12 ALPHA)Yokoyama    docomo SLC/1CHIP対応 
 */
/********************************************************************************************************************/
int bpf_hm_dev_irq_info(unsigned int dev,int* file_descriptor,pthread_mutex_t  **mutex)
{
	int							count;
	t_bpf_hm_devc_fd_info		*info;
	t_bpf_hm_devc_info			*info2;
	
	for( count = 0; count < (sizeof(di_devc_data_fd)/sizeof(t_bpf_hm_devc_fd_info)) ; count++ )
	{
		if( 0 == di_devc_data_fd.info[count].validflg )
		{
			/* 無効なDeviceの場合は別を探す	*/
			continue;
		}
		
		/* IRQ番号が一致したら情報を返す	*/
		if( dev == di_devc_data_fd.info[count].irq_no )
		{
			info = &di_devc_data_fd.info[ count ];
			info2 =&di_devc_data_p->info[ count ];

			*file_descriptor = info->fd;
			*mutex = &info2->mutex;
			
			return BPF_HM_DEVC_COMPLETE;
		}
	}
	
	/* 見つからなければNG	*/
	return BPF_HM_DEVC_NG;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  delay処理関数
 *  @note   引数で指定された時間分delay処理を行う。
 *  @param  sec   [in]  delay時間(単位:秒)
 *  @param  nsec  [in]  delay時間(単位:ナノ秒)
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2014/12/05 ALPHA)yokoyama create
 */
/********************************************************************************************************************/
int bpf_hm_devc_delay( int sec, int nsec)
{
    struct timespec  dtime;                         /* 中断時間               */
    struct timespec  rem;                           /* 残り中断時間           */

    /* 初期化処理 */
    dtime.tv_sec = sec;
    dtime.tv_nsec = nsec;

    /* 指定秒数delay */
    while(1){
        if( nanosleep(&dtime,&rem) == 0 ){
            break;
        }
        /* 0以外が返ってきたら異常終了 */
        if( errno == EINTR ){ /* 割り込みによる中断なので残り時間を再設定 */
            /* 念のため残り時間をチェックしておく */
            if( rem.tv_sec <= 0 && rem.tv_nsec <= 0 ){
                break;        /* 残り時間0なので満了したことにしとく */
            }
            /* 残り時間を再設定する */
            dtime.tv_sec = rem.tv_sec;
            dtime.tv_nsec = rem.tv_nsec;
        } else {

            bpf_com_dbglog_fmt( BPF_COM_LOG_ERR_L3,"bpf_hm_devc_delay returned NG. errno = %d", errno);
            /* 割り込み以外で異常終了した場合はNGを返す */
            return BPF_HM_DEVC_NG;
        }
    }

    return BPF_HM_DEVC_COMPLETE;
}

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  I2C READ
 *  @note   I2C READを行う。
 *  @param  kind			[in]	I2C種別
 *  @param  setpage			[in]	setpage data
 *  @param  offset			[in]	offset
 *  @param  count			[in]	読み込み数
 *  @param  access_type		[in]	アクセスタイプ
 *  @param  datap			[out]	読み込みData
 *  @return int
 *  @date   2013/12/06 ALPHA)Yokoyama create
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応
 */
/********************************************************************************************************************/
/* 201508 TDD_RRE_ZYNQ memo 
 *  TDD_RRE_ZYNQにともなって、SFPが Primary/Secondary の２個となる。
 *  そのため、本関数の中で行っているenable/disable処理も Primary/Secondaryの区別をつける
 */
int bpf_hm_i2c_read(	unsigned int 	kind,
						int				setpage,
						unsigned short	offset,
						unsigned int	count,
						unsigned int	access_type,
						unsigned char	*data_p )
{
	int file_descriptor;                            /* ファイルディスクリプタ */
	pthread_mutex_t  *mutex;                        /* mutexオブジェクト      */
    int ret;                                        /* 戻り値                 */
    char offset_buf[1];                             /* 書き込みデータ         */
	unsigned int readcount;
	unsigned int readsize;
	unsigned char* read_p;
	
	file_descriptor = di_devc_data_fd.info[ kind ].fd;    /* ファイルディスクリプタ */
	mutex = &di_devc_data_p->info[ kind ].mutex;          /* mutexオブジェクト      */
	
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock(mutex);

	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
			"pthread_mutex_lock mutex = %08x, error = %d",
			mutex,errno);

		return BPF_HM_DEVC_MUTEX_NG;
	}

	ret = bpf_hm_devc_i2c_stat_chk_wa(kind);
	if(ret == BPF_HM_DEVC_NG)
	{
		/* Mutex Unlock */
		pthread_mutex_unlock(mutex);
		/* i2c stack detect and recovery action done */
		return BPF_HM_DEVC_NG;
	}

	/* ioctl */
	/* 注意：setpage パラメータは SFP の primary/secondary を区別できるようにするため 0x60/0x61を使い
	 * 実際にioctlで渡す値と一致していない。
	 * そのため、secondary指定の場合は primary指定と同じ値に変更して渡す。
	 */
	ret = ioctl(file_descriptor, I2C_SLAVE,
					(( setpage == (BPF_HM_DEVC_SFP_BASE_ADD_SA0>>8) || (setpage == BPF_HM_DEVC_SFP_BASE_ADD_SA2>>8) )?
						setpage - 0x10	:	/* secondaryの場合 50/51 -> 60/61 でsetpageが設定されている */ 
						setpage));
	if (ret == BPF_HM_DEVC_SYS_NG)
	{
		/* Mutex Unlock */
		pthread_mutex_unlock(mutex);
		
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
			"ioctl. file_descriptor = %d, setpage = 0x%02x,offset = %04x, error = %d",
			file_descriptor,setpage,offset,errno);
		
		/* ioctl失敗 */
		return BPF_HM_DEVC_NG;
	}
	
	/************************************************
	* file read processing.                         *
	************************************************/
	/* 先頭1バイトに書き込み先のアドレスを指定 */
    offset_buf[0] = offset & 0x00FF;
	ret = write(	file_descriptor,				/* ファイルディスクリプタ (I) */
					offset_buf	,					/* 書き込みデータ         (I) */ 
					1);								/* 書き込みサイズ         (I) */
	
	/* file writeの結果を判定する */
	if (ret == BPF_HM_DEVC_SYS_NG || ret == BPF_HM_DEVC_SIZE_ZERO)
	{
		/* Mutex Unlock */
		pthread_mutex_unlock(mutex);
		
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
			"write. file_descriptor = %d, count_result = %d, error = %d",
			file_descriptor, ret,errno);
		
		/* file write失敗 */
		return BPF_HM_DEVC_NG;
	}
	read_p = data_p;
	
	/* アクセスタイプによって読み込みサイズが異なる	*/
	/* 1byteアクセスの場合	*/
	if( BPF_HM_DEVC_ACCESS_1BYTE == access_type )
	{
		readsize = 1;
	}
	/* 2byteアクセスの場合	*/
	else
	{
		readsize = 2;
	}
	
	for(readcount = 0 ; readcount < count ; readcount++ , read_p+=readsize)
	{
		/************************************************
		* file read processing.                         *
		************************************************/
		/* システムコール file readを実行する。*/
		/* 読み出しは常に1byte単位とする(エラッタ対策) */
		ret = read(	file_descriptor,	/* ファイルディスクリプタ (I) */ 
				read_p,					/* 読み出しデータ格納先 (I/O) */ 
				readsize);				/* 読み出しサイズ         (I) */

		/* file readの結果を判定する */
		if (ret == BPF_HM_DEVC_SYS_NG)
		{
			/* Mutex Unlock */
			pthread_mutex_unlock(mutex);

			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
			    "read. file_descriptor = %d, count_result = %d, error = %d",
			    file_descriptor, ret,errno);

			/* file read失敗 */
			return BPF_HM_DEVC_NG;
		}
	}
	/* Mutex Unlock */
	pthread_mutex_unlock(mutex);
	
    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  I2C READ(mutex取得無し)
 *  @note   I2C READ(mutex取得無し)を行う。
 *  @param  kind			[in]	I2C種別
 *  @param  setpage			[in]	setpage data
 *  @param  offset			[in]	offset
 *  @param  count			[in]	読み込み数
 *  @param  access_type		[in]	アクセスタイプ
 *  @param  datap			[out]	読み込みData
 *  @return int
 *  @date   2013/12/06 ALPHA)Yokoyama create
 */
/********************************************************************************************************************/
int bpf_hm_i2c_read_no_mutex(	unsigned int 	kind,
								int				setpage,
								unsigned short	offset,
								unsigned int	count,
								unsigned int	access_type,
								unsigned char	*data_p )
{
	int file_descriptor;                            /* ファイルディスクリプタ */
    int ret;                                        /* 戻り値                 */
    char offset_buf[1];                             /* 書き込みデータ         */
	unsigned int readcount;
	unsigned int readsize;
	unsigned char* read_p;
	
	/* MUTEX LOCK無しで単純なREADを行う	*/
	/* 基本的にはSFP(I2C#1) READ⇒MMI CPLD(I2C#1)でのSFP有効設定時にしかCALLされない	*/
	
	file_descriptor = di_devc_data_fd.info[ kind ].fd;    /* ファイルディスクリプタ */
	
	ret = bpf_hm_devc_i2c_stat_chk_wa(kind);
	if(ret == BPF_HM_DEVC_NG)
	{
		/* i2c stack detect and recovery action done */
		return BPF_HM_DEVC_NG;
	}
	
	/* ioctl */
	ret = ioctl(file_descriptor, I2C_SLAVE,
					(( setpage == (BPF_HM_DEVC_SFP_BASE_ADD_SA0>>8) || (setpage == BPF_HM_DEVC_SFP_BASE_ADD_SA2>>8) )?
						setpage - 0x10	:	/* secondaryの場合 50/51 -> 60/61 でsetpageが設定されている */ 
						setpage));
	if (ret == BPF_HM_DEVC_SYS_NG)
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
			"ioctl. file_descriptor = %d, setpage = 0x%02x,offset = %04x, error = %d",
			file_descriptor,setpage,offset,errno);
		
		/* ioctl失敗 */
		return BPF_HM_DEVC_NG;
	}
	
	/************************************************
	* file read processing.                         *
	************************************************/
	/* 先頭1バイトに書き込み先のアドレスを指定 */
    offset_buf[0] = offset & 0x00FF;
	ret = write(	file_descriptor,				/* ファイルディスクリプタ (I) */
					offset_buf	,					/* 書き込みデータ         (I) */ 
					1);								/* 書き込みサイズ         (I) */
	
	/* file writeの結果を判定する */
	if (ret == BPF_HM_DEVC_SYS_NG || ret == BPF_HM_DEVC_SIZE_ZERO)
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
			"write. file_descriptor = %d, count_result = %d, error = %d",
			file_descriptor, ret,errno);
		
		/* file write失敗 */
		return BPF_HM_DEVC_NG;
	}
	read_p = data_p;
	
	/* アクセスタイプによって読み込みサイズが異なる	*/
	/* 1byteアクセスの場合	*/
	if( BPF_HM_DEVC_ACCESS_1BYTE == access_type )
	{
		readsize = 1;
	}
	/* 2byteアクセスの場合	*/
	else
	{
		readsize = 2;
	}
	
	for(readcount = 0 ; readcount < count ; readcount++ , read_p+=readsize)
	{
		/************************************************
		* file read processing.                         *
		************************************************/
		/* システムコール file readを実行する。*/
		/* 読み出しは常に1byte単位とする(エラッタ対策) */
		ret = read(	file_descriptor,	/* ファイルディスクリプタ (I) */ 
				read_p,					/* 読み出しデータ格納先 (I/O) */ 
				readsize);				/* 読み出しサイズ         (I) */

		/* file readの結果を判定する */
		if (ret == BPF_HM_DEVC_SYS_NG)
		{
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
			    "read. file_descriptor = %d, count_result = %d, error = %d",
			    file_descriptor, ret,errno);

			/* file read失敗 */
			return BPF_HM_DEVC_NG;
		}
	}
	
    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  I2C WRITE
 *  @note   I2C WRITEを行う。
 *  @param  kind			[in]	I2C種別
 *  @param  setpage			[in]	setpage data
 *  @param  offset			[in]	offset
 *  @param  datap			[in]	書き込みData
 *  @param  size			[in]	書き込みsize
 *  @return int
 *  @date   2013/12/06 ALPHA)Yokoyama create
 *  @date   2015/08/04 ALPHA)Ueda M-RRU-ZSYS-01654対応
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応 
 */
/********************************************************************************************************************/
int bpf_hm_i2c_write(	unsigned int 	kind,
						int				setpage,
						unsigned short	offset,
						unsigned char	*data_p,
						unsigned short	size )
{
    int file_descriptor;                            /* ファイルディスクリプタ */
    ssize_t count_result;                           /* バイト数               */
    pthread_mutex_t  *mutex;                        /* mutexオブジェクト      */
    char writedata[3];                              /* 書き込みデータ         */
    int ret;                                        /* 戻り値                 */
    unsigned int data;                              /* レジスタデータ         */
	unsigned int value;
	
    /* 初期化処理 */
    count_result = 0;
    ret = 0;
	data = 0;

	value = BPF_HM_DEVC_EEP_WRITE_PROTECT_MASK;

    /************************************************
    * file write processing.                        *
    ************************************************/
    /* システムコール file writeを実行 */
    writedata[0] = offset & 0x00FF;
	switch(size){
	case 1:
	    writedata[1] = data_p[0] & 0xFF;
		break;
		
	case 2:
		writedata[1] = (*(UINT*)data_p >> 8) & 0xFF;
		writedata[2] =  *(UINT *)data_p & 0xFF;
		break;
	default:
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "size error = %d", size );
		return BPF_HM_DEVC_NG;
	}
    
    file_descriptor = di_devc_data_fd.info[kind].fd;    /* ファイルディスクリプタ */
    mutex = &di_devc_data_p->info[kind].mutex;          /* mutexオブジェクト      */

    /* Mutex lock */
    ret = bpf_hm_devc_mutex_lock(mutex);
    if( ret != 0 ){
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "pthread_mutex_lock mutex = %08x, error = %d",
            mutex,errno);

        return BPF_HM_DEVC_MUTEX_NG;
    }
	
	/* EEPROM WRITE PROTECTを外す	*/
	/* I2C0の方には温度センサもぶら下がっているがWRITEは無いので無条件でWRITE PROTECTを外す	*/
	/* 処理終了時はPROTECTを再度掛けること													*/
	if(kind == BPF_HM_DEVC_I2C_0)
	{
		/* EEPROM WriteProtect OFF*/
		(void)BPF_HM_DEVC_GPIO_READ(D_RRH_LOG_REG_LV_READ, BPF_HM_DEVC_EEP_WRITE_PROTECT_ADDR, &data);
		data &= ~value;
		(void)BPF_HM_DEVC_GPIO_WRITE(D_RRH_LOG_REG_LV_WRITE, BPF_HM_DEVC_EEP_WRITE_PROTECT_ADDR, &data);
	}

	ret = bpf_hm_devc_i2c_stat_chk_wa(kind);
	if(ret == BPF_HM_DEVC_NG)
	{
		/* Mutex Unlock */
		pthread_mutex_unlock(mutex);
		/* i2c stack detect and recovery action done */
		return BPF_HM_DEVC_NG;
	}
		
    /* ioctl */
	/* 注意：setpage パラメータは SFP の primary/secondary を区別できるようにするため 0x60/0x61を使い
	 * 実際にioctlで渡す値と一致していない。
	 * そのため、secondary指定の場合は primary指定と同じ値に変更して渡す。
	 */
	ret = ioctl(file_descriptor, I2C_SLAVE,
					(( setpage == (BPF_HM_DEVC_SFP_BASE_ADD_SA0>>8) || setpage == (BPF_HM_DEVC_SFP_BASE_ADD_SA2>>8) )?
						setpage - 0x10	:	/* secondaryの場合 50/51 -> 60/61 でsetpageが設定されている */ 
						setpage));			/* 以外はそのまま */
    if (ret == BPF_HM_DEVC_SYS_NG){

		if(kind == BPF_HM_DEVC_I2C_0)
		{
			/* EEPROM WriteProtect ON*/
			(void)BPF_HM_DEVC_GPIO_READ(D_RRH_LOG_REG_LV_READ, BPF_HM_DEVC_EEP_WRITE_PROTECT_ADDR, &data);
			data |= value;
			(void)BPF_HM_DEVC_GPIO_WRITE(D_RRH_LOG_REG_LV_WRITE, BPF_HM_DEVC_EEP_WRITE_PROTECT_ADDR, &data);
		}

        /* Mutex Unlock */
        pthread_mutex_unlock(mutex);

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "ioctl. file_descriptor = %d, setpage = 0x%02x,offset = %04x, error = %d",
            file_descriptor,setpage,offset,errno);

        /* ioctl失敗 */
        return BPF_HM_DEVC_NG;
    }
    
    count_result = write(file_descriptor,                /* ファイルディスクリプタ (I) */
                         &writedata         ,            /* 書き込みデータ         (I) */ 
                         (size + 1) );                   /* 書き込みサイズ(アドレス部(1)+書き込みデータサイズ)         (I) */

    /* file writeの結果を判定する */
    if (count_result == BPF_HM_DEVC_SYS_NG || count_result == BPF_HM_DEVC_SIZE_ZERO){

		if(kind == BPF_HM_DEVC_I2C_0)
		{
			/* EEPROM WriteProtect ON*/
			(void)BPF_HM_DEVC_GPIO_READ(D_RRH_LOG_REG_LV_READ, BPF_HM_DEVC_EEP_WRITE_PROTECT_ADDR, &data);
			data |= value;
			(void)BPF_HM_DEVC_GPIO_WRITE(D_RRH_LOG_REG_LV_WRITE, BPF_HM_DEVC_EEP_WRITE_PROTECT_ADDR, &data);
		}

        /* Mutex Unlock */
        pthread_mutex_unlock(mutex);

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "write. file_descriptor = %d, count_result = %d, error = %d",
            file_descriptor, count_result,errno);

        /* file write失敗 */
        return BPF_HM_DEVC_NG;
    }
	
    /* 10mSec delay */
	/* 以下の処理を入れないと、WRITE後の即READでNGになる	*/
	/* ドライバ側で保障してもらえない模様					*/
	ret = bpf_hm_devc_delay( 0, BPF_HM_DEVC_WAIT_TO_10MS );
	
	if(kind == BPF_HM_DEVC_I2C_0)
	{
		/* EEPROM WriteProtect ON*/
		(void)BPF_HM_DEVC_GPIO_READ(D_RRH_LOG_REG_LV_READ, BPF_HM_DEVC_EEP_WRITE_PROTECT_ADDR, &data);
		data |= value;
		(void)BPF_HM_DEVC_GPIO_WRITE(D_RRH_LOG_REG_LV_WRITE, BPF_HM_DEVC_EEP_WRITE_PROTECT_ADDR, &data);
	}
    /* Mutex Unlock */
    pthread_mutex_unlock(mutex);

    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  I2C WRITE(mutex lock無し)
 *  @note   I2C WRITE(mutex lock無し)を行う。
 *  @param  kind			[in]	I2C種別
 *  @param  setpage			[in]	setpage data
 *  @param  offset			[in]	offset
 *  @param  datap			[in]	書き込みData
 *  @param  size			[in]	書き込みsize
 *  @return int
 *  @date   2013/12/06 ALPHA)Yokoyama create
 
 */
/********************************************************************************************************************/
int bpf_hm_i2c_write_no_mutex(	unsigned int 	kind,
								int				setpage,
								unsigned short	offset,
								unsigned char	*data_p,
								unsigned short	size )
{
    int file_descriptor;                            /* ファイルディスクリプタ */
    ssize_t count_result;                           /* バイト数               */
    char writedata[3];                              /* 書き込みデータ         */
    int ret;                                        /* 戻り値                 */

	/* MUTEX LOCK無しで単純なREADを行う	*/
	/* 基本的にはSFP(I2C#1) READ⇒MMI CPLD(I2C#1)でのSFP有効設定時にしかCALLされない	*/
	
    /* 初期化処理 */
    count_result = 0;
    ret = 0;

    /************************************************
    * file write processing.                        *
    ************************************************/
    /* システムコール file writeを実行 */
    writedata[0] = offset & 0x00FF;
	switch(size){
	case 1:
	    writedata[1] = data_p[0] & 0xFF;
		break;
		
	case 2:
		writedata[1] = (*(UINT*)data_p >> 8) & 0xFF;
		writedata[2] =  *(UINT *)data_p & 0xFF;
		break;
	default:
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "size error = %d", size );
		return BPF_HM_DEVC_NG;
	}
    
    file_descriptor = di_devc_data_fd.info[kind].fd;    /* ファイルディスクリプタ */

	ret = bpf_hm_devc_i2c_stat_chk_wa(kind);
	if(ret == BPF_HM_DEVC_NG)
	{
		/* i2c stack detect and recovery action done */
		return BPF_HM_DEVC_NG;
	}

    /* ioctl */
    ret = ioctl(file_descriptor, I2C_SLAVE, setpage );
    if (ret == BPF_HM_DEVC_SYS_NG){

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "ioctl. file_descriptor = %d, setpage = 0x%02x,offset = %04x, error = %d",
            file_descriptor,setpage,offset,errno);

        /* ioctl失敗 */
        return BPF_HM_DEVC_NG;
    }
    
    count_result = write(file_descriptor,                /* ファイルディスクリプタ (I) */
                         &writedata         ,            /* 書き込みデータ         (I) */ 
                         (size + 1) );                   /* 書き込みサイズ(アドレス部(1)+書き込みデータサイズ)         (I) */

    /* file writeの結果を判定する */
    if (count_result == BPF_HM_DEVC_SYS_NG || count_result == BPF_HM_DEVC_SIZE_ZERO){

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "write. file_descriptor = %d, count_result = %d, error = %d",
            file_descriptor, count_result,errno);

        /* file write失敗 */
        return BPF_HM_DEVC_NG;
    }
	
    /* 10mSec delay */
	/* 以下の処理を入れないと、WRITE後の即READでNGになる	*/
	/* ドライバ側で保障してもらえない模様					*/
	ret = bpf_hm_devc_delay( 0, BPF_HM_DEVC_WAIT_TO_10MS );
	
    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PS接続QSPI Upper/Lower切り替え
 *  @note   PS接続QSPI Upper/Lower切り替えを行う。
 *  @param  side [in] Upper/Lower指定
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_QSPI_ULSELNG
 *  @date   2014/04/03 HID)Iwabuchi create
 *  @date   2014/12/12 ALPHA)Yokoyama    docomo SLC/1CHIP対応
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_sect_set(unsigned long side)
{
    int          cnt;
    int          cnt_side_check;
    int          rtn;
    unsigned int status;
    unsigned int config;
    unsigned int tx_dat;
    unsigned int rx_dat;
    int          success_flg;

    /* QSPI RX FIFO初期化 */
    cnt = 0;
    while( cnt < 252 ) {
        rtn = BPF_HM_DEVC_REG_READ(0,ZYNQ_QSPI_INTR_STATUS_REG, &status);
        if ( rtn != BPF_HM_DEVC_COMPLETE ) {
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }
        if ( (status & 0x00000010) == 0) {
            break;
        }
        cnt += 1;
    }

    if (cnt >= 252) {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "bpf_hm_devc_qspi_sect_set RX FIFO err");
        return BPF_HM_DEVC_QSPI_ULSELNG;
    }

    cnt_side_check = 0;
    success_flg = 0;
    
    while(1){
        /* write enable コマンド送信 */
        tx_dat = BPF_HM_DEVC_PLQSPI_WRITE_ENABLE;
        rtn = BPF_HM_DEVC_REG_WRITE(0,ZYNQ_QSPI_TXD1, &tx_dat);
        if ( rtn != BPF_HM_DEVC_COMPLETE ) {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_qspi_sect_set write enable NG");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }

        /* CS送信 */
        rtn = BPF_HM_DEVC_REG_READ(0,ZYNQ_QSPI_CONFIG_REG, &config);
        if ( rtn != BPF_HM_DEVC_COMPLETE ) {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_qspi_sect_set CS send NG");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }
        config |= 0x00010000;    //CS start
        rtn = BPF_HM_DEVC_REG_WRITE(0,ZYNQ_QSPI_CONFIG_REG, &config);
        if ( rtn != BPF_HM_DEVC_COMPLETE ) {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_qspi_sect_set CS start NG");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }

        /* データ受信 */
        cnt = 0;
        while( cnt <= 100 ) {
            rtn = BPF_HM_DEVC_REG_READ(0,ZYNQ_QSPI_INTR_STATUS_REG, &status);
            if ( rtn != BPF_HM_DEVC_COMPLETE ) {
                bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                    "bpf_hm_devc_qspi_sect_set data rcv NG");
                return BPF_HM_DEVC_QSPI_ULSELNG;
            }
            if ( (status & 0x00000010) == 0) {
                break;
            }
            cnt += 1;
        }
        if (cnt > 100) {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_qspi_sect_set RX FIFO timeout");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }

        rtn = BPF_HM_DEVC_REG_READ(0,ZYNQ_QSPI_RX_DATA_REG, &rx_dat);
        if ( rtn != BPF_HM_DEVC_COMPLETE ) {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_qspi_sect_set reg read NG");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }

        /* write extended address register コマンド送信 */
        tx_dat = BPF_HM_DEVC_PLQSPI_WRITE_EXT_ADR | ((side << 8) & 0x0000FF00);
        rtn = BPF_HM_DEVC_REG_WRITE(0,ZYNQ_QSPI_TXD2, &tx_dat);
        if ( rtn != BPF_HM_DEVC_COMPLETE ) {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_qspi_sect_set write extended address register NG");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }

        /* CS送信 */
        rtn = BPF_HM_DEVC_REG_READ(0,ZYNQ_QSPI_CONFIG_REG, &config);
        if ( rtn != BPF_HM_DEVC_COMPLETE ) {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_qspi_sect_set CS send2 NG");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }
        config |= 0x00010000;    //CS start
        rtn = BPF_HM_DEVC_REG_WRITE(0,ZYNQ_QSPI_CONFIG_REG, &config);
        if ( rtn != BPF_HM_DEVC_COMPLETE ) {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_qspi_sect_set CS start2 NG");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }

        /* データ受信 */
        cnt = 0;
        while( cnt <= 10000 ) {
            rtn = BPF_HM_DEVC_REG_READ(0,ZYNQ_QSPI_INTR_STATUS_REG, &status);
            if ( rtn != BPF_HM_DEVC_COMPLETE ) {
                bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                    "bpf_hm_devc_qspi_sect_set data rcv2 NG");
                return BPF_HM_DEVC_QSPI_ULSELNG;
            }
            if ( (status & 0x00000010) == 0) {
                break;
            }
            cnt += 1;
        }
        if (cnt > 10000) {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_qspi_sect_set RX FIFO timeout");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }

        rtn = BPF_HM_DEVC_REG_READ(0,ZYNQ_QSPI_RX_DATA_REG, &rx_dat);
        if ( rtn != BPF_HM_DEVC_COMPLETE ) {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_qspi_sect_set reg read2 NG");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }

        /* read extended address register コマンド送信(debug) */
        tx_dat = 0xC8;
        rtn = BPF_HM_DEVC_REG_WRITE(0,ZYNQ_QSPI_TXD2, &tx_dat);
        if ( rtn != BPF_HM_DEVC_COMPLETE ) {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_qspi_sect_set debug write NG");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }

        /* CS送信 */
        rtn = BPF_HM_DEVC_REG_READ(0,ZYNQ_QSPI_CONFIG_REG, &config);
        if ( rtn != BPF_HM_DEVC_COMPLETE ) {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_qspi_sect_set CS send3 NG");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }
        config |= 0x00010000;    //CS start
        rtn = BPF_HM_DEVC_REG_WRITE(0,ZYNQ_QSPI_CONFIG_REG, &config);
        if ( rtn != BPF_HM_DEVC_COMPLETE ) {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_qspi_sect_set CS start3 NG");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }

        /* データ受信 */
        cnt = 0;
        while( cnt <= 10000 ) {
            rtn = BPF_HM_DEVC_REG_READ(0,ZYNQ_QSPI_INTR_STATUS_REG, &status);
            if ( rtn != BPF_HM_DEVC_COMPLETE ) {
                bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                    "bpf_hm_devc_qspi_sect_set reg read3 NG");
                return BPF_HM_DEVC_QSPI_ULSELNG;
            }
            if ( (status & 0x00000010) == 0) {
                break;
            }
            cnt += 1;
        }
        if (cnt > 10000) {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_qspi_sect_set RX FIFO timeout");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }
    
        rtn = BPF_HM_DEVC_REG_READ(0,ZYNQ_QSPI_RX_DATA_REG, &rx_dat);
        if ( rtn != BPF_HM_DEVC_COMPLETE ) {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_qspi_sect_set reg read4 NG");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }

        bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"expanded address register = 0x%08x",
                            (unsigned int)rx_dat);

        rx_dat = rx_dat >> 24;

        if ( rx_dat != side )
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "Upper/Lower verify NG. write value = %d, read value = %d",side, rx_dat);
            success_flg = 0; // 失敗するとフラグを落とす 
        }
        else{
            if( success_flg == 1 ) break; // 2回連続成功なら抜ける
            else success_flg = 1;        // 1回の成功ではゆるさない
        }

        cnt_side_check++;

        if ( cnt_side_check > 10 )
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_qspi_sect_set retry NG");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }

        rtn = bpf_hm_devc_delay( 0, BPF_HM_DEVC_WAIT_TO_100MS); /* 100ms wait */
        if (rtn != BPF_HM_DEVC_COMPLETE)
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_qspi_sect_set delay NG");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }
    }

    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  QSPI-FLASHアクセス領域指定(ioctl使用版)
 *  @note   PS接続QSPIの面切り替え、Upper/Lower切り替えを行う。
 *  @param  file_descriptor    [in]  file descriptor
 *  @param  face    [in]   Flash面
 *  @param  offset  [in]   Flash書き込みオフセット
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @retval BPF_HM_DEVC_QSPI_ULSELNG
 *  @date   2014/11/19 FUJITSU)ando B3対応(ioctl対応) 
 *  @date   2014/12/12 ALPHA)Yokoyama    docomo SLC/1CHIP対応
 */
/********************************************************************************************************************/
int bpf_hm_devc_select_qspi_area2(int file_descriptor, unsigned int face,unsigned int offset)
{
    unsigned int data;    /* レジスタデータ         */
    int ret;              /* 戻り値                 */
    int cnt_side_check;
    unsigned char w_ul;
    erase_info_t        ei;

	/* 16MがMAX	*/
	if( offset >= BPF_HM_DEVC_FLASH_UL_BOUND )
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "bpf_hm_devc_select_qspi_area2 offset NG. offset = %d", offset);
		/* 範囲外 */
		return BPF_HM_DEVC_NG;
	}
	
    /* 状態判定 */
    if( ( offset >= BPF_HM_DEVC_FLASH_UL_BOUND ) && 
        ( offset < BPF_HM_DEVC_FLASH_UL_BOUND + BPF_HM_DEVC_PLQSPI_BOUNDARY_ADD ) ){

        /* Upper選択 */
        w_ul = 1;
    }
    else if ( offset < BPF_HM_DEVC_FLASH_UL_BOUND ){
        /* Lower選択 */
        w_ul = 0;
    }
    else{

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "bpf_hm_devc_select_qspi_area2 offset NG. offset = %d", offset);
        /* 範囲外 */
        return BPF_HM_DEVC_NG;
    }

    if( (di_devc_data_p->qspi_side == face ) && ( di_devc_data_p->qspi_uplw == w_ul) ){

        /* 指定面、Upper/Lowerが保持する状態と一致する場合、処理を行わない */
        return BPF_HM_DEVC_COMPLETE;
    }

    /* QSPI状態情報初期化 */
    di_devc_data_p->qspi_side = 0xFFFF;
    di_devc_data_p->qspi_uplw = 0xFF;

    /* B3で変更
        QPSI接続Flash ABとCDが物理的に分離

        切替手順：1. SS0/1切替(Zynq LQPSI_CFG(U_PAGE) bit28へのアクセス)
                  2. 0/1面切替実施(Flashコマンド)

         0面 1面
        +---+---+
        | A | B |SS1
        +---+---+   
        +---+---+   
        | C | D |SS0
        +---+---+
    */
    /************************************************
    * SS0/SS1切替                                   *
    ************************************************/

    /* ZNYQのSPI/LQSPI_CFGレジスタの読み出し */
    ret = BPF_HM_DEVC_REG_READ( 0,ZYNQ_QSPI_LQSPI_CFG, &data );
    if (ret != BPF_HM_DEVC_COMPLETE) {

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "BPF_HM_DEVC_REG_READ NG. address = %08x, ret = %d",
            ZYNQ_QSPI_LQSPI_CFG, ret);

        /* データ取得失敗 */
        return BPF_HM_DEVC_NG;
    }

	/* qspi.LQSPI_CFG[TWO_MEM](bit30) = 0 */
	data &= ~BPF_HM_DEVC_QSPI_TWO_MEM_BIT;

    /* qspi.LQSPI_CFG[U_PAGE](bit28) => SS0=0, SS1=1 */
    if( w_ul == 0 ) data &= ~BPF_HM_DEVC_QSPI_CHIP_SEL_BIT;
    else            data |= BPF_HM_DEVC_QSPI_CHIP_SEL_BIT;

    /* ZNYQのSPI/LQSPI_CFGレジスタの書き戻し */
    ret = BPF_HM_DEVC_REG_WRITE( 0,ZYNQ_QSPI_LQSPI_CFG, &data );
    if (ret != BPF_HM_DEVC_COMPLETE) {

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "BPF_HM_DEVC_REG_WRITE NG. address = %08x, ret = %d",
            ZYNQ_QSPI_LQSPI_CFG, ret);

        /* データ取得失敗 */
        return BPF_HM_DEVC_NG;
    }

    /************************************************
    * 0/1面切替                                     *
    ************************************************/
    cnt_side_check = 0;

    /* Flashコマンド送信 */
    while(1){
        ei.start = 0;
        ei.length = face;
        ret = ioctl(file_descriptor, MEMLOCK,   &ei );


        if( ret != BPF_HM_DEVC_COMPLETE ) {

            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_select_qspi_area2 NG. face= %d",
                face);

            /* コマンド発行失敗 */
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }


        ret = ioctl(file_descriptor, MEMUNLOCK,   &ei );

        if( ei.length != ret ) {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                    "Upper/Lower verify NG. write value = %d, read value = %d",ei.length, ret);
        } else {
            break;
        }

        cnt_side_check++;

        if ( cnt_side_check > 10 )
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_select_qspi_area2 retry NG");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }

        ret = bpf_hm_devc_delay( 0, BPF_HM_DEVC_WAIT_TO_100MS ); /* 100ms wait */
        if (ret != BPF_HM_DEVC_COMPLETE)
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "bpf_hm_devc_select_qspi_area2 delay NG");
            return BPF_HM_DEVC_QSPI_ULSELNG;
        }

    }


    di_devc_data_p->qspi_side = face;
    di_devc_data_p->qspi_uplw = w_ul;

    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PS接続SPIアクセスコマンド
 *  @note   PS接続SPIアクセスコマンド を行う。
 *  @param  bus     [in]   bus
 *  @param  cs      [in]   cs:0-2
 *  @param  dat_len [in]   tx_datのデータ数(コマンド、アドレスも含む)
 *  @param  tx_dat  [in]   送信データの先頭アドレス
 *  @param  rx_dat  [in]   受信データの格納先頭アドレス(ulong)
 *  @param  cs_cnt  [in]   cs制御有り/1 cs制御最初無し/2 cs制御最後無し/3 cs制御無し
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 */
/********************************************************************************************************************/
int bpf_hm_devc_spi_ps_spi(	unsigned int bus,
							unsigned int cs,
							unsigned int dat_len,
							unsigned int *tx_dat,
							unsigned int *rx_dat,
							unsigned short cs_cnt,
							unsigned int configSpeed)
{
	int	 fd = -1;
	char name[256];
	unsigned char		tx_buf[16];
	unsigned char		rx_buf[16];
	int					cnt;
	unsigned char		mode;

	memset( tx_buf, 0x00, sizeof(tx_buf) );
	memset( rx_buf, 0x00, sizeof(rx_buf) );

	/* 4byte型を1byte型に入れ替え */
	for (cnt = 0; cnt < dat_len; cnt++) {
		tx_buf[cnt] = (unsigned char)(tx_dat[cnt] & 0x000000FF);
	}
	
	struct spi_ioc_transfer spi_data = 
	{
		.tx_buf = (unsigned long)tx_buf,
		.rx_buf = (unsigned long)rx_buf,
		.len = dat_len,
		.delay_usecs = 0,
		.speed_hz = 12500000,
		.bits_per_word = 8,
	};

	/* bus/csの組み合わせでopenするデバイスを選択 */
	if (bus == BPF_HM_DEVC_SPI_SPI0) {
		switch (cs) {
		case BPF_HM_DEVC_SPI_CS0:	// PTP
			snprintf (name, sizeof(name), "%s", BPF_HM_DEVC_SPI00);
			/*	CLK速度設定を12.5M→6.25Mに変更	*//*	ハソ仕様書v1.09対応	*/
			spi_data.speed_hz = 6250000;
			break;
		case BPF_HM_DEVC_SPI_CS1:	// JC-PLL
			snprintf (name, sizeof(name), "%s", BPF_HM_DEVC_SPI01);
			break;
		case BPF_HM_DEVC_SPI_CS2:	// RF-PLL
			snprintf (name, sizeof(name), "%s", BPF_HM_DEVC_SPI02);
			break;
		default :
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "spi bus0 csNG:%d", cs);
			return BPF_HM_DEVC_NG;
		}
	} else 
	if (bus == BPF_HM_DEVC_SPI_SPI1) {
		switch (cs) {
		case BPF_HM_DEVC_SPI_CS0:	// RFIC#1
			snprintf (name, sizeof(name), "%s", BPF_HM_DEVC_SPI10);
			break;
		case BPF_HM_DEVC_SPI_CS1:	// RFIC#2
			snprintf (name, sizeof(name), "%s", BPF_HM_DEVC_SPI11);
			break;
		default :
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "spi bus1 csNG:%d", cs);
			return BPF_HM_DEVC_NG;
		}
	} else
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "spi busNG:%d", bus);
		return BPF_HM_DEVC_NG;
	}
	/* ファイルディスクリプタopen */
	if ((fd = open(name,O_RDWR)) < 0) {
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "spi fd openNG:%d", fd);
		return BPF_HM_DEVC_NG;
	}

	/* モード設定 */
	if (ioctl(fd, SPI_IOC_RD_MODE, &mode) < 0) {
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "spi SPI_IOC_WR_MODE NG");
		close(fd);
		return BPF_HM_DEVC_NG;
	}
	mode &= ~(SPI_CPHA | SPI_CPOL);
	if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) {
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "spi SPI_IOC_WR_MODE NG");
		close(fd);
		return BPF_HM_DEVC_NG;
	}

	/* 転送ビット設定 */
	if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &(spi_data.bits_per_word)) < 0) {
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "spi SPI_IOC_WR_BITS_PER_WORD NG");
		close(fd);
		return BPF_HM_DEVC_NG;
	}

	/* 転送スピード設定 */
	if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &(spi_data.speed_hz)) < 0) {
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "spi SPI_IOC_WR_MAX_SPEED_HZ NG");
		close(fd);
		return BPF_HM_DEVC_NG;
	}

	/* データアクセス */
	if (ioctl(fd, SPI_IOC_MESSAGE(1), &spi_data) < 0) {
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "spi SPI_IOC_MESSAGE NG");
		close(fd);
		return BPF_HM_DEVC_NG;
	}

	/* ファイルディスクリプタclose */
	close(fd);

	/* 1byte型を4byte型に入れ替え */
	for (cnt = 0; cnt < dat_len; cnt++) {
		rx_dat[cnt] = (unsigned int)rx_buf[cnt];
	}

    return BPF_HM_DEVC_COMPLETE;
}
/* @} */



/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Trx spi busy check
 *  @return int
 *  @date   2019/02/18 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static int bpf_hm_devc_spi_pl_spi_busychk(void){

	unsigned int loop = 0;
	unsigned int bsy_port_data = 0;
	int ret;

	while(1){
		ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_PORT_OFFSET, &bsy_port_data);
		if (ret != BPF_HM_DEVC_COMPLETE){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Port read NG = %08x",bsy_port_data);
			return BPF_HM_DEVC_NG;
		}
		if((bsy_port_data & 0x00020000) == 0){
			break;
		}
		bpf_hm_devc_delay(0,1000000);
		loop++;
		if(loop > 1000){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Port Busy. data = %08x",bsy_port_data);
			return BPF_HM_DEVC_NG;
		}
	}
	return BPF_HM_DEVC_COMPLETE;
}
/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PS接続SPIアクセスコマンド
 *  @note   PS接続SPIアクセスコマンド を行う。
 *  @param  offset      [in]   書き込み先のオフセット
 *  @param  dat_len     [in]   tx_datのデータ数(コマンド、アドレスも含む)
 *  @param  tx_dat      [in]   送信データの先頭アドレス
 *  @param  target_type [in]   ターゲット種別(Tx_DAC = BPF_HM_DEVC_SPI_TYPE_TXDAC(0) or Rx_DAC = BPF_HM_DEVC_SPI_TYPE_RXADC(1))
 *  @param  rx_dat      [in]   読み込みデータの格納先先頭アドレス
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create

 */
/********************************************************************************************************************/
int bpf_hm_devc_spi_pl_spi( unsigned int dat_len,
                            unsigned int *tx_dat,
                            unsigned int target_type,
                            unsigned int *rx_dat)
{
    unsigned int target_data;               /* 汎用SPI ターゲット設定データ */
    unsigned int cnt;
    unsigned int *tx_data;
    unsigned int *rx_data;
    tx_data = tx_dat;
    rx_data = rx_dat;

    volatile int ret;                       /* 戻り値                       */
    unsigned int is_rx_judge_bit;           /* read / write処理判定用bit    */
    is_rx_judge_bit = 0x08000000;           /* b[27] = "1!                  */
   

	if((ret = bpf_hm_devc_spi_pl_spi_busychk()) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"1st busy check NG");
		return BPF_HM_DEVC_NG;
	}

    /* 汎用SPI クロック設定 */
    ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_CLOCK_OFFSET, 0x0ffffff0);
    if (ret != BPF_HM_DEVC_COMPLETE)
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "Offset Range NG. offset = %08x",
            BPF_HM_DEVC_SPI_CLOCK_OFFSET);
        /* データ取得失敗 */
        return BPF_HM_DEVC_NG;
    }

    /* 汎用SPI イネーブル設定 */
    ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_EN_OFFSET, 0xc0000003);
    if (ret != BPF_HM_DEVC_COMPLETE)
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "Offset Range NG. offset = %08x",
            BPF_HM_DEVC_SPI_EN_OFFSET);
        /* データ取得失敗 */
        return BPF_HM_DEVC_NG;
    }

    /* 汎用SPI 出力制御設定 */
    if((*tx_data & is_rx_judge_bit) != is_rx_judge_bit)
    {
        /* write処理 */
        ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_TX_DATA_OFFSET, 0xffffffff);
    }
    else
    {
        /* read処理 */
        ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_TX_DATA_OFFSET, 0xfffff000);
    }
    if (ret != BPF_HM_DEVC_COMPLETE)
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "Offset Range NG. offset = %08x",
            BPF_HM_DEVC_SPI_TX_DATA_OFFSET);
        /* データ取得失敗 */
        return BPF_HM_DEVC_NG;
    }

    for (cnt = 0; cnt < dat_len; cnt++)
    {
        /* 汎用SPI 送信データ設定 */
        ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_SEND_DATA_OFFSET, *tx_data);
        if (ret != BPF_HM_DEVC_COMPLETE)
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "Offset Range NG. offset = %08x",
                BPF_HM_DEVC_SPI_SEND_DATA_OFFSET);
            /* データ取得失敗 */
            return BPF_HM_DEVC_NG;
        }

        /* Tx_DAC */
        if(target_type == BPF_HM_DEVC_SPI_TYPE_TXDAC)
        {
            target_data = BPF_HM_DEVC_SPI_TYPE_TXDAC_OFFSET;
        }
        /* Rx_DAC */
        else
        {
            target_data = BPF_HM_DEVC_SPI_TYPE_RXADC_OFFSET;
        }
        /* 汎用SPI ターゲット設定 */
        ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PORT_OFFSET, target_data);
        if (ret != BPF_HM_DEVC_COMPLETE)
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                "Offset Range NG. offset = %08x",
                BPF_HM_DEVC_SPI_PORT_OFFSET);
            /* データ取得失敗 */
            return BPF_HM_DEVC_NG;
        }

		if((ret = bpf_hm_devc_spi_pl_spi_busychk()) != BPF_HM_DEVC_COMPLETE){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"2nd busy check NG");
			return BPF_HM_DEVC_NG;
		}


        /* read処理 */
        if((*tx_data & is_rx_judge_bit) == is_rx_judge_bit)
        {
            /* 汎用SPI Read値取得 */
            ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_SPI_RX_DATA_OFFSET, rx_data);
            if (ret != BPF_HM_DEVC_COMPLETE)
            {
                bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                    "Offset Range NG. offset = %08x",
                    BPF_HM_DEVC_SPI_RX_DATA_OFFSET);
                /* データ取得失敗 */
                return BPF_HM_DEVC_NG;
            }
            *rx_data = (*rx_data >> 4) & 0x000000FF;
        }
    }

    /* 汎用SPI 出力制御設定 */
    ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_TX_DATA_OFFSET, 0x00000000);
    if (ret != BPF_HM_DEVC_COMPLETE)
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "Offset Range NG. offset = %08x",
            BPF_HM_DEVC_SPI_TX_DATA_OFFSET);
        /* データ取得失敗 */
        return BPF_HM_DEVC_NG;
    }

    return ret;
}
/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PS接続SPIアクセスコマンド FAN DAC向け
 *  @note   PS接続SPIアクセスコマンド を行う。
 *  @param  offset      [in]   書き込み先のオフセット
 *  @param  dat_len     [in]   tx_datのデータ数(コマンド、アドレスも含む)
 *  @param  tx_dat      [in]   送信データの先頭アドレス
 *  @param  target_type [in]   ターゲット種別(Tx_DAC = BPF_HM_DEVC_SPI_TYPE_TXDAC(0) or Rx_DAC = BPF_HM_DEVC_SPI_TYPE_RXADC(1))
 *  @param  rx_dat      [in]   読み込みデータの格納先先頭アドレス
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2019/02/18 FJT)Taniguchi create

 */
/********************************************************************************************************************/
int bpf_hm_devc_spi_pl_spi_fandac( unsigned int data)
{
	int ret;
	unsigned int	write_data;


	if((ret = bpf_hm_devc_spi_pl_spi_busychk()) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"1st busy check NG");
		return BPF_HM_DEVC_NG;
	}

	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_CLOCK_OFFSET, 0x0ffffff0);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. offset = %08x",BPF_HM_DEVC_SPI_CLOCK_OFFSET);
		return BPF_HM_DEVC_NG;
	}

	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_EN_OFFSET, 0xff0000ff);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. offset = %08x",BPF_HM_DEVC_SPI_EN_OFFSET);
		return BPF_HM_DEVC_NG;
	}

	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_TX_DATA_OFFSET, 0xffffffff);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. offset = %08x",BPF_HM_DEVC_SPI_TX_DATA_OFFSET);
		return BPF_HM_DEVC_NG;
	}

	write_data = 0;
	write_data |= (data & 0xFFF) << 10;
	
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_SEND_DATA_OFFSET, write_data);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. offset = %08x",BPF_HM_DEVC_SPI_SEND_DATA_OFFSET);
		return BPF_HM_DEVC_NG;
	}

	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_PORT_OFFSET, BPF_HM_DEVC_SPI_TYPE_FANADC_OFFSET);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. offset = %08x",BPF_HM_DEVC_SPI_PORT_OFFSET);
		return BPF_HM_DEVC_NG;
	}

	if((ret = bpf_hm_devc_spi_pl_spi_busychk()) != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"2nd busy check NG");
		return BPF_HM_DEVC_NG;
	}
	
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_SPI_TX_DATA_OFFSET, 0x00000000);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. offset = %08x",BPF_HM_DEVC_SPI_TX_DATA_OFFSET);
		return BPF_HM_DEVC_NG;
	}

	return BPF_HM_DEVC_COMPLETE;
}

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PS接続SPIレジスタ読み出し
 *  @note   PS接続SPIレジスタ読み出し を行う。
 *  @param  offset      [in]   読み出し先のオフセット
 *  @param  data_p      [in]   読み出しデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create

 */
/********************************************************************************************************************/
int bpf_hm_devc_spi_pl_spi_reg_read( unsigned int offset,
                                     unsigned int *data_p)
{
    volatile int read_offset;               /* readオフセット               */
    volatile int size;                      /* サイズ                       */
    volatile int ret;                       /* 戻り値                       */
    volatile pthread_mutex_t  *mutex;       /* mutexオブジェクト            */
    volatile char *map_descriptor;          /* マップディスクリプタ         */
    volatile int access_type;               /* アクセスタイプ               */

    /* オフセット情報から、取得対象のデータの情報を取得する */
    ret = bpf_hm_dev_io_info(offset, &read_offset, &size, &map_descriptor, &mutex, &access_type);
    if (ret != BPF_HM_DEVC_COMPLETE)
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "Offset Range NG. offset = %08x",
            offset);

        /* データ取得失敗 */
        return BPF_HM_DEVC_NG;
    }
    /* レジスタREAD関数CALL */
    ret = bpf_hm_dev_reg_read( access_type, map_descriptor, read_offset, size, mutex, data_p );
    if( ret != 0 )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, offset );
        return BPF_HM_DEVC_NG;
    }

	/* 頻繁にコールされるため、D_RRH_LOG_REG_LV_READ_BUSY */
    BPF_COM_LOG_REG( D_RRH_LOG_REG_LV_READ_BUSY, access_type, offset, *data_p, 0 );

    return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PS接続SPIレジスタ書き込み
 *  @note   PS接続SPIレジスタ書き込み を行う。
 *  @param  offset      [in]   書き込み先のオフセット
 *  @param  data        [in]   書き込みデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 */
/********************************************************************************************************************/
int bpf_hm_devc_spi_pl_spi_reg_write( unsigned int offset,
                                      unsigned int data)
{
    volatile int read_offset;               /* readオフセット               */
    volatile int size;                      /* サイズ                       */
    volatile int ret;                       /* 戻り値                       */
    volatile pthread_mutex_t  *mutex;       /* mutexオブジェクト            */
    volatile char *map_descriptor;          /* マップディスクリプタ         */
    volatile int access_type;               /* アクセスタイプ               */
    unsigned int *data_p;
    data_p = &data;

    /* オフセット情報から、取得対象のデータの情報を取得する */
    ret = bpf_hm_dev_io_info(offset, &read_offset, &size, &map_descriptor, &mutex, &access_type);
    if (ret != BPF_HM_DEVC_COMPLETE)
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "Offset Range NG. offset = %08x",
            offset);
        /* データ取得失敗 */
        return BPF_HM_DEVC_NG;
    }
    /* レジスタWRITE関数CALL    */
    ret = bpf_hm_dev_reg_write( access_type, map_descriptor, read_offset, size, mutex, data_p );
    if( ret != 0 )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_write_error ret=%d addr=0x%08x", ret, offset );
        return BPF_HM_DEVC_NG;
    }

	/* 頻繁にコールされるため、D_RRH_LOG_REG_LV_WRITE_POL */
	BPF_COM_LOG_REG( D_RRH_LOG_REG_LV_WRITE_POL, access_type, offset, data, 0 );

    return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RF-EEP アクセス
 *  @note   RF-EEP アクセスを行う。
 *  @param  eep_page    [in]    EEPページアドレス
 *  @param  address     [in]    書き込み / 読み出し 先頭アドレス
 *  @param  data_p      [in]    書き込み / 読み出し データアドレス
 *  @param  kind        [in]    書き込み処理 or 読み出し判定(0:読み出し処理, 1:書き込み処理)
 *  @return int
 *  @date   2018/08/02 KCN)Nanjo create
 */
/********************************************************************************************************************/
int bpf_hm_i2c_soft_rfeep( unsigned int eep_page,
                           unsigned int address,
                           unsigned int *data_p,
                           unsigned int kind )
{
    int             ret         = 0;
    unsigned int    val         = 0;
    unsigned int    write_data  = 0;
    unsigned int    write_data_base;
    unsigned int    device_address;
    write_data_base = 0x100;
    device_address  = 0x0A0;

    ret = bpf_hm_devc_reg_read_fpga(BPF_HM_DEVC_I2C_OFFSET2, &val);
    if( ret != BPF_HM_DEVC_COMPLETE )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET2 );
        return ret;
    }
    /* TX_FIFO Empty確認 */
    if((val & BPF_HM_DEVC_SOFTMAC_TX_FIFO) != BPF_HM_DEVC_SOFTMAC_TX_FIFO)
    {
        /* I2C IP Macroソフトリセット  */
        ret = bpf_hm_i2c_soft_reset();
        if( ret != BPF_HM_DEVC_COMPLETE )
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d ", ret);
            return ret;
        }
    }
    /* RX_FIFO Empty確認 */
    if((val & BPF_HM_DEVC_SOFTMAC_RX_FIFO) != BPF_HM_DEVC_SOFTMAC_RX_FIFO)
    {
        /* I2C IP Macroソフトリセット  */
        ret = bpf_hm_i2c_soft_reset();
        if( ret != BPF_HM_DEVC_COMPLETE )
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d ", ret);
            return ret;
        }
    }
    /* I2C bus busy確認 */
    if((val & BPF_HM_DEVC_SOFTMAC_BUSY) == BPF_HM_DEVC_SOFTMAC_BUSY)
    {
        /* I2C IP Macroソフトリセット  */
        ret = bpf_hm_i2c_soft_reset();
        if( ret != BPF_HM_DEVC_COMPLETE )
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d ", ret);
            return ret;
        }
    }

    /* アクセスデバイス選択 */
	val = 0;
    ret = bpf_hm_devc_reg_write_fpga(BPF_HM_DEVC_I2C_OFFSET4, &val);
    if( ret != BPF_HM_DEVC_COMPLETE )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET4 );
        return ret;
    }

    /* Start bit設定 & デバイスアドレス設定 & Writeアクセス設定 */
    write_data = write_data_base | device_address | (eep_page << 1);
    ret = bpf_hm_devc_reg_write_fpga(BPF_HM_DEVC_I2C_OFFSET3, &write_data);
    if( ret != BPF_HM_DEVC_COMPLETE )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET3 );
        return ret;
    }

    /* メモリアドレス設定 */
    ret = bpf_hm_devc_reg_write_fpga(BPF_HM_DEVC_I2C_OFFSET3, &address);
    if( ret != BPF_HM_DEVC_COMPLETE )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET3 );
        return ret;
    }

    /* read処理 */
    if(kind == BPF_HM_DEVC_I2C_READ)
    {
        /* Start bit設定 & デバイスアドレス設定 & Writeアクセス設定 */
        write_data = write_data_base | device_address | (eep_page << 1) | 0x1;
        ret = bpf_hm_devc_reg_write_fpga(BPF_HM_DEVC_I2C_OFFSET3, &write_data);
        if( ret != BPF_HM_DEVC_COMPLETE )
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET3 );
            return ret;
        }

        /* Stop bit設定 & リードバイト設定 */
        write_data = 0x200 | BPF_HM_DEVC_I2C_READ_SIZE;
        ret = bpf_hm_devc_reg_write_fpga(BPF_HM_DEVC_I2C_OFFSET3, &write_data);
        if( ret != BPF_HM_DEVC_COMPLETE )
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET3 );
            return ret;
        }
    }
    /* write処理 */
    else
    {
        write_data = *data_p & 0xFF;
        /* Stop-bit設定 */
        write_data |= 0x200;
        /* Writeデータ設定 */
        ret = bpf_hm_devc_reg_write_fpga(BPF_HM_DEVC_I2C_OFFSET3, &write_data);
        if( ret != BPF_HM_DEVC_COMPLETE )
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET3 );
            return ret;
        }
    }

    /* I2Cイネーブル */
	ret = bpf_hm_devc_reg_read_fpga(BPF_HM_DEVC_I2C_OFFSET1, &write_data);
    if( ret != BPF_HM_DEVC_COMPLETE )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET1 );
        return ret;
    }
    write_data = write_data | 0x1;
    ret = bpf_hm_devc_reg_write_fpga(BPF_HM_DEVC_I2C_OFFSET1, &write_data);
    if( ret != BPF_HM_DEVC_COMPLETE )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET1 );
        return ret;
    }

    /* wait 1us */
    usleep(1000);

    /* I2Cアクセス完了 */
    ret = bpf_hm_devc_reg_read_fpga(BPF_HM_DEVC_I2C_OFFSET2, &val);
    if( ret != BPF_HM_DEVC_COMPLETE )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET2 );
        return ret;
    }
    /* I2C bus busy確認 */
    if((val & BPF_HM_DEVC_SOFTMAC_BUSY) == BPF_HM_DEVC_SOFTMAC_BUSY)
    {
        /* I2C IP Macroソフトリセット  */
        ret = bpf_hm_i2c_soft_reset();
        if( ret != BPF_HM_DEVC_COMPLETE )
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d", ret );
            return ret;
        }
    }

    /* readデータ読み出し処理 */
    if(kind == BPF_HM_DEVC_I2C_READ)
    {
        ret = bpf_hm_devc_reg_read_fpga(BPF_HM_DEVC_I2C_OFFSET9, &val);
        if( ret != BPF_HM_DEVC_COMPLETE )
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET9 );
            return ret;
        }
        /* read値設定 */
        *data_p = val;

        
    }

    ret = bpf_hm_devc_reg_read_fpga(BPF_HM_DEVC_I2C_OFFSET2, &val);
    if( ret != BPF_HM_DEVC_COMPLETE )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET2 );
        return ret;
    }
    /* TX_FIFO Empty確認 */
    if((val & BPF_HM_DEVC_SOFTMAC_TX_FIFO) != BPF_HM_DEVC_SOFTMAC_TX_FIFO)
    {
        /* I2C IP Macroソフトリセット  */
        ret = bpf_hm_i2c_soft_reset();
        if( ret != BPF_HM_DEVC_COMPLETE )
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d", ret );
            return ret;
        }
    }
    /* RX_FIFO Empty確認 */
    if((val & BPF_HM_DEVC_SOFTMAC_RX_FIFO) != BPF_HM_DEVC_SOFTMAC_RX_FIFO)
    {
        /* I2C IP Macroソフトリセット  */
        ret = bpf_hm_i2c_soft_reset();
        if( ret != BPF_HM_DEVC_COMPLETE )
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d", ret );
            return ret;
        }
    }
    /* I2C bus busy確認 */
    if((val & BPF_HM_DEVC_SOFTMAC_BUSY) == BPF_HM_DEVC_SOFTMAC_BUSY)
    {
        /* I2C IP Macroソフトリセット  */
        ret = bpf_hm_i2c_soft_reset();
        if( ret != BPF_HM_DEVC_COMPLETE )
        {
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d", ret );
            return ret;
        }
    }

    /* I2Cディセーブル */
    ret = bpf_hm_devc_reg_read_fpga(BPF_HM_DEVC_I2C_OFFSET1, &write_data);
    if( ret != BPF_HM_DEVC_COMPLETE )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET1 );
        return ret;
    }
    write_data = write_data & 0xFFFFFFFE;
    ret = bpf_hm_devc_reg_write_fpga(BPF_HM_DEVC_I2C_OFFSET1, &write_data);
    if( ret != BPF_HM_DEVC_COMPLETE )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET1 );
        return ret;
    }

	if(kind != BPF_HM_DEVC_I2C_READ){
	    /* wait 10ms */
	    usleep(10000);
	}

    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  I2C(SoftMacro)リセット
 *  @note   I2C(SoftMacro)リセットを行う。
 *  @return int
 *  @date   2018/08/02 KCN)Nanjo create
 */
/********************************************************************************************************************/
int bpf_hm_i2c_soft_reset()
{
    int             ret         = 0;
    unsigned int    write_data  = 0;

    /* I2C Soft Reset ON */
    write_data = 0x0000000A;
    ret = bpf_hm_devc_reg_write_fpga(BPF_HM_DEVC_I2C_OFFSET5, &write_data);
    if( ret != BPF_HM_DEVC_COMPLETE )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET5 );
        return ret;
    }

    /* RX FIFO FULL閾値設定 */
    write_data = 0x0000000F;
    ret = bpf_hm_devc_reg_write_fpga(BPF_HM_DEVC_I2C_OFFSET6, &write_data);
    if( ret != BPF_HM_DEVC_COMPLETE )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET6 );
        return ret;
    }

    /* High period of the scl clock */
    write_data = 0x00000076;
    ret = bpf_hm_devc_reg_write_fpga(BPF_HM_DEVC_I2C_OFFSET7, &write_data);
    if( ret != BPF_HM_DEVC_COMPLETE )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET7 );
        return ret;
    }

    /* Low period of the scl clock */
    write_data = 0x00000075;
    ret = bpf_hm_devc_reg_write_fpga(BPF_HM_DEVC_I2C_OFFSET8, &write_data);
    if( ret != BPF_HM_DEVC_COMPLETE )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"reg_read_error ret=%d addr=0x%08x", ret, BPF_HM_DEVC_I2C_OFFSET8 );
        return ret;
    }

    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Trx spi busy check
 *  @return int
 *  @date   2019/02/18 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static int bpf_hm_devc_spi_trx_spi_busychk(void){

	unsigned int loop = 0;
	unsigned int bsy_port_data = 0;
	int ret;

	while(1){
		ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_TRXSPI_PORT_OFFSET, &bsy_port_data);
		if (ret != BPF_HM_DEVC_COMPLETE){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Port read NG = %08x",bsy_port_data);
			return BPF_HM_DEVC_NG;
		}
		if((bsy_port_data & BPF_HM_DEVC_TRXSPI_PORT_ENABLE) == 0){
			break;
		}
		bpf_hm_devc_delay(0,1000000);
		loop++;
		if(loop > 1000){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Port Busy. data = %08x",bsy_port_data);
			return BPF_HM_DEVC_NG;
		}
	}
	return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Trx spi write for sub6
 *  @return int
 *  @date   2019/02/18 FJT)Taniguchi create
 */
/********************************************************************************************************************/
int bpf_hm_devc_spi_trx_spi_write_sub6(
                                  unsigned int target_type,
                                  unsigned char ant,
                                  unsigned int dat)
{
	int	ret;
	unsigned int	wrt_data;

	if((ret = bpf_hm_devc_spi_trx_spi_busychk()) != BPF_HM_DEVC_COMPLETE){
		return BPF_HM_DEVC_NG;
	}
	
	/* 汎用SPIクロック設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_CLOCK_OFFSET, BPF_HM_DEVC_TRXSPI_CLOCK_TxVATT_PA_VGDAC);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_CLOCK_OFFSET");
		return BPF_HM_DEVC_NG;
	}
	
	/* 汎用SPIイネーブル設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_EN_OFFSET, BPF_HM_DEVC_TRXSPI_EN_DAC);
	if (ret != BPF_HM_DEVC_COMPLETE)
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_EN_OFFSET");
		return BPF_HM_DEVC_NG;
	}
	
	/* 汎用SPI出力制御設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_TX_DATA_OFFSET, BPF_HM_DEVC_TRXSPI_TX_DATA_ON);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_TX_DATA_OFFSET");
		return BPF_HM_DEVC_NG;
	}

	wrt_data = 0;
	wrt_data |= ((unsigned int)(ant & 3)) << 22;
	wrt_data |= ((unsigned int)1) << 20;
	wrt_data |= ((unsigned int)(dat & 0x3FF)) << 10;

	/* 汎用SPI送信データ設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_SEND_DATA_OFFSET, wrt_data);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_SEND_DATA_OFFSET");
		return BPF_HM_DEVC_NG;
	}
	
	/* 汎用SPIターゲット設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_PORT_OFFSET, target_type);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_PORT_OFFSET");
		return BPF_HM_DEVC_NG;
	}
	
	if((ret = bpf_hm_devc_spi_trx_spi_busychk()) != BPF_HM_DEVC_COMPLETE){
		return BPF_HM_DEVC_NG;
	}

	/* 汎用SPI出力制御設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_TX_DATA_OFFSET, BPF_HM_DEVC_TRXSPI_TX_DATA_OFF);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_PORT_OFFSET");
		return BPF_HM_DEVC_NG;
	}

	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Trx spi write for mmw
 *  @return int
 *  @date   2019/02/18 FJT)Taniguchi create
 */
/********************************************************************************************************************/
int bpf_hm_devc_spi_trx_spi_write_mmw(
                                  unsigned int target_type,
                                  unsigned int dat)
{

	int	ret;
	unsigned int	clk_wrt_data;
	unsigned int	en_wrt_data;

	switch(target_type){
		case BPF_HM_DEVC_TRXSPI_TARGET_QMOD_V:
		case BPF_HM_DEVC_TRXSPI_TARGET_QDEM_V:
		case BPF_HM_DEVC_TRXSPI_TARGET_QMOD_H:
		case BPF_HM_DEVC_TRXSPI_TARGET_QDEM_H:
			clk_wrt_data = BPF_HM_DEVC_TRXSPI_CLOCK_QMOD;
			en_wrt_data = BPF_HM_DEVC_TRXSPI_EN_QMOD;
			break;
		case BPF_HM_DEVC_TRXSPI_TARGET_MMWQMODDAC:
		case BPF_HM_DEVC_TRXSPI_TARGET_MMWPAVGDAC:
			clk_wrt_data = BPF_HM_DEVC_TRXSPI_CLOCK_DAC;
			en_wrt_data = BPF_HM_DEVC_TRXSPI_EN_DAC;
			break;
		case BPF_HM_DEVC_TRXSPI_TARGET_MMWTXVATTDAC_V:
		case BPF_HM_DEVC_TRXSPI_TARGET_MMWRXVATTDAC_V:
		case BPF_HM_DEVC_TRXSPI_TARGET_MMWTXVATTDAC_H:
		case BPF_HM_DEVC_TRXSPI_TARGET_MMWRXVATTDAC_H:
			clk_wrt_data = BPF_HM_DEVC_TRXSPI_CLOCK_VATTDAC;
			en_wrt_data = BPF_HM_DEVC_TRXSPI_EN_VATTDAC;
			break;
		default:
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"target_type Range NG. 0x%08x",target_type);
			return BPF_HM_DEVC_NG;
			break;
	}


	if((ret = bpf_hm_devc_spi_trx_spi_busychk()) != BPF_HM_DEVC_COMPLETE){
		return BPF_HM_DEVC_NG;
	}

	/* 汎用SPIクロック設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_CLOCK_OFFSET, clk_wrt_data);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_CLOCK_OFFSET");
		return BPF_HM_DEVC_NG;
	}
	
	/* 汎用SPIイネーブル設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_EN_OFFSET, en_wrt_data);
	if (ret != BPF_HM_DEVC_COMPLETE)
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_EN_OFFSET");
		return BPF_HM_DEVC_NG;
	}
	
	/* 汎用SPI出力制御設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_TX_DATA_OFFSET, BPF_HM_DEVC_TRXSPI_TX_DATA_ON);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_TX_DATA_OFFSET");
		return BPF_HM_DEVC_NG;
	}

	/* 汎用SPI送信データ設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_SEND_DATA_OFFSET, dat);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_SEND_DATA_OFFSET");
		return BPF_HM_DEVC_NG;
	}
	
	/* 汎用SPIターゲット設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_PORT_OFFSET, target_type);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_PORT_OFFSET");
		return BPF_HM_DEVC_NG;
	}
	
	if((ret = bpf_hm_devc_spi_trx_spi_busychk()) != BPF_HM_DEVC_COMPLETE){
		return BPF_HM_DEVC_NG;
	}

	/* 汎用SPI出力制御設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_TX_DATA_OFFSET, BPF_HM_DEVC_TRXSPI_TX_DATA_OFF);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_PORT_OFFSET");
		return BPF_HM_DEVC_NG;
	}

	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Trx spi read for mmw
 *  @return int
 *  @date   2019/02/18 FJT)Taniguchi create
 */
/********************************************************************************************************************/
int bpf_hm_devc_spi_trx_spi_read_mmw(
                                  unsigned int target_type,
                                  unsigned int dat,
                                  unsigned int* r_dat)
{
	unsigned int clk_wrt_data;
	unsigned int en_wrt_data;
	unsigned int read_dat;
	int		ret;

	switch(target_type){
		case BPF_HM_DEVC_TRXSPI_TARGET_QMOD_V:
		case BPF_HM_DEVC_TRXSPI_TARGET_QDEM_V:
		case BPF_HM_DEVC_TRXSPI_TARGET_QMOD_H:
		case BPF_HM_DEVC_TRXSPI_TARGET_QDEM_H:
			clk_wrt_data = BPF_HM_DEVC_TRXSPI_CLOCK_QMOD;
			en_wrt_data = BPF_HM_DEVC_TRXSPI_EN_QMOD;
			break;
		default:
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"target_type Range NG. 0x%08x",target_type);
			return BPF_HM_DEVC_NG;
			break;
	}
	if((ret = bpf_hm_devc_spi_trx_spi_busychk()) != BPF_HM_DEVC_COMPLETE){
		return BPF_HM_DEVC_NG;
	}
	/* 汎用SPIクロック設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_CLOCK_OFFSET, clk_wrt_data);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_CLOCK_OFFSET");
		return BPF_HM_DEVC_NG;
	}
	/* 汎用SPIイネーブル設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_EN_OFFSET, en_wrt_data);
	if (ret != BPF_HM_DEVC_COMPLETE)
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_EN_OFFSET");
		return BPF_HM_DEVC_NG;
	}
	/* 汎用SPI出力制御設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_TX_DATA_OFFSET, BPF_HM_DEVC_TRXSPI_TX_READ_DATA_ON);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_TX_DATA_OFFSET");
		return BPF_HM_DEVC_NG;
	}
	/* 汎用SPI送信データ設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_SEND_DATA_OFFSET, dat);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_SEND_DATA_OFFSET");
		return BPF_HM_DEVC_NG;
	}
	/* 汎用SPIターゲット設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_PORT_OFFSET, target_type);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_PORT_OFFSET");
		return BPF_HM_DEVC_NG;
	}
	if((ret = bpf_hm_devc_spi_trx_spi_busychk()) != BPF_HM_DEVC_COMPLETE){
		return BPF_HM_DEVC_NG;
	}
	/* 汎用SPI Read値取得 */
	ret = bpf_hm_devc_spi_pl_spi_reg_read(BPF_HM_DEVC_TRXSPI_RX_DATA_OFFSET, &read_dat);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_RX_DATA_OFFSET");
		return BPF_HM_DEVC_NG;
	}
	*r_dat = (read_dat >> 5) & 0xFFFF;
	/* 汎用SPI出力制御設定 */
	ret = bpf_hm_devc_spi_pl_spi_reg_write(BPF_HM_DEVC_TRXSPI_TX_DATA_OFFSET, BPF_HM_DEVC_TRXSPI_TX_DATA_OFF);
	if (ret != BPF_HM_DEVC_COMPLETE){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Offset Range NG. BPF_HM_DEVC_TRXSPI_PORT_OFFSET");
		return BPF_HM_DEVC_NG;
	}
	return ret;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  pll select
 *  @return N/A
 *  @date   2019/02/21 FJT)Taniguchi create
 */
/********************************************************************************************************************/
void bpf_hm_devc_pll_select(unsigned int select){

	unsigned int				cur_select;

	BPF_HM_DEVC_REG_READ(0,BPF_HM_DEVC_PLL_SELECT_OFFSET,&cur_select);

	if(select != BPF_HM_DEVC_PLL_SELECT_DIS){
		cur_select &= ~BPF_HM_DEVC_PLL_SELECT_DIS;
		cur_select |= select;
	}else{
		cur_select |= select;
	}

	BPF_HM_DEVC_REG_WRITE(0,BPF_HM_DEVC_PLL_SELECT_OFFSET,&cur_select);

	return;
}
/* @} */
/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
