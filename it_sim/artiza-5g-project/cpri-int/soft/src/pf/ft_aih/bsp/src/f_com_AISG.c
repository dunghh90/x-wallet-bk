/*!
 * @skip  $ld:$
 * @file  f_hnd_AISG.c
 * @brief AISG sending and receiving data process funcation.
 * @date  2010/10/25 FFCS)zouw create\n
 * @date  2011/06/01 FFCS)Niemsh modify for M-RRU-ZSYS-00680
 * @date  2011/11/01 FFCS)Niemsh modify for M-RRU-ZSYS-00722
 * @date  2012/07/04 FFCS)Niemsh modify for M-RRU-ZSYS-00890
 * @date  2013/03/19  FFCS)Niemsh modify for ST800
 * @date  2019/09/10  FJT)Takahashi modify for 5GDU-M-002
 * @date  2019/09/19  FJT)Takahashi modify for 5GDU-M-002 IT2
 * 
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2010-
 */

/*!
 * @addtogroup RRH_HND
 * @{
 */
/* project header */
#include "f_rrh_inc.h"
#include "f_com_inc.h"
#include "f_com_aisg.h"
#include "f_aih_def.h"
#include "f_aih_ftyp.h"
#include "BPF_COM_PLOG.h"
#include <stdio.h>
#ifndef FOR_IT1
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#endif
#define __func__  __FUNCTION__

T_AISG_DRV_CTL f_bspw_aisgdrv_ctl ;

/****************************************************************************************************/
/*!
 * @brief AISG receiving process
 * @note  this function process as follow \n
 *	        -Clear interrupt factor
 *	        -Mask receiving interrupt
 *	        -Notify al2 task to perform further process
 * @param *pDrvCtrl  [in] AISG drive control pointer
 * @return void
 * @retval none  
 * @date 2010/10/25 FFCS)Zouw Create
 * @date 2013/03/19  FFCS)Niemsh modify for ST800
 *
 ****************************************************************************************************/
void f_bsp_AISGRxInt(T_AISG_DRV_CTL *pDrvCtrl  )
{
	/* RX mask */
	(void)BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_HDL_READ_WRITE, D_AISG_RADR_IRQ1MSK, D_AISG_RBIT_IRQ1MSK_RX );

	pDrvCtrl->f_bsp_rcvIntNtf( f_bsp_AISGRcv, pDrvCtrl);

	return;
}

/****************************************************************************************************/
/*!
 * @brief AISG receiving process
 * @note  this function process as follow \n
 *	        -Clear interrupt factor
 *	        -Mask receiving interrupt
 *	        -Notify al2 task to perform further process
 * @param *pDrvCtrl  [in] AISG drive control pointer
 * @return void
 * @retval none  
 * @date 2010/10/25 FFCS)Zouw Create
 * @date 2013/03/19  FFCS)Niemsh modify for ST800
 *
 ****************************************************************************************************/
void f_bsp_AISGRxInt_err(T_AISG_DRV_CTL *pDrvCtrl  )
{
	/* RX ERR mask */
	(void)BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_HDL_READ_WRITE, D_AISG_RADR_IRQ1MSK, D_AISG_RBIT_IRQ1MSK_ER );

	pDrvCtrl->f_bsp_rcvIntNtf( f_bsp_AISGRcv_err, pDrvCtrl);

	return;
}

/****************************************************************************************************/
/*!
 * @brief AISG Receiving interrupt process
 * @note  the function is processed as follow.\n
 *              sending AISG Receiving notification to AL2 task   
 *		
 * @param *pDrvCtrl [in] AISG device control pointer	
 * @return void
 * @retval none 
 * @date 2015/10/14  FFCS)Wuqy Create
 *
 ****************************************************************************************************/
void f_bsp_AISGRxingInt(T_AISG_DRV_CTL *pDrvCtrl)
{
	/* RX mask */
	(void)BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_HDL_READ_WRITE, D_AISG_RADR_IRQ1MSK, D_AISG_RBIT_IRQ1MSK_RXING );

	f_com_AISGRcvingIntNtf();
	return;
}

/****************************************************************************************************/
/*!
 * @brief AISG receiving process
 * @note  this function process as follow \n
 *          -Get receiving data
 *          -if data is not be destroyed, call callback function to send 
 *              data receiving notification to AL2, otherwise the destroyed 
 *              frame will be discard and send error code to AL2 task.
 *          - Unmask receiving interrupt
 * @param *pDrvCtrl  [in] AISG drive control pointer
 * @return void
 * @retval none  
 * @date 2010/10/25 FFCS)Zouw Create
 * @date 2013/03/19  FFCS)Niemsh modify for ST800
 * @date 2019/09/19  FJT)Takahashi for 5GDU-M-002
 *
 ****************************************************************************************************/

void f_bsp_AISGRcv_err( VOID *pPara )
{	
	UINT                  uiAisgarxerr;            	/* AISG receiving data destroyed information */
	USHORT			usErr[4];
	T_AISG_DRV_CTL        *pDrvCtrl;

	/*********************/
	/* parameter check*/
	/*********************/
	if(pPara == NULL)
	{
		return;
	}

	/*******************/
	/* Intialization*/
	/*******************/
	pDrvCtrl = (T_AISG_DRV_CTL *)pPara;
	uiAisgarxerr    = 0;
	memset((char*)usErr,0,sizeof(usErr));

	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,	D_AISG_RADR_RXASERR,&uiAisgarxerr);
	if((uiAisgarxerr & D_AISG_RBIT_ERRMSK_RX)  != 0 ){
		if((uiAisgarxerr & D_AISG_RBIT_ERRMSK_FCS ) != 0){
			usErr[0] = D_SYS_ON;
		}
		if((uiAisgarxerr & D_AISG_RBIT_ERRMSK_SHF ) != 0){
			usErr[1] = D_SYS_ON;
		}
		if((uiAisgarxerr & D_AISG_RBIT_ERRMSK_LHF ) != 0){
			usErr[2] = D_SYS_ON;
		}
		if((uiAisgarxerr & D_AISG_RBIT_ERRMSK_ABF ) != 0){
			usErr[3] = D_SYS_ON;
		}
	}else{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO,"RX ERROR: Nothing");
		usErr[3] = D_SYS_ON;
	}

	pDrvCtrl->f_bsp_rcvDatNtf( NULL,0, D_RRH_NG, usErr,0);

	(void)BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_HDL_READ_WRITE, D_AISG_RADR_IRQ1MSK, D_AISG_RBIT_IRQ1MSK_ER );

	return;
}


/****************************************************************************************************/
/*!
 * @brief AISG receiving process
 * @note  this function process as follow \n
 *          -Get receiving data
 *          -if data is not be destroyed, call callback function to send 
 *              data receiving notification to AL2, otherwise the destroyed 
 *              frame will be discard and send error code to AL2 task.
 *          - Unmask receiving interrupt
 * @param *pDrvCtrl  [in] AISG drive control pointer
 * @return void
 * @retval none  
 * @date 2010/10/25 FFCS)Zouw Create
 * @date 2013/03/19  FFCS)Niemsh modify for ST800
 * @date 2019/09/19  FJT)Takahashi for 5GDU-M-002
 *
 ****************************************************************************************************/

void f_bsp_AISGRcv( VOID *pPara )
{	
	UINT                  uiAisgrcvsts;             /* AISG receiving  status */
	UINT                  uiAisgrcvface;          	/* AISG receiving face information */
	UINT                  uiAisgrcvsize;           	/* AISG receiving data size */
	UINT				  uiAisgrcvsize_first;
	UINT				  uiAisgrcvsize_all;
	UINT                  aisgRcvAddr;            	/* AISG receiving data address */
	char                   chAisgBuffer[D_AISG_BSC_MEM_SIZE];  /* data buffer */
	char                   chAisgBuffer_all[D_AISG_BSC_MEM_SIZE];  /* data buffer */
	USHORT			usErr[4];
	T_AISG_DRV_CTL        *pDrvCtrl;
	UINT				  resultFlg;
	UINT				  count;

	/*********************/
	/* parameter check*/
	/*********************/
	if(pPara == NULL)
	{
		return;
	}

	/*******************/
	/* Intialization*/
	/*******************/
	pDrvCtrl = (T_AISG_DRV_CTL *)pPara;
	uiAisgrcvsts    = 0;
	uiAisgrcvface   = 0;
	aisgRcvAddr     = 0;
	uiAisgrcvsize   = 0;
	uiAisgrcvsize_all   = 0;
	uiAisgrcvsize_first = 0;
	memset(chAisgBuffer_all,0,sizeof(chAisgBuffer_all));
	resultFlg = D_RRH_NG_OTH;
	memset((char*)usErr,0,sizeof(usErr));

	for(count = 0 ; count < D_AISG_RCV_BDMAX ; count++){

		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_AISG_RADR_RXASPKC,&uiAisgrcvsts);
		if( D_RRH_OFF == ( uiAisgrcvsts & D_AISG_RBIT_RCVMSK_NTF)){
			break;
		}

		uiAisgrcvface = (uiAisgrcvsts >> 16)  & 0xf;
		if( uiAisgrcvface >= D_AISG_RCV_BDMAX ){
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,  "Rx BANK ERR(%d) error 0x%08x", uiAisgrcvface,uiAisgrcvsts);
			resultFlg = D_RRH_NG;
			usErr[3] = D_SYS_ON;
			break;			/* invalid face,  normal-range=0-11 */
		}

		aisgRcvAddr = (pDrvCtrl->m_RxBuffer + uiAisgrcvface * D_AISG_BSC_BNK_SIZE);
		uiAisgrcvsize = uiAisgrcvsts & 0x7ff;

		/* レングス長がmax以上または最小(FCS+addr+ctl)未満 */
		if((uiAisgrcvsize > D_AISG_BSC_MEM_SIZE) || (uiAisgrcvsize < ( D_AIH_MSG_MIN_SIZE + 2))){
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Rx length error %d %d error 0x%08x", uiAisgrcvsize_all , uiAisgrcvsize , uiAisgrcvsts );
			resultFlg = D_RRH_NG;
			usErr[3] = D_SYS_ON;
			break;
		}

		f_bsp_AISGCpyFromFpga(aisgRcvAddr, (UCHAR*)chAisgBuffer, D_AISG_BSC_MEM_SIZE, uiAisgrcvsize);

		uiAisgrcvsize -= 2;	/* FCS分を引く */
		if(( uiAisgrcvsize_all + uiAisgrcvsize ) <= D_AISG_BSC_MEM_SIZE ){
			memcpy( &chAisgBuffer_all[uiAisgrcvsize_all],chAisgBuffer,uiAisgrcvsize);
			if(uiAisgrcvsize_first == 0){
				uiAisgrcvsize_first = uiAisgrcvsize;	/* 初回のみ */
			}
			uiAisgrcvsize_all += uiAisgrcvsize;
		}else{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "Rx all length error %d %d error 0x%08x", uiAisgrcvsize_all , uiAisgrcvsize , uiAisgrcvsts );
			resultFlg = D_RRH_NG;
			usErr[3] = D_SYS_ON;
			break;
		}
		resultFlg = D_RRH_OK;
	}
	
	switch(resultFlg){
		case D_RRH_OK:
			pDrvCtrl->f_bsp_rcvDatNtf( chAisgBuffer_all,uiAisgrcvsize_first, D_RRH_OK,NULL,uiAisgrcvsize_all);
			break;
		case D_RRH_NG:
			pDrvCtrl->f_bsp_rcvDatNtf( NULL ,0, D_RRH_NG, usErr,0);
			break;
		case D_RRH_NG_OTH:
		default:
			/* do nothing */
			break;
	}
	
	/* RX unmask */
	(void)BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_HDL_READ_WRITE, D_AISG_RADR_IRQ1MSK, D_AISG_RBIT_IRQ1MSK_RX );
	return;
}

/****************************************************************************************************/
/*!
 * @brief AISG sending data complete interrupt process
 * @note  the function is processed as follow.\n
 *           - if sending complete,    
 *              then write interrupt fact, and call callback function to send 
 *              sending data complete notification to AL2 task   
 *		
 * @param *pDrvCtrl [in] AISG device control pointer	
 * @return void
 * @retval none 
 * @date 2010/10/25  FFCS)Zouw Create
 * @date 2013/03/19  FFCS)Niemsh modify for ST800
 *
 ****************************************************************************************************/
void f_bsp_AISGTxInt(T_AISG_DRV_CTL *pDrvCtrl)
{
	UINT uiSndSts;	    	/* sending data status 	*/ 

	/************************************************************************************************/
	/* initialization																				*/
	/************************************************************************************************/
	uiSndSts = 0;
	
	if(pDrvCtrl->m_nSndOnFlags != D_RRH_ON){
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, 
			"invalid AISG send IRQ %d", pDrvCtrl->m_nSndOnFlags);
	}
	/************************************************************************************************/
	/* read sending data status 																	*/
	/************************************************************************************************/
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,	D_AISG_RADR_SNDCTLBASE,&uiSndSts);
	uiSndSts &= D_ASIG_RBIT_SNDMSK_BSY;
	/***********************************************************************************************/
	/* sending data status judgement	                                                           */
	/* 0: Idle,   1:Busy                                                                           */
	/***********************************************************************************************/
	if( 0 == uiSndSts)
	{
		/***************************/
		/*Notify upper task(AL2) to process the msg by calling the callback function*/
		/***************************/
		pDrvCtrl->f_bsp_sndNtf();	
		
		pDrvCtrl->m_nSndOnFlags = D_RRH_OFF;
	}
	return;
}

/****************************************************************************************************/
/*!
 * @brief AISG sending data process
 * @note  the function is processed as follow. \n
 *	       	- parameter check
 *          - read sending ctrl register
 *              If sending status is busy, it is need to try for  several times
 *              otherwise, send data
 * @param 		*pDrvCtrl [in]  AISG device control pointer	
 *              *pPack    [in] AISG pack pointer
 * @return	UINT
 * @retval	0:D_BSP_OK
 *          1:D_BSP_NG
 * @date 2010/10/25  FFCS)Zouw Create
 * @date 2013/03/19  FFCS)Niemsh modify for ST800
 *
 ****************************************************************************************************/
UINT f_bsp_AISGSnd( T_AISG_DRV_CTL *pDrvCtrl,T_AISG_PACK *pPack )
{
	int    iRetry;        	/* retry times  				*/
	UINT 	uiSndSts;   	/* sending data status 			*/
	UINT 	uiDataLen; 		/* data length 					*/
	char*  pbufferadr; 		/* buffer address 				*/
	UINT   txBDBuffer;        
	UINT   uiTxBDInd; 		/* TxBD index 					*/
	UINT   uiIrqFct;
	UINT   uiRcvSts;
	UINT   uiBaudRate;
	UINT   uiEndian;


	/*****************************/
	/* initialization*/
	/****************************/
	iRetry = 0;
	uiSndSts = 0;
	uiTxBDInd = 0;
	
	uiDataLen = pPack->m_nLen;
	pbufferadr = pPack->m_dataAddr;
	/******************************/
	/* read sending ctrl register*/
	/******************************/
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_AISG_RADR_SNDCTLBASE,&uiSndSts);
	/*****************************************************/
	/*If sending status is busy, it is need to try several times*/
	/****************************************************/
	while((uiSndSts & D_ASIG_RBIT_SNDMSK_BSY) != 0)
	{
		if(iRetry++ > D_AISG_MAX_RETRY)
		{
			/****************************************/
			/*clear tx factor*/
			/***************************************/
			(void)BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_HDL_READ_WRITE, D_AISG_RADR_IRQ1MSK, D_AISG_RBIT_IRQ1MSK_TX );

			uiIrqFct = D_AISG_RBIT_IRQ1MSK_TX;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_AISG_RADR_IRQ1FCT,&uiIrqFct);

			(void)BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_HDL_READ_WRITE, D_AISG_RADR_IRQ1MSK, D_AISG_RBIT_IRQ1MSK_TX );

			/****************************************/
			/* reset FIFO & clear rx factor			*/
			/***************************************/
			uiRcvSts = D_AISG_RCV_RST;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_AISG_RADR_RCVCTLBASE,&uiRcvSts);

			uiRcvSts = D_AISG_RCV_START;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_AISG_RADR_RCVCTLBASE,&uiRcvSts);

			uiIrqFct = D_AISG_RBIT_IRQ1MSK_RX;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_AISG_RADR_IRQ1FCT,&uiIrqFct);

			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "AISG send busy 0x%08x", uiSndSts);

			return  D_RRH_NG;
		}

		/*20ms delay*/
		f_com_bspDelay(20);
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_AISG_RADR_SNDCTLBASE,&uiSndSts);

	}

	/* The maximum gap time between two consecutive octets shall not exceed the time t= 3*10/datarate.	*/
	/* データ送信と次データ送信の間には3msec以上おいてから送信する必要がある。念のため4msec待つことにする */
	f_com_bspDelay(4);

	/* dummy read */
	UINT   uiDumyRcvCnt;
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_AISG_RADR_RXASFNM,&uiDumyRcvCnt);
	BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ,D_AISG_RADR_RXASFDL,&uiDumyRcvCnt);

	/* ALD 受信FIFO reset (2 step必要) */
	uiRcvSts = D_AISG_RCV_RST;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_HDL_WRITE,D_AISG_RADR_RCVCTLBASE,&uiRcvSts);
	uiRcvSts = D_AISG_RCV_START;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_HDL_WRITE,D_AISG_RADR_RCVCTLBASE,&uiRcvSts);

	/*****************************************************/
	/*config baud rate, endian and timer*/
	/****************************************************/
	if(pDrvCtrl->m_baudRate != pPack->m_baudRate)
	{
		if( pPack->m_baudRate == D_SYS_AISG_BAUDRATE_115200)
		{
			uiBaudRate = 0x04;
		}
		else if( pPack->m_baudRate == D_SYS_AISG_BAUDRATE_38400)
		{
			uiBaudRate = 0x02;
		}
		else
		{
			uiBaudRate = 0x01;
		}
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_HDL_WRITE,D_AISG_CADR_BAUDRATE,&uiBaudRate);

		pDrvCtrl->m_baudRate = pPack->m_baudRate;
	}
	if(pDrvCtrl->m_endian != pPack->m_endian)
	{
		if(pPack->m_endian == D_SYS_AISG_BIG_ENDIAN )
		{
			uiEndian = D_SYS_AISG_BIG_ENDIAN;
		}
		else
		{
			uiEndian = D_SYS_AISG_LITTLE_ENDIAN;
		}
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_HDL_WRITE,D_AISG_CADR_ENDIAN,&uiEndian);
		pDrvCtrl->m_endian = pPack->m_endian;
	}
	if(pDrvCtrl->m_timer != pPack->m_timer)
	{
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_HDL_WRITE,D_AISG_CADR_TIMER,&(pPack->m_timer));
		pDrvCtrl->m_timer = pPack->m_timer;
	}

	/************************************/
	/* Copy the data into current TxBD */
	/************************************/
	uiTxBDInd = (USHORT)pDrvCtrl->m_nTxBDIndex;
	txBDBuffer = (pDrvCtrl->m_nLinkTxBDOffset[uiTxBDInd]);
	
	/*Copy the data into current TxBD  : pTxBuffer*/
	f_bsp_AISGCpyToFpga((UCHAR*)pbufferadr,txBDBuffer,D_AISG_BSC_SNDMEM_SIZE, uiDataLen);

	/***********************************************/
	/* Set AISG sending start face */
	/***********************************************/
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_HDL_WRITE,D_AISG_RADR_TXSIBNK,&uiTxBDInd);
	/***********************************************/
	/* Set AISG sending length*/
	/***********************************************/
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_HDL_WRITE,D_AISG_RADR_TXSILEN,&uiDataLen);
	
	/***********************************************/
	/* In create current TxBD offset*/
	/***********************************************/
	pDrvCtrl->m_nTxBDIndex++;
	if(pDrvCtrl->m_nTxBDIndex == pDrvCtrl->m_nTxBDNum)
	{
		pDrvCtrl->m_nTxBDIndex = 0;
	}
	
	/***********************************************/
	/* Set AISG sending start */
	/**********************************************/
	pDrvCtrl->m_nSndOnFlags = D_RRH_ON;

	uiSndSts = D_AISG_SNDSTART;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_HDL_WRITE,D_AISG_RADR_SNDCTLBASE,&uiSndSts);

	return D_RRH_OK;
}


/****************************************************************************************************/
/*!
 * @brief AISG interrupt function
 * @note  the function is processed as follow. \n
 *	        -if receiving case, receiving interrupt process
 *          -if sending case, sending interrupt process
 * @param usFact  [in] 	interrupt fact 	
 * @return void	
 * @retval none
 * @date 2010/10/25  FFCS)Zouw Create
 * @date 2013/03/19  FFCS)Niemsh modify for ST800
 *
 ****************************************************************************************************/
void f_bsp_AISGInt(UINT	uiFact)
{

	/*************************************/
	/* Rxing case             受信開始   */
	/*************************************/
	if((uiFact & D_AISG_RBIT_IRQ1FCT_RXING) != 0)
	{
		f_bsp_AISGRxingInt(&f_bspw_aisgdrv_ctl );
	}

	/*************************************/
	/* receive error case     受信ERROR  */
	/*************************************/
	if((uiFact & ( D_AISG_RBIT_IRQ1FCT_ER | D_AISG_RBIT_IRQ1FCT_TOUT)) != 0)
	{
		f_bsp_AISGRxInt_err(&f_bspw_aisgdrv_ctl );
	}

	/*************************************/
	/* receive case           受信完了   */
	/*************************************/
	if((uiFact & D_AISG_RBIT_IRQ1FCT_RX ) != 0)
	{
		f_bsp_AISGRxInt(&f_bspw_aisgdrv_ctl );
	}

	/*************************************/
	/* sending case           送信完了   */
	/*************************************/
	if((uiFact & D_AISG_RBIT_IRQ1FCT_TX) != 0)
	{
		f_bsp_AISGTxInt(&f_bspw_aisgdrv_ctl );
	}


	return;	
}

/****************************************************************************************************/
/*!
 * @brief  AISG start function
 * @note  the function is processed as follow. \n
 *	       - unmask rx interrupt bit, mask tx interrupt bit
 *         - set AISG receiving starting
 * @param  *pDrvCtrl  [in] 	AISG device control pointer
 *
 * @return UINT	
 * @retval  0: D_BSP_OK
 * @date 2010/10/25 FFCS)Zouw Create
 * @date 2013/03/19  FFCS)Niemsh modify for ST800
 *
 ****************************************************************************************************/
UINT f_bsp_AISGStart( T_AISG_DRV_CTL* pDrvCtrl  )
{
	UINT uiIrqFct;
	UINT uiRcvSts;

	/****************************************/
	/* unmask rx interrupt bit, mask tx interrupt bit	*/
	/***************************************/
	uiIrqFct = D_AISG_RBIT_IRQ1MSK_ER | D_AISG_RBIT_IRQ1MSK_TX | D_AISG_RBIT_IRQ1MSK_RX | D_AISG_RBIT_IRQ1MSK_RXING;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_AISG_RADR_IRQ1FCT,&uiIrqFct);

	(void)BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_AISG_RADR_IRQ1MSK,uiIrqFct);

	/****************************************/
	/* unmask rx error bit						*/
	/***************************************/
	(void)BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_AISG_RADR_RCVERR,D_AISG_RBIT_ERRMSK_RX);

	/****************************************/
	/* set AISG receiving stop 				*/
	/***************************************/
	uiRcvSts = D_AISG_RCV_RST;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_AISG_RADR_RCVCTLBASE,&uiRcvSts);

	return (D_RRH_OK);
}


/****************************************************************************************************/
/*!
 * @brief  AISG mapping function
 * @note  the function is processed as follow. \n
 *	       - mapping Tx,Rx register address into local memory address
 *
 * @return UINT	
 * @retval  0: D_BSP_OK
 * @date 2013/07/08  FFCS)Niemsh modify for B25
 *
 ****************************************************************************************************/
UINT f_bsp_AISGBufferMap()
{
	f_bspw_aisgdrv_ctl.m_TxBuffer = D_AISG_SND_BUFADR;
	f_bspw_aisgdrv_ctl.m_RxBuffer = D_AISG_RCV_BUFADR;

	return (D_RRH_OK);	
}

/****************************************************************************************************/
/*!
 * @brief AISG initialization function
 * @note  the function is processed as follow. \n
 *		     - set AISG installing information
 *              if AISG device is not installed, then return
 *           -Max sending and recieving BD number set 
 *           -divide txBD by maxlinks
 *           -current sending offset set
 *           -set receiving base address
 *           -set sending base address
 * @param  *pAisgDrvCtl [in] AISG device control pointer	
 *
 * @return int	
 * @retval 0: D_BSP_OK
 *         1: D_BSP_NG
 * @date 2010/10/11  FFCS)Zouw Create
 * @date 2013/03/19  FFCS)Niemsh modify for ST800
 *
 ****************************************************************************************************/
UINT f_bsp_AISGInit(T_AISG_DRV_CTL* pAisgDrvCtl)
{
	int iLoop;                 	/* cycle times 				*/
	/* UINT regVal; */
	UINT uiBaudRate;

	/*************************/
	/* parameter check*/
	/************************/
	if(pAisgDrvCtl == NULL)
	{
		return D_RRH_NG;
	}
	
	if(pAisgDrvCtl->m_nFlags == D_AISG_INSTALL)
	{
		return D_RRH_NG;
	}
	
	pAisgDrvCtl->m_nFlags = D_AISG_INSTALL;
	pAisgDrvCtl->m_nSndOnFlags = D_RRH_OFF;
	
	/************************/
	/* Max sending and recieving BD number set */
	/************************/
	pAisgDrvCtl->m_nRxBDNum = D_AISG_RCV_BDMAX;
	pAisgDrvCtl->m_nTxBDNum = D_AISG_SND_BDMAX;
	pAisgDrvCtl->m_nTxBDIndex = 0;

	if(D_RRH_OK != f_bsp_AISGBufferMap()){
		return D_RRH_NG;
	}
	/***********************/
	/* divide txBD */
	/**********************/
	for(iLoop = 0; iLoop < D_AISG_MAX_BUF_NUM ; iLoop++)
	{
		pAisgDrvCtl->m_nLinkTxBDOffset[iLoop] = (pAisgDrvCtl->m_TxBuffer + iLoop * D_AISG_BSC_BNK_SIZE);
	}

	pAisgDrvCtl->m_baudRate = D_SYS_AISG_BAUDRATE_9600;
	pAisgDrvCtl->m_endian = D_SYS_AISG_LITTLE_ENDIAN;
	pAisgDrvCtl->m_timer = 0;

	uiBaudRate = D_DU_AISG_AISGRATE_9600;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_AISG_CADR_BAUDRATE,&uiBaudRate);
	
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_AISG_CADR_ENDIAN,&(pAisgDrvCtl->m_endian));

	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE,D_AISG_CADR_TIMER,&(pAisgDrvCtl->m_timer));
	
	return D_RRH_OK;
}

/****************************************************************************************************/
/*! * @brief AISG memory copy in 4-byte way 
* @note  the function is processed as follow. \n 
*		    - parameter valid check 
*           - set copy length 
*           if the rest bytes is not to 4 bytes, it will be teated as 4 bytes 
*           -copy data 
* @param  *src  		[in] source character buffer address pointer 
*         *dest 		[in] destination character buffer address pointer 
*         destbuflen 	[in] destination buffer length 
*         len 			[in] data length 
* @return UINT	 
* @retval  		0: D_BSP_OK
*               1: D_BSP_NG
* @date 2010/10/11  FFCS)Zouw Create 
* ****************************************************************************************************/
void f_bsp_AISGCpyToFpga( UCHAR* src, UINT dest, UINT destbuflen, UINT len )
{	
	UINT				w_quot;									/* 商								*/
	UINT				w_loop;									/* ループカウンタ					*/
	UINT*				w_src_p;								/* 送信元ポインタ					*/
	UINT				w_data;
	
	/************************************************************************************************/
	/* 初期化																						*/
	/************************************************************************************************/
	if((src == NULL) || (dest == 0) || (len == 0))
	{
		return;
	}

	/*	コントロールが0xBFまたは0xAFの場合(XID)	*/
	if((src[1] & 0xEF) == 0xAF){
		BPF_COM_PLOG_AISG_SND_X(len,src);
	}else{
	/* XID以外 */
		BPF_COM_PLOG_AISG_SND(len,src);
	}

	f_com_dumpDataAisgSend(len,(CHAR*)src);

	w_src_p  = (UINT*)src;
	w_quot   = (len + 3) / sizeof(UINT);

	/************************************************************************************************/
	/* Endian 																*/
	/************************************************************************************************/
	for( w_loop=0; w_loop < w_quot ; w_loop++ )
	{
		w_data = f_com_swap32(*w_src_p);	
		*w_src_p = w_data;
		/* アドレス更新					*/
		w_src_p++;
	}

	BPF_HM_DEVC_REG_WRITE_BUFFER(
					D_RRH_LOG_REG_LV_HDL_WRITE,
					dest,				/**< レジスタ書き込みオフセット		*/
					destbuflen,			/**< レジスタ書き込みサイズ			*/
					(UINT*)src);		/**< 書き込みポインタ				*/
	return;
}

void f_bsp_AISGCpyFromFpga( UINT src, UCHAR* dest, UINT destbuflen, UINT len)
{
	UINT				w_quot;									/* 商								*/
	UINT				w_loop;									/* ループカウンタ					*/
	UINT*				w_dest_p;								/* 送信先ポインタ					*/
	UINT				w_data;
	
	/************************************************************************************************/
	/* 初期化																						*/
	/************************************************************************************************/
	if((src == 0) || (dest == NULL) || (len == 0))
	{
		return;
	}

	BPF_HM_DEVC_REG_READ_BUFFER(
				D_RRH_LOG_REG_LV_READ,
				src,					/**< レジスタ読み込みオフセット		*/
				destbuflen,				/**< レジスタ読み込みサイズ			*/
				(UINT*)dest);			/**< 読み込みポインタ				*/

	w_dest_p = (UINT*)dest;
	w_quot   = (len + 3) / sizeof(UINT);

	/************************************************************************************************/
	/* 送信先アドレスへコピー																		*/
	/************************************************************************************************/
	for( w_loop=0; w_loop < w_quot ; w_loop++ )
	{
		w_data = f_com_swap32(*w_dest_p);	
		*w_dest_p = w_data;
		w_dest_p++;
	}

	/*	コントロールが0xBFまたは0xAFの場合(XID)	*/
	if((dest[1] & 0xEF) == 0xAF){
		/* packet log send */
		BPF_COM_PLOG_AISG_RCV_X(len,dest);
	}else{
	/* XID以外 */
		BPF_COM_PLOG_AISG_RCV(len,dest);
	}

	f_com_dumpDataAisgRecv((len - 2),(CHAR*)dest);	/* FCSはカットする */

	return;
}

/**
 *  @brief  Forcely Message timing
 *  @note  A minimum of 3 ms shall elapse between receiving and transmitting messages
 *  @return VOID
 *  @date  2012/07/04 FFCS)niemsh M-RRU-ZSYS-00890
 */

VOID f_com_bspDelay (int ticks)
{
	f_com_taskDelay(ticks,0,0,0);

	return ;
}


/****************************************************************************************************/
/*!
 * @brief AISG Initialization function 
 * @note  the function is processed as follow. \n
 *		- init aisg device control table: f_bspw_aisgdrv_ctl
 *           - call f_bsp_AISGInit
 *              to init TxBd and RxBd
 *                       TxBd base address and RxBd base address
 * @param linkno [in] link number
 *              f_rcvIntNtf  [in] function pointer for L1 interrupt notification function
 *              f_rcvDatNtf  [in] function pointer for L1 receiving notification function
 *              f_sndNtf  [in] function pointer for sending data complete notification function
 * @return UINT	
 * @retval  0: D_BSP_OK
 *          1: D_BSP_NG
 * @date 2010/09/27 FFCS)Zouw Create
 *
 ****************************************************************************************************/

 UINT f_com_bspAISGInit(VOIDFUNCPTR f_rcvIntNtf,VOIDFUNCPTR f_rcvDatNtf, VOIDFUNCPTR f_sndNtf)
{
	UINT				w_rtn;							/*return value		*/

	w_rtn    = D_AISG_ERR_INIT;

	/* set f_bspw_aisgdrv_ctl */
	if(f_rcvIntNtf == NULL || f_rcvDatNtf == NULL || f_sndNtf == NULL){
		w_rtn = D_AISG_ERR_PARAM;
		return w_rtn;
	}

	if(D_AISG_INSTALL == f_bspw_aisgdrv_ctl.m_nFlags){
		return w_rtn;
	}

	/* set function pointer for L1 receiving interrupt notification function */
	f_bspw_aisgdrv_ctl.f_bsp_rcvIntNtf = f_rcvIntNtf;  
	/* set function pointer for L1 receiving data notification function */
	f_bspw_aisgdrv_ctl.f_bsp_rcvDatNtf = f_rcvDatNtf;  
	/* set function pointer for sending data complete notification function*/
	f_bspw_aisgdrv_ctl.f_bsp_sndNtf = f_sndNtf; 
	/* initialize the AISG install flag as uninstall */
	f_bspw_aisgdrv_ctl.m_nFlags = D_AISG_UNINSTALL;  
	
	w_rtn = f_bsp_AISGInit(&f_bspw_aisgdrv_ctl);

	return w_rtn;
}
	

/****************************************************************************************************/
/*!
 * @brief AISG start function
 * @note  -Check AISG device is already initialized
 *             if not, then return NG
 *            -call f_bsp_AISGStart to start AISG	
 * @param none	
 * @return UINT	
 * @retval  0: D_BSP_OK
 *          1: D_BSP_NG
 * @date 2010/09/27 FFCS)Zouw Create
 *
 ****************************************************************************************************/

UINT f_com_bspAISGStart ()
{	
	/* Check AISG device is already initialized */
	if(f_bspw_aisgdrv_ctl.m_nFlags != D_AISG_INSTALL )
	{
		return D_SYS_NG;
	}
	
	return f_bsp_AISGStart(&f_bspw_aisgdrv_ctl);
}


/****************************************************************************************************/
/*!
 * @brief AISG receive stop function
 * @note  - receive AISG	
 * @param N/A	
 * @return N/A	
 * @date 2020/01/06 Taniguchi
 *
 ****************************************************************************************************/
VOID f_com_bspAISGStop ( VOID )
{	
	UINT	uiRcvSts;

	uiRcvSts = D_AISG_RCV_RST;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_HDL_WRITE,D_AISG_RADR_RCVCTLBASE,&uiRcvSts);	
	return;
}

/****************************************************************************************************/
/*!
 * @brief AISG sending data function
 * @note    - Check AISG device is already initialized 
 *              - set AISG pack
 *		   -call f_bsp_AISGSnd to send data
 * @param  *pData [in] data buffer address pointer      
 *                nLength [in] data length
 * @return UINT	
 * @retval  0: D_BSP_OK
 *              1: D_BSP_NG
 * @date 2010/09/27 FFCS)Zouw Create
 *
 ****************************************************************************************************/
UINT f_com_bspAISGSnd(char *pData,UINT nLength,UINT baudrate,UINT endian,UINT timer)
{
	T_AISG_PACK tPack;

	/* Check AISG device is already initialized */
	if(f_bspw_aisgdrv_ctl.m_nFlags != D_AISG_INSTALL )
	{
		return D_RRH_NG;
	}

	if(pData == NULL ){
		return  D_RRH_NG;
	}

	if(nLength > D_AISG_SND_MAX_BYTE){
		return  D_RRH_NG;
	}

	/***********************************/
	/* set AISG pack*/
	/***********************************/
	memcpy(tPack.m_dataAddr,pData,nLength);
	tPack.m_nLen = nLength;           /* sending data length */
	tPack.m_baudRate = baudrate;
	tPack.m_endian = endian;
	tPack.m_timer = timer;

	return f_bsp_AISGSnd(&f_bspw_aisgdrv_ctl,&tPack);
}

/*!
 *  @brief  the process of swap 4 chars in data of 32bit
 *  @note   This function is processed as follows. \n
 *          - swap function;
 *  @return VOID
 *  @retval none
 *  @date   2013/05/10 FFCS)niemsh Create\n
 */
UINT f_com_swap32(UINT v32)
{
	return (v32 <<24) | ((v32 <<8) & 0x00ff0000) | ((v32 >>8) & 0x0000ff00) | ((v32 >>24) & 0x000000ff);
}

/** @} */

