/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_eio_def.h
 *  @brief  5GDU-LLS external IO supervision & cntrol defination
 *  @date   2019/03/19 FJT)Koshida create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019-
 */
/*********************************************************************************/

/* 二重取込み防止 */
#ifndef		F_EIO_DEF
#define		F_EIO_DEF

/** @addtogroup RRH_PF_EIO
* @{ */

/* ======================================================================== */
/* 状態定義																	*/
/* ======================================================================== */
enum
{
	E_EIO_STAT_INI = 0,									/* init						*/
	E_EIO_STAT_RUN,										/* running					*/
	E_EIO_STAT_MAX,										/* Status Number			*/
};

/* ======================================================================== */
/* イベント定義																*/
/* ======================================================================== */
enum
{
	E_EIO_EVT_INIT = 0,									/* Thread Create								*/
	E_EIO_EVT_EXTIOSV_TIMEOUT,							/* External IO SV timer timeout indication		*/
	E_EIO_EVT_FANSVCTRL_TIMEOUT,						/* FAN SV & Cntrol timer timeout indication		*/
	E_EIO_EVT_API_FANDBG_IND,							/* FAN debug mode indication					*/
	E_EIO_EVT_API_FANDAC_IND,							/* FAN DAC chage indication						*/
	E_EIO_EVT_MSI_MSGRCV_NTF,							/* MSI message receive notify					*/
	E_EIO_EVT_MAX,										/* Event Number									*/
};

/*	タイマ値	*/
#define D_EIO_EXTOUTSV_TIMVAL			100				/* 外部IO入力状態監視時間(100ms)				*/
#define D_EIO_FANSVCTRL_TIMVAL			100				/* FAN監視制御時間(100ms)						*/


/* ======================================================================== */
/* 外部IO監視制御定義														*/
/* ======================================================================== */
#define D_EIO_EXTIO_OPEN				0				/*	open(OFF)									*/
#define D_EIO_EXTIO_SHORT				1				/*	short(ON)									*/
#define D_EIO_EXTIO_INVALID				0xffffffff		/*	invalid(無効)								*/
#define D_EIO_EXTIO_PORT0				0				/*	Port番号:0									*/

/* ======================================================================== */
/* FAN監視制御定義															*/
/* ======================================================================== */

#define D_EIO_TONUM_1S					10				/* 1秒間のタイムアウト回数						*/

#define D_EIO_MAXSTATIM_MIN				0				/* FAN MAX継続最小回数							*/
#define D_EIO_MAXSTATIM_MAX				0				/* FAN MAX継続最大回数							*//*	ハソv1.05変更(3->0)	*/

/*	FAN 監視関連	*/
#define D_EIO_PROTECTCNT_FANMOUNT		5				/* FAN実装Normal保護段数						*/
#define D_EIO_PROTECTCNT_FANUNMOUNT		5				/* FAN未実装Fault保護段数						*/

#define D_EIO_PROTECTCNT_FANPSFLT		3				/* FAN電源Fault保護段数							*/
#define D_EIO_PROTECTCNT_FANPSNOR		3				/* FAN電源Normal保護段数						*/

#define D_EIO_PROTECTCNT_FANSPEEDFLT	10				/* FAN回転数Fault保護段数						*/
#define D_EIO_PROTECTCNT_FANSPEEDNOR	10				/* FAN回転数Normal保護段数						*/
#define D_EIO_PROTECTCNT_FANRESET_FLT	60				/* FAN回転数FaultReset保護段数					*/
#define D_EIO_PROTECTCNT_FANRESET_NOR	5				/* FAN回転数FaultReset保護段数					*/

#define D_EIO_THRESHOLD_FANSPEED		D_DU_THRESHOLD_FANSPEED	/* FAN回転数Fault閾値	*/

#define D_EIO_FANSPEED_MASK_TEMP_SUB6	650				/* FAN回転数低下マスク用センサ温度閾値(sub6)	*/
#define D_EIO_FANSPEED_MASK_TEMP_MMW	600				/* FAN回転数低下マスク用センサ温度閾値(mmw)		*/

/*	FAN 制御関連	*/
#define D_EIO_SPINMODE_LOW_SPEED		0xFFF			/* FAN回転数モード：低回転モード				*/
#define D_EIO_SPINMODE_NML_SPEED		0x000			/* FAN回転数モード：通常回転モード				*/

/*	FAN Unit監視項目	*/
enum
{
	E_EIO_FANITEM_MOUNT = 0,
	E_EIO_FANITEM_SPSTS,
	E_EIO_FANITEM_MAXNO
};

/*	FAN GET関連	*/
#define D_EIO_FANSPEED_MAX				D_DU_FANSPEED_MAX		/* FAN MAX回転数(3810[rpm])(TBD)				*/
#define D_EIO_FANSPEED_CALRPM			D_DU_FANSPEED_CALRPM	/* FAN回転数rpm変換								*/

/* @} */

#endif	/* F_EIO_DEF */

