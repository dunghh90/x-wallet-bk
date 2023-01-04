/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_RFEEP.c
 *  @brief  
 *  @date   2018/08/02 KCN)Nanjo create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_HM_DEVC /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "f_du_reg.h"
#include "f_du_reg_oth.h"
#include "bpf_i_hm_devc.h"

/** @addtogroup BPF_HM_DEVC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RF実装チェック
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2019/04/07 FJT)Taniguchi create
 
 */
/********************************************************************************************************************/
static int bpf_hm_devc_rfboard_check( void )
{
	unsigned int data = 1;
	BPF_HM_DEVC_REG_READ_BIT( D_RRH_LOG_REG_LV_HDL_READ, D_DU_REG_OTH_GPIO_DATA_2_RO, D_DU_VAL_SHIFT_L(1,21) , &data );
	if( data != 0){
		return BPF_HM_DEVC_NG;
	}
	return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RF-EEP読み込み
 *  @note   BPF_HM_DEVC_RFEEP_READ
 *  @param  address  [in]   RF-EEP読み込みオフセット
 *  @param  data_p   [out]  読み込みポインタ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_RFEEP_READ(unsigned int address, unsigned int *data_p)
{
    /* Trace log */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_RFEEP_READ address = %04x" ,address);

    int ret = 0;                    /* 戻り値           */
    unsigned int eep_page = 0;      /* RFEEP-ページ番号 */

    /* mmw装置以外の場合NG応答 */
    if(di_devc_data_p->device_kind != D_RRH_EEP_DEV_KIND_5GDU_MMW)
    {
        ret = BPF_HM_DEVC_NG;
        return ret;
    }

	if((ret = bpf_hm_devc_rfboard_check()) == BPF_HM_DEVC_NG){
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"RF board unmount = %04x" ,address);
        return BPF_HM_DEVC_NG;
	}

    /* RFEEP-ページ番号 */
    switch(address & 0xF00)
    {
        case BPF_HM_DEVC_I2C_PAGE_OFFSET0:
            eep_page = BPF_HM_DEVC_I2C_PAGEDATA0;
            break;

        case BPF_HM_DEVC_I2C_PAGE_OFFSET1:
            eep_page = BPF_HM_DEVC_I2C_PAGEDATA1;
            break;

        case BPF_HM_DEVC_I2C_PAGE_OFFSET2:
            eep_page = BPF_HM_DEVC_I2C_PAGEDATA2;
            break;

        case BPF_HM_DEVC_I2C_PAGE_OFFSET3:
            eep_page = BPF_HM_DEVC_I2C_PAGEDATA3;
            break;

        case BPF_HM_DEVC_I2C_PAGE_OFFSET4:
            eep_page = BPF_HM_DEVC_I2C_PAGEDATA4;
            break;

        case BPF_HM_DEVC_I2C_PAGE_OFFSET5:
            eep_page = BPF_HM_DEVC_I2C_PAGEDATA5;
            break;

        case BPF_HM_DEVC_I2C_PAGE_OFFSET6:
            eep_page = BPF_HM_DEVC_I2C_PAGEDATA6;
            break;

        case BPF_HM_DEVC_I2C_PAGE_OFFSET7:
            eep_page = BPF_HM_DEVC_I2C_PAGEDATA7;
            break;

        default:
            ret = BPF_HM_DEVC_NG;
            return ret;
            break;
    }

	address = address & BPF_HM_DEVC_RFEEP_ADDR_MASK ;

	pthread_mutex_t* mutex = &di_devc_data_p->info[ BPF_HM_DEVC_I2C_2 ].mutex;			/* mutexオブジェクト      */
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}

    /* RF-EE読み込み */
    ret = bpf_hm_i2c_soft_rfeep(eep_page, address, data_p, BPF_HM_DEVC_I2C_READ);

	/* Mutex Unlock */
	pthread_mutex_unlock(mutex);

	BPF_COM_PLOG_I2C(E_I2C_LV_READ,address,*data_p,2,0);	/* packet log */

    return ret;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RF-EEP書き込み
 *  @note   BPF_HM_DEVC_RFEEP_WRITE
 *  @param  address  [in]   RF-EEP書き込みオフセット
 *  @param  data     [out]  書き込みデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/08/02 KCN)Nanjo create
 
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_RFEEP_WRITE(unsigned int address, unsigned int data)
{
    /* Trace log */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_RFEEP_WRITE address = %04x" ,address);

    int ret = 0;                    /* 戻り値           */
    unsigned int eep_page = 0;      /* RFEEP-ページ番号 */

    /* mmw装置以外の場合NG応答 */
    if(di_devc_data_p->device_kind != D_RRH_EEP_DEV_KIND_5GDU_MMW)
    {
        ret = BPF_HM_DEVC_NG;
        return ret;
    }

	if((ret = bpf_hm_devc_rfboard_check()) == BPF_HM_DEVC_NG){
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"RF board unmount = %04x %04x" ,address, data);
        return BPF_HM_DEVC_NG;
	}

    /* RFEEP-ページ番号 */
    switch(address & 0xF00)
    {
        case BPF_HM_DEVC_I2C_PAGE_OFFSET0:
            eep_page = BPF_HM_DEVC_I2C_PAGEDATA0;
            break;

        case BPF_HM_DEVC_I2C_PAGE_OFFSET1:
            eep_page = BPF_HM_DEVC_I2C_PAGEDATA1;
            break;

        case BPF_HM_DEVC_I2C_PAGE_OFFSET2:
            eep_page = BPF_HM_DEVC_I2C_PAGEDATA2;
            break;

        case BPF_HM_DEVC_I2C_PAGE_OFFSET3:
            eep_page = BPF_HM_DEVC_I2C_PAGEDATA3;
            break;

        case BPF_HM_DEVC_I2C_PAGE_OFFSET4:
            eep_page = BPF_HM_DEVC_I2C_PAGEDATA4;
            break;

        case BPF_HM_DEVC_I2C_PAGE_OFFSET5:
            eep_page = BPF_HM_DEVC_I2C_PAGEDATA5;
            break;

        case BPF_HM_DEVC_I2C_PAGE_OFFSET6:
            eep_page = BPF_HM_DEVC_I2C_PAGEDATA6;
            break;

        case BPF_HM_DEVC_I2C_PAGE_OFFSET7:
            eep_page = BPF_HM_DEVC_I2C_PAGEDATA7;
            break;

        default:
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "address NG = %08x",address);
            ret = BPF_HM_DEVC_NG;
            return ret;
            break;
    }

	address = address & BPF_HM_DEVC_RFEEP_ADDR_MASK ;

	pthread_mutex_t* mutex = &di_devc_data_p->info[ BPF_HM_DEVC_I2C_2 ].mutex;			/* mutexオブジェクト      */
	/* Mutex lock */
	ret = bpf_hm_devc_mutex_lock(mutex);
	if( ret != 0 ){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "pthread_mutex_lock mutex = %08x, error = %d", *mutex ,errno);
		return BPF_HM_DEVC_MUTEX_NG;
	}

    /* RF-EEP書き込み */
    ret = bpf_hm_i2c_soft_rfeep(eep_page, address, &data, BPF_HM_DEVC_I2C_WRITE);

	/* Mutex Unlock */
	pthread_mutex_unlock(mutex);

	BPF_COM_PLOG_I2C(E_I2C_LV_READ,address,data,2,0);	/* packet log */

    return ret;
}

/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
