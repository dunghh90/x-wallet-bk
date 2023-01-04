/*********************************************************************************/
/**
 *  @skip	$Id:$
 *  @file	f_eio_subFunc.c
 *  @brief  5GDU-LLS externalio fan sv & ctrl sub functions
 *  @date   2019/03/19 FJT)Koshida create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019-
 */
/*********************************************************************************/

/*!
 * @addtogroup RRH_PF_EIO
 * @{
 */
#include "f_eio_header.h"			/* 5GDU-LLS�O��IO�^�X�N�w�b�_�t�@�C��			*/
#include <stdarg.h>

/*!
 *  @brief  This fuction recover FAN Speed fault.
 *  @note   This function recover FAN Speed fault.\n
 *  @param  none 
 *  @return VOID
 *  @retval none
 *  @date   2019/3/20 FJT)Koshida create
 */
VOID f_eio_fanFaultRecover( )
{
	UINT							fanIdx;
	
	/*	FAN#1~3�܂ŌJ��Ԃ�	*/
	for(fanIdx=E_DU_FAN_1; fanIdx<E_DU_FAN_MAXNO; fanIdx++)
	{
		/*	�J�E���^������������	*/
		f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].normalCnt = 0;
		f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].abnormalCnt = 0;
		f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].faultCnt = 0;
		
		/*	FAN��]����Ԃ�fault�̏ꍇ	*/
		if((f_eiow_svctrl_mngp->fanSv.fanSpeedInfo.fanSpeedFault & f_eior_fanSpeedInfo[fanIdx].stsBit) != 0)
		{
			/*	fault����	*/
			f_com_SVCtl(f_eior_fanSpeedInfo[fanIdx].fltId, D_DU_SVCTL_OFF, f_eior_fanSpeedInfo[fanIdx].fltFct);
		}
	}
	/*	FAN#1~3��]��Fault��Ԃ𕜋�	*/
	f_eiow_svctrl_mngp->fanSv.fanSpeedInfo.fanSpeedFault = D_DU_BIT_FAN_ALLOFF;
	
	return;
}


/*!
 *  @brief  This fuction detect to FAN Unit status.
 *  @note   This function detect to FAN Unit status.\n
 *  @param  regVal    [in]   register read value 
 *  @param  *pSvInfo  [in]   detect facter info pointer
 *  @return VOID
 *  @retval none
 *  @date   2019/3/20 FJT)Koshida create
 */
VOID f_eio_fanUnitStsDetect(UINT regVal, const T_EIOR_FANUNIT_SV_INFO *pSvInfo )
{
	/*	���W�X�^�Ǐo���ʂ�fault�̏ꍇ	*/
	if((regVal & pSvInfo->regChkBit) != 0)
	{
		/*	FAN���j�b�g��Ԃ�normal�̏ꍇ	*/
		if((f_eiow_svctrl_mngp->fanSv.fanUnitFault & pSvInfo->stsBit) == 0)
		{
			/*	fault�񐔂��C���N�������g����	*/
			(*(pSvInfo->faultCnt))++;
			
			/*	fault�ی�i�񐔒���	*/
			if(*(pSvInfo->faultCnt) >= pSvInfo->faultProtect)
			{
				/*	fault����	*/
				f_com_SVCtl(pSvInfo->faultID, D_DU_SVCTL_ON, pSvInfo->faultFact);
				
				/*	FAN���j�b�g��Ԃ�fault�ݒ�	*/
				f_eiow_svctrl_mngp->fanSv.fanUnitFault |= pSvInfo->stsBit;
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "%s(0x%08x)", pSvInfo->fltLogMsg, regVal );
				
				/*	�g�yfault�𕜋�����	*/
				f_eio_fanFaultRecover();
				
				/*	fault�񐔂�����������	*/
				*(pSvInfo->faultCnt) = 0;
			}
		}
		/*	normal�񐔂�����������	*/
		*(pSvInfo->normalCnt) = 0;
	}
	/*	���W�X�^�Ǐo���ʂ�normal�̏ꍇ	*/
	else
	{
		/*	FAN���j�b�g��Ԃ�fault�̏ꍇ	*/
		if((f_eiow_svctrl_mngp->fanSv.fanUnitFault & pSvInfo->stsBit) != 0)
		{
			/*	normal�񐔂��C���N�������g����	*/
			(*(pSvInfo->normalCnt))++;
			
			/*	normal�ی�i�񐔒���	*/
			if(*(pSvInfo->normalCnt) >= pSvInfo->normalProtect)
			{
				/*	fault����	*/
				f_com_SVCtl(pSvInfo->faultID, D_DU_SVCTL_OFF, pSvInfo->faultFact);
				
				/*	FAN���j�b�g��Ԃ�fault����	*/
				f_eiow_svctrl_mngp->fanSv.fanUnitFault &= ~(pSvInfo->stsBit);
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "%s(0x%08x)", pSvInfo->norLogMsg, regVal );
				
				/*	normal�񐔂�����������	*/
				*(pSvInfo->normalCnt) = 0;
			}
		}
		/*	fault�񐔂�����������	*/
		*(pSvInfo->faultCnt) = 0;
	}
	return;
}


/*!
 *  @brief  This fuction excute FAN Reset.
 *  @note   This function excute FAN Reset.\n
 *  @param  fanIdx    [in]   fan locational Index
 *  @return VOID
 *  @retval none
 *  @date   2019/3/20 FJT)Koshida create
 */
VOID f_eio_fanReset(UINT fanIdx )
{
	/*	FAN Reset(ON)	*/
	BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTB_FANOCPRST, f_eior_fanSpeedInfo[fanIdx].resetBit);
	
	/* 2ms wait*/
	usleep(2000);
	
	/*	FAN Reset(OFF)	*/
	BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTB_FANOCPRST, f_eior_fanSpeedInfo[fanIdx].resetBit);
	
	/*	�O��IO���䗚�����O	*/
	f_eio_extIoSvCtrlLog("[TO]FAN#%d Reset : abnormal counter : nor=%d, flt=%d",
			 fanIdx, f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].abnormalCnt, f_eiow_fanMngTbl.fanSv.fanSpeed[fanIdx].faultCnt);
	
	return;
}


/*!
 *  @brief  This fuction will be called to write external IO ctrl log.
 *  @note   This function is processed as follows.\n
 *          - Write external IO ctrl log for EXT task
 *  @param  *Message_p [in]   log message 
 *  @param  ...        [in]   log message paramater
 *  @return VOID
 *  @retval none
 *  @date   2019/3/19 FJT)Koshida create
 */
VOID f_eio_extIoSvCtrlLog(const char* Message_p, ... )
{
	T_RRH_DU_SVCTRL_LOG_INFO	*logp;
	UINT						index;
    va_list args;
	
	/* Check message */
	if(Message_p == NULL)
	{
		return;
	}
	index = f_eiow_svctrl_logp->index;
	logp = &(f_eiow_svctrl_logp->logInfo[index]);
	
	/* Get System time			*/
	(VOID)BPF_HM_DEVC_GETTIME(&(logp->date));
	/* set message 			*/
	memset(logp->message, 0, sizeof(logp->message));
    va_start(args, Message_p);	/* pgr0039 */
	(VOID)vsnprintf((char*)logp->message, sizeof(logp->message), Message_p, args);
	va_end(args);
	
	/*	update index	*/
	f_eiow_svctrl_logp->index = (UINT)((f_eiow_svctrl_logp->index + 1) % D_DU_EIOLOG_MAX);
	
	return;
}

/* @} */

/*!
 *  @brief  �d�͕ϓ��Ď�����
 *  @note   �d�͕ϓ��Ď�����
 *  @return VOID
 *  @retval none
 *  @date   2020/2/28 FJT)Hasegawa create
 */
VOID f_eio_PowerMonitor()
{
	UINT	antno;
    UINT	regval;
	UINT	regaddr = 0xB11001C0;
	UINT    val = 0;
	UINT	powref_pre[D_RRH_ANT_MAX]={};
	static UINT	ng_cnt = 0, ok_cnt = 0;
	static UINT pre_val = 0;

	/* Carrier�d�̓��b�`�ݒ� */
	regval = 0x1;
	BPF_HM_DEVC_REG_WRITE(0, 0xB11001E4, &regval);
	
	for(antno = 0; antno < D_RRH_ANT_MAX; antno++) {
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, regaddr + antno*0x400, &regval);
		if(regval != 0) {
			if(f_eiow_antMngTbl[antno].powref == 0 ||
				((DL)f_eiow_antMngTbl[antno].powref/(DL)regval <= 0.5) || 
				((DL)f_eiow_antMngTbl[antno].powref/(DL)regval >= 2.0)) {
					val |= (0x1 << antno);
					f_eiow_antMngTbl[antno].change_cnt++;
			}
		}

		powref_pre[antno] = f_eiow_antMngTbl[antno].powref;
		f_eiow_antMngTbl[antno].powref = regval;
	}
	
	if(val != 0) {
		BPF_HM_DEVC_REG_WRITE(0, 0xB1202040, &val);
		if(val != pre_val){
			ng_cnt++;
			if(ng_cnt >= 100) {
				/* val�̕ω���100��A�������烍�O���c�� */ 
				BPF_COM_PLOG_TRACE_CAL( D_RRH_LOG_AST_LV_INFO , "Pow:0x%x A(c:%d):0x%x -> 0x%x,B(c:%d):0x%x -> 0x%x,C(c:%d):0x%x -> 0x%x,D(c:%d):0x%x -> 0x%x",
					val, f_eiow_antMngTbl[D_RRH_ANT_A].change_cnt, powref_pre[D_RRH_ANT_A], f_eiow_antMngTbl[D_RRH_ANT_A].powref,
						 f_eiow_antMngTbl[D_RRH_ANT_B].change_cnt, powref_pre[D_RRH_ANT_B], f_eiow_antMngTbl[D_RRH_ANT_B].powref,
						 f_eiow_antMngTbl[D_RRH_ANT_C].change_cnt, powref_pre[D_RRH_ANT_C], f_eiow_antMngTbl[D_RRH_ANT_C].powref,
						 f_eiow_antMngTbl[D_RRH_ANT_D].change_cnt, powref_pre[D_RRH_ANT_D], f_eiow_antMngTbl[D_RRH_ANT_D].powref);
				ng_cnt = 0;
			}
		}
		else {
			ng_cnt = 0;
		}
	}

	pre_val = val;
	ok_cnt++;
	if(ok_cnt >= 216000) {
		/* 6���Ԍo�߂��Ă����烍�O���c�� */
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "Pow:0x%x A(c:%d):0x%x -> 0x%x,B(c:%d):0x%x -> 0x%x,C(c:%d):0x%x -> 0x%x,D(c:%d):0x%x -> 0x%x",
			val, f_eiow_antMngTbl[D_RRH_ANT_A].change_cnt, powref_pre[D_RRH_ANT_A], f_eiow_antMngTbl[D_RRH_ANT_A].powref,
				 f_eiow_antMngTbl[D_RRH_ANT_B].change_cnt, powref_pre[D_RRH_ANT_B], f_eiow_antMngTbl[D_RRH_ANT_B].powref,
				 f_eiow_antMngTbl[D_RRH_ANT_C].change_cnt, powref_pre[D_RRH_ANT_C], f_eiow_antMngTbl[D_RRH_ANT_C].powref,
				 f_eiow_antMngTbl[D_RRH_ANT_D].change_cnt, powref_pre[D_RRH_ANT_D], f_eiow_antMngTbl[D_RRH_ANT_D].powref);
		ok_cnt = 0;
		for(antno = 0; antno < D_RRH_ANT_MAX; antno++) {
			f_eiow_antMngTbl[antno].change_cnt = 0;
		}
	}
	
	return;
}

/* @} */


