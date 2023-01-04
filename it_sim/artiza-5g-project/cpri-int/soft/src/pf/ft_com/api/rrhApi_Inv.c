/*!
 * @skip  $ld:$
 * @file  rrhApi_Inv.c
 * @brief API : インベントリ関連
 * @date  2013/11/05 FFCS)Xut Create.
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */

/** @addtogroup RRH_API_INV
 *  @{
 */
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "f_rrh_inc.h"
#include "BPF_RU_IPCM.h"
#include "BPF_HM_DEVC.h"
#include "BPF_COM_LOG.h"

#include "rrhApi_Com.h"
#include "rrhApi_Inv.h"
#include "f_rrh_reg_eep.h"

/* 2021/01/19 M&C) Merge 4G FHM src (start add) */
/********************************************************************************************************************/
/**
 *  @brief  API : デバイス情報取得
 *  @note   
 *          Reponse  ID : -
 *          TYPE        : FSM
 *  @param  *data_p     : 取得データポインタ
 *  @return INT
 *  @retval ret
 *  @date   2013/11/05 FFCS)Xut Create.
 *  @FeatureID	PF_Inv-003-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
static VOID* shm_GetDevInfo_p = 0;
E_RRHAPI_RCODE rrhApi_Inv_Fsm_GetDevInfo(	VOID			*data_p,INT size )
{
	INT ret;
	INT errcd;

	/* 共有メモリから外部インターフェース情報を取得する */
	if(shm_GetDevInfo_p == 0)
	{
		ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_DEVICE_MNG_TBL/**/,(VOID**)&shm_GetDevInfo_p,&errcd);
		if(ret != BPF_RU_IPCM_OK)
		return E_API_RCD_NG;
	}
	
	memcpy(data_p,shm_GetDevInfo_p,size);
	/*BPF_RU_IPCM_PROCSHM_ADDRFREE(localdata_p,&errcd);*/

	return E_API_RCD_OK;
}
/* 2021/01/19 M&C) Merge 4G FHM src (end add) */

/********************************************************************************************************************/
/**
 *  @brief  API : デバイスkind取得
 *  @note   
 *          Reponse  ID : -
 *          TYPE        : FSM
 *  @param  *data_p     : 取得データポインタ
 *  @return INT
 *  @retval ret
 *  @date   2013/11/05 FFCS)Xut Create.
 *  @FeatureID	PF_Inv-003-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
static VOID* shm_GetDevKind_p = 0;
E_RRHAPI_RCODE rrhApi_Inv_Fsm_GetDevKind(	VOID			*data_p,INT size )
{
	INT ret;
	INT errcd;

	/* 共有メモリから外部インターフェース情報を取得する */
	if(shm_GetDevKind_p == 0)
	{
		ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_RRH_KIND/**/,(VOID**)&shm_GetDevKind_p,&errcd);
		if(ret != BPF_RU_IPCM_OK)
		return E_API_RCD_NG;
	}
	
	memcpy(data_p,shm_GetDevKind_p,size);
	/*BPF_RU_IPCM_PROCSHM_ADDRFREE(localdata_p,&errcd);*/

	return E_API_RCD_OK;
}


/********************************************************************************************************************/
/**
 *  @brief  API : TRAインベントリ情報取得
 *  @note   
 *          Reponse  ID : -
 *          TYPE        : FSM
 *  @param  *data_p     : 取得データポインタ
 *  @return INT
 *  @retval ret
 *  @date   2013/11/05 FFCS)Xut Create.
 *  @FeatureID	PF_Inv-003-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
static VOID* shm_GetTraInvInfo_p = 0;
E_RRHAPI_RCODE rrhApi_Inv_Fsm_GetTraInvInfo(	VOID	*data_p,INT size )
{
	INT ret;
	INT errcd;

	/* 共有メモリから外部インターフェース情報を取得する */
	if(shm_GetTraInvInfo_p == 0)
	{
		ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY/**/,(VOID**)&shm_GetTraInvInfo_p,&errcd);
		if(ret != BPF_RU_IPCM_OK)
		return E_API_RCD_NG;
	}

	memcpy(data_p,shm_GetTraInvInfo_p,size);
	/*BPF_RU_IPCM_PROCSHM_ADDRFREE(localdata_p,&errcd);*/
	
	return E_API_RCD_OK;
}

/* 2021/01/19 M&C) Merge 4G FHM src (start add) */
/********************************************************************************************************************/
/**
 *  @brief  API : Shelfインベントリ情報取得
 *  @note   PF EVENT ID : 0xA0090007
 *          Reponse  ID : -
 *          TYPE        : FSM
 *  @param  *data_p     : 取得データポインタ
 *  @return INT
 *  @retval ret
 *  @date   2013/11/05 FFCS)Xut Create.
 *  @FeatureID	PF_Inv-003-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
static VOID* shm_GetSlfInvInfo_p = 0;
E_RRHAPI_RCODE rrhApi_Inv_Fsm_GetSlfInvInfo(VOID	*data_p,INT size )
{
	INT ret;
	INT errcd;

	/* 共有メモリから外部インターフェース情報を取得する */
	if(shm_GetSlfInvInfo_p == 0)
	{
		ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_SHELF_INVENTORY/**/,(VOID**)&shm_GetSlfInvInfo_p,&errcd);
		if(ret != BPF_RU_IPCM_OK)
		return E_API_RCD_NG;
	}

	memcpy(data_p,shm_GetSlfInvInfo_p,size);
	/*BPF_RU_IPCM_PROCSHM_ADDRFREE(localdata_p,&errcd);*/
	
	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  API : system parameter情報取得
 *  @note   
 *          Reponse  ID : -
 *          TYPE        : FSM
 *  @param  *data_p     : 取得データポインタ
 *  @return INT
 *  @retval ret
 *  @date   2013/11/05 FFCS)Xut Create.
 *  @FeatureID	PF_Inv-003-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
static VOID* shm_GetSystemPara_p = 0;
E_RRHAPI_RCODE rrhApi_Inv_Fsm_GetSystemPara(VOID *data_p ,INT size)
{
	INT ret;
	INT errcd;

	/* 共有メモリからデバイス情報を取得する */
	if(shm_GetSystemPara_p == 0)
	{
		ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_SYSPARA_INFO/**/,(VOID**)&shm_GetSystemPara_p,&errcd);
		if(ret != BPF_RU_IPCM_OK)
		return E_API_RCD_NG;
	}

	/*semphore take*/
	ret = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_SYSPARA_INFO,BPF_RU_IPCM_LOCK_RW,BPF_RU_IPCM_WAIT,D_RRH_NULL,&errcd);
	memcpy(data_p,shm_GetSystemPara_p,size);
	/*semphore give*/
	ret |= BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_SYSPARA_INFO,BPF_RU_IPCM_LOCK_RW,&errcd);

	/*BPF_RU_IPCM_PROCSHM_ADDRFREE(localdata_p,&errcd);*/
	
	return E_API_RCD_OK;
}
/********************************************************************************************************************/
/**
 *  @brief  API : system parameter情報更新
 *  @param  *data_p     : 取得データポインタ
 *  @param  size        : copy size of system parameter
 *  @param  offset      : not use
 *  @param  efParam     : not use
 *  @param  len         : not use
 *  @return INT
 *  @retval ret
 *  @date   2013/11/05 FFCS)Xut Create.
 *  @date   2015/09/02 TDI)satou EEPROM保存処理を削除
 *  @FeatureID	PF_Inv-002-001-001
 */
/********************************************************************************************************************/

E_RRHAPI_RCODE rrhApi_Inv_Fsm_SetSystemPara(VOID *data_p,INT size,UINT offset,USHORT*efParam,INT len)
{
	INT ret;
	INT errcd;
	
	/*semphore take*/
	/* 共有メモリからデバイス情報を取得する */
	if(shm_GetSystemPara_p == 0)
	{
		ret = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_SYSPARA_INFO/**/,(VOID**)&shm_GetSystemPara_p,&errcd);
		if(ret != BPF_RU_IPCM_OK) {
			return E_API_RCD_NG;
		}
	}
	
	/*semphore take*/
	ret = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_SYSPARA_INFO,BPF_RU_IPCM_LOCK_RW,BPF_RU_IPCM_WAIT,D_RRH_NULL,&errcd);
	memcpy(shm_GetSystemPara_p,data_p,size);
	/*semphore give*/
	ret |= BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_SYSPARA_INFO,BPF_RU_IPCM_LOCK_RW,&errcd);	
	if(ret != BPF_RU_IPCM_OK) {
	    return E_API_RCD_NG;
	}

	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief  API : system parameter EEPROM書き込み
 *  @param  offset      : 変数E先頭からのオフセット
 *  @param  efParam     : 書き込みデータ
 *  @param  len         : 書き込み長
 *  @return INT
 *  @retval ret
 *  @date	2016/03/25 TDI)satou M-RRU-ZSYS-02090 create
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE rrhApi_Inv_Fsm_SetSystemPara_eeprom(UINT offset, USHORT*efParam, INT len)
{
	INT ret;
	UCHAR a_writeData;
	INT index;
	UINT a_eepadr;
	UCHAR data;

	/* SREでは実施していないが、変数E/Fの内容を書き込み中にリセットされてしまうと、中途半端に保存された
	 * システムパラメータを使用してFHMとREの起動を行ってしまうため、一旦、システムパラメータ受信フラグを未受信に書き換えた上で
	 * EEPROMに変数E/Fを保存する. 保存が完了したらシステムパラメータ受信フラグを受信済みに更新する */
	data = 0xFF;
	ret = BPF_HM_DEVC_EEPROM_WRITE( D_RRH_EEP_SYSPARA_FLG1, &data);
	if( BPF_HM_DEVC_COMPLETE != ret )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "SetSystemPara EEPROM WRITE NG 0");
		return E_API_RCD_NG;
	}

	a_eepadr = D_RRH_EEP_RE_PRIORITY_SYS_INFO0 + offset;  /* eeprom address of system parameter priority */
	for(index=0;index<len;index++)
	{
		a_writeData = (UCHAR)((*efParam) >> 8);
		ret = BPF_HM_DEVC_EEPROM_WRITE(a_eepadr, &a_writeData);
		if( BPF_HM_DEVC_COMPLETE != ret )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "SetSystemPara EEPROM WRITE NG 1");
			return E_API_RCD_NG;
		}

		a_writeData = (UCHAR)(*efParam);
		ret = BPF_HM_DEVC_EEPROM_WRITE(a_eepadr+1, &a_writeData);
		if( BPF_HM_DEVC_COMPLETE != ret )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "SetSystemPara EEPROM WRITE NG 2");
			return E_API_RCD_NG;
		}

		efParam++;
		a_eepadr+=2;
	}

	data = 0x00;
	ret = BPF_HM_DEVC_EEPROM_WRITE( D_RRH_EEP_SYSPARA_FLG0, &data);
	if( BPF_HM_DEVC_COMPLETE != ret )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "SetSystemPara EEPROM WRITE NG 3");
		return E_API_RCD_NG;
	}

	data = 0x01;
	ret = BPF_HM_DEVC_EEPROM_WRITE( D_RRH_EEP_SYSPARA_FLG1, &data);
	if( BPF_HM_DEVC_COMPLETE != ret )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "SetSystemPara EEPROM WRITE NG 4");
		return E_API_RCD_NG;
	}

	return E_API_RCD_OK;
}
/* 2021/01/19 M&C) Merge 4G FHM src (end add) */

/********************************************************************************************************************/
/**
 *  @brief  API : 優先システム情報を取得
 *  @note   
 *          Reponse  ID : -
 *          TYPE        : FSM
 *  @param  *data_p     : 取得データポインタ
 *  @return INT
 *  @retval ret
 *  @date   2014/01/06 FFCS)Linb Create.
 *  @FeatureID	RRH-011-012
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/

E_RRHAPI_RCODE rrhApi_Inv_Fsm_GetPrioritySystem(VOID *data_p)
{
	return E_API_RCD_OK;	
}

/********************************************************************************************************************/
/**
 *  @brief  API : Bandwidth情報を設定
 *  @note   
 *          Reponse  ID : -
 *          TYPE        : FSM
 *  @param  value     : 設定値
 *  @return INT
 *  @retval ret
 *  @date   2014/01/06 FFCS)Linb Create.
 *  @FeatureID	RRH-011-012
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/

E_RRHAPI_RCODE rrhApi_Inv_Fsm_SetBandwidth(USHORT value)
{
	return E_API_RCD_OK;
}

/** @} */

