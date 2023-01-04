/****************************************************************************************************/
/*!
 * @skip  $ld:$
 * @file  f_bsp_AISG.h
 *
 * @brief  AISG Defination Header
 *
 * @date  2010/10/03 FFCS) Zouw
 * @date  2019/09/10 FJT)Takahashi modify for 5GDU-M-002
 * @date  2019/09/19 FJT)Takahashi modify for 5GDU-M-002
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2010-
 * 
 ****************************************************************************************************/

/*!
 * @addtogroup RRH_BSP_AISG
 * @{
 */
#ifndef F_BSP_AISG_H
#define F_BSP_AISG_H
/*#include "f_mpsw_msg_o-ran_ald_def.h"
#include "f_mpsw_msg_o-ran_ald_port_def.h"
#include "f_mpsw_msg_o-ran_ald_typ.h"
#include "f_mpsw_msg_o-ran_ald_port_typ.h"*/
#include "f_du_reg_aisg.h"
#include "f_du_reg_ctb.h"

/****************************************************************************************************/
/*!
 * @name include
 * @{
 ****************************************************************************************************/

/* @} */
/****************************************************************************************************/
/* AISG defination */
/****************************************************************************************************/
/*!
 * @name AISG const defination
 * @note  
 * @{
 */
  
#define D_AISG_RCV_START				0x00000001						/*!<AISG receiving start  	*/
#define D_AISG_RCV_RST					0x00000000						/*!<AISG receiving start  	*/
#define D_AISG_MAX_BUF_NUM				1								/*!<max tx buffer number 	*/
#define D_AISG_MAX_RETRY				100								/*!< sending retry times  	*/
#define D_AISG_SNDSTART					0x00000001						/*!<sending data start 		*/
#define D_AISG_SND_MAX_BYTE				1200							/*!< Max sending data bytes */

#define D_AISG_INSTALL					1								/*!<ASIG install 			*/
#define D_AISG_UNINSTALL				0								/*!<AISG uninstall 			*/
/* @} */

/*!
 * @name AISG register address defination
 * @note  
 * @{
 */
#define D_AISG_CADR_BAUDRATE				D_DU_REG_AISG_AISGRATE
#define D_AISG_CADR_ENDIAN					D_DU_REG_AISG_AISGFCS
#define D_AISG_CADR_TIMER					D_DU_REG_AISG_AISGTIM

#define D_AISG_RADR_RCVCTLBASE				D_DU_REG_AISG_RXAISG		/*!<AISG receiving control address   	*/
#define D_AISG_RADR_RXASPK					D_DU_REG_AISG_RXASPK
#define D_AISG_RADR_RXASPKC					D_DU_REG_AISG_RXASPKC
#define D_AISG_RADR_RXASERR					D_DU_REG_AISG_RXASERR
#define D_AISG_RADR_RCVERR					D_DU_REG_AISG_RXASERRM		/*!<AISG receving error  address 	    */
#define D_AISG_RADR_RXASFNM					D_DU_REG_AISG_RXASFNM
#define D_AISG_RADR_RXASFDL					D_DU_REG_AISG_RXASFDL

#define D_AISG_RADR_IRQ1FCT					D_DU_REG_CTB_IRQAISG		/*!< IRQ1 factor register address		*/
#define D_AISG_RADR_IRQ1MSK					D_DU_REG_CTB_IRQAISGM		/*!< IRQ1 mask register address			*/

#define D_AISG_RADR_SNDCTLBASE				D_DU_REG_AISG_TXAISG		/*!<AISG sending control base address 	*/
#define D_AISG_RADR_TXSIBNK					D_DU_REG_AISG_TXSIBNK
#define D_AISG_RADR_TXSILEN					D_DU_REG_AISG_TXSILEN

#define D_AISG_SND_BDMAX					1                         	/*!<AISG max sending buffer number		*/
#define D_AISG_RCV_BDMAX					12                        	/*!<AISG max receiving buffer 	number	*/
#define D_AISG_BSC_MEM_SIZE  				1202                       	/*!<AISG max rcv buffer size			*/
#define D_AISG_BSC_SNDMEM_SIZE 				1200                       	/*!<AISG max snd buffer size			*/
#define D_AISG_BSC_BNK_SIZE  				2048                       	/*!<AISG  bank size						*/
/* @} */


/*!
 * @name AISG register bit  value defination
 * @note  
 * @{
 */
#define D_AISG_RBIT_IRQ1FCT_TOUT	0x00000008							/*!<AISG IRQ1 time out	 				*/
#define D_AISG_RBIT_IRQ1FCT_ER		0x00000004							/*!<AISG IRQ1 error check  				*/
#define D_AISG_RBIT_IRQ1FCT_TX		0x00000002							/*!<AISG IRQ1 sending interrupt factor 	*/
#define D_AISG_RBIT_IRQ1FCT_RX		0x00000001							/*!<AISG IRQ1 received interrupt factor*/
#define D_AISG_RBIT_IRQ1FCT_RXING	0x00000010							/*!<AISG IRQ1 receiving interrupt factor*/

#define D_AISG_RBIT_IRQ1MSK_ER		0x00000004							/*!<AISG IRQ1 error check  				*/
#define D_AISG_RBIT_IRQ1MSK_TX		0x00000002							/*!<AISG IRQ1 sending interrupt factor 	*/
#define D_AISG_RBIT_IRQ1MSK_RX		0x00000001							/*!<AISG IRQ1 received interrupt factor*/
#define D_AISG_RBIT_IRQ1MSK_RXING	0x00000010							/*!<AISG IRQ1 receiving interrupt factor*/

#define D_AISG_RBIT_RCVMSK_NTF		D_DU_AISG_RXASPKC_NEMP
#define D_ASIG_RBIT_SNDMSK_BSY		D_DU_AISG_TXAISG_BUSY
#define D_AISG_RBIT_ERRMSK_RX		D_DU_AISG_RXASERR_MSK				/*!<AISG receiving error mask 			*/
#define D_AISG_RBIT_ERRMSK_FCS		D_DU_AISG_RXASERR_FCS				/*!<AISG receiving error mask 			*/
#define D_AISG_RBIT_ERRMSK_SHF		D_DU_AISG_RXASERR_SHF				/*!<AISG receiving error mask 			*/
#define D_AISG_RBIT_ERRMSK_LHF		D_DU_AISG_RXASERR_LOF				/*!<AISG receiving error mask 			*/
#define D_AISG_RBIT_ERRMSK_ABF		D_DU_AISG_RXASERR_ABF				/*!<AISG receiving error mask 			*/
/* @} */

/*!
 * @name AISG buffer address defination
 * @note  
 * @{
 */
#define D_AISG_SND_BUFADR			D_DU_REG_AISG_SNDBUF				/*!<AISG sending buffer base address		*/
#define D_AISG_RCV_BUFADR			D_DU_REG_AISG_RCVBUF 				/*!<AISG receiving buffer base address  	*/
/* @} */

/*!
 * @name AISG debug flag defination
 * @note  
 * @{
 */
#define D_AISG_DEBUG_RECV			0x0001
#define D_AISG_DEBUG_SEND			0x0002
#define D_AISG_DEBUG_INTR			0x0004
#define D_AISG_DEBUG_INIT			0x0008
#define D_AISG_DEBUG_FUNC			0x0010
/* @} */

/*!
 * @name AISG error code defination
 * @note  
 * @{
 */																					
#define D_AISG_ERR_INIT				1						/*!< AISG init abnormal			*/
#define D_AISG_ERR_PARAM     		2						/*!< AISG param error			*/
 /* @} */
/*****************************************************************************************************/
/* AISG table structure defination*/
/*****************************************************************************************************/

/****************************************************************************************************/
/** 
 * @brief	 AISG receiving control table
 * @note	 AISG receiving control table is ..
 */
/****************************************************************************************************/
typedef struct
{
	UINT m_aisgrcvclt;			/* AISG receiving control          4000_3800*/
	UINT m_dummy;
	UINT m_aisgrcvstat;			/* AISG receiving notification     4000_3808*/
	UINT m_aisgrcverr;			/* AISG receiving error information  4000_380c */
	UINT m_aisgrcverr_m;			/* AISG receiving error mask  4000_3810 */
}T_AISG_RCVCTL;

/* @} */


/****************************************************************************************************/
/** 
 * @brief	 AISG Sending control table
 * @note	 AISG Sending control table is ..
 */
/****************************************************************************************************/
typedef struct
{
	UINT m_aisgsndstat;			/* AISG sending status 4000_3900*/
	UINT m_aisgsndface;			/* AISG sending start  face  4000_3904*/   
	UINT m_aisgsndsize;			/* AISG sending datasize 4000_3908*/
}T_AISG_SNDCTL;
/* @} */


/****************************************************************************************************/
/** 
 * @brief	 AISG driver table
 * @note	 AISG driver table is ..
 */
/****************************************************************************************************/
typedef struct
{
	USHORT m_nFlags;								/* AISG installment information */
	USHORT m_nSndOnFlags;
	UINT m_nRxBDNum;								/* RxBD number     				*/
	UINT m_nTxBDNum;								/* TxBD number    				*/
	UINT m_nTxBDIndex;								/* Index to TxBD:point to the next TxBD offset  */
	UINT m_RxBuffer;								/* RxBD base address   			*/
	UINT m_TxBuffer;								/* TxBD base address   			*/
	UINT m_nLinkTxBDOffset[D_AISG_MAX_BUF_NUM];   	/* TxBD offset         			*/
	VOIDFUNCPTR f_bsp_rcvIntNtf;					/* function pointer to process interrupt notification 		*/
	VOIDFUNCPTR f_bsp_rcvDatNtf;					/* function pointer to receiving data notification 			*/
	VOIDFUNCPTR f_bsp_sndNtf;						/* function pointer to sending data complete notificaiton 	*/

	UINT m_baudRate;
	UINT m_endian;
	UINT m_timer;
}T_AISG_DRV_CTL;

/* @} */


/****************************************************************************************************/
/** 
 * @brief	 AISG sending data packet
 * @note	 AISG sending data packet is ..
 */
/****************************************************************************************************/
typedef struct
{
	char m_dataAddr[D_AISG_SND_MAX_BYTE];     /* sending data buffer address */
	UINT m_nLen;          /* sending data length */
	UINT m_baudRate;	     /* sending baudrate */
	UINT m_endian;	     /* sending endian */
	UINT m_timer;	     /* sending timer */
}T_AISG_PACK;
/* @} */

/****************************************************************************************************/
/* AISG table declaration*/
/***************************************************************************************************/

/****************************************************************************************************/
/*!
 * @name AISG drvier control table
 * @note
 * @{
 ****************************************************************************************************/
extern	T_AISG_DRV_CTL	f_bspw_aisgdrv_ctl;		/* aisg driver control 	*/

/* @} */

/****************************************************************************************************/
/* AISG function declaration */
/****************************************************************************************************/

/****************************************************************************************************/
/*!
 * @name AISG functions
 * @note
 * @{
 ****************************************************************************************************/

typedef struct
{
    T_RRH_SYSTEM_TIME  logTime;
	int length;
	char frame[128];
	INT rcvStat;
	INT rcvDbg;
	INT rcvErr;
	INT rcvCnt;
	INT rcvCnt2;
	INT sndStat;
	INT sndCnt;
	INT sndCnt2;
}T_AISG_LOG;

typedef struct
{
	INT index;
	T_AISG_LOG logRec[128];	
}T_AISG_LOG_TBL;

extern void f_bsp_AISGRxInt(T_AISG_DRV_CTL *pDrvCtrl);
extern void f_bsp_AISGRxInt_err(T_AISG_DRV_CTL *pDrvCtrl);

extern void f_bsp_AISGTxInt(T_AISG_DRV_CTL *pDrvCtrl);

extern void f_bsp_AISGRcv(VOID *pDrvCtrl  );
extern void f_bsp_AISGRcv_err(VOID *pDrvCtrl  );

extern UINT f_bsp_AISGSnd( T_AISG_DRV_CTL *pDrvCtrl,T_AISG_PACK *pPack );

extern void f_bsp_AISGInt(UINT	uiFact);

extern UINT f_bsp_AISGStart( T_AISG_DRV_CTL* pDrvCtrl  );

extern UINT f_bsp_AISGInit(T_AISG_DRV_CTL* pAisgDrvCtl);

extern void f_bsp_AISGCpyToFpga( UCHAR* src, UINT dest, UINT destbuflen, UINT len );

extern void f_bsp_AISGCpyFromFpga( UINT src, UCHAR* dest, UINT destbuflen, UINT len );

extern void f_com_bspDelay(int);

extern  UINT f_com_bspAISGInit(VOIDFUNCPTR f_rcvIntNtf,VOIDFUNCPTR f_rcvDatNtf, VOIDFUNCPTR f_sndNtf);

extern VOID f_com_AISGRcvDatNtf(char *,UINT , UINT ,USHORT *, UINT );

extern VOID f_com_AISGSndNtf();

extern VOID f_com_AISGRcvIntNtf(VOIDFUNCPTR f_rcvFunc,VOID *iParam);

extern UINT f_com_bspAISGStart ();
extern VOID f_com_bspAISGStop(VOID);

extern UINT f_com_bspAISGSnd(char *pData,UINT nLength,UINT baudrate,UINT endian,UINT timer);

extern void f_com_AISGRcvingIntNtf();

extern void f_bsp_AISGRxingInt(T_AISG_DRV_CTL *pDrvCtrl);

/* @} */

#endif /* F_BSP_AISG_H */

/* @} */

