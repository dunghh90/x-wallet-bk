/*!
* @skip $Id:$
* @file f_frw_def.h
* @brief frw task Define Header
* @date  2015/08/07 TDIPS)sasaki update
* @date  2015/08/14 TDIPS)sasaki update Add Event-ID(D_FRW_EVTIDX_FWINFO_SAVE_REQ)
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
*/

/***************************************************************************************************************************//**
 *
 * @addtogroup RRH_PF_FRW
 * @{
 *
 ******************************************************************************************************************************/
#ifndef F_FRW_DEF_H
#define F_FRW_DEF_H

/*!
 * @name Data length 
 * @note Data length definition
 * @{
 */
#define D_FRW_SWVER_LEN             32
#define D_FRW_PRODUCTDATE_LEN       10
#define D_FRW_PRODUCTNUM_LEN        26
#define D_FRW_PRODUCTNAME_LEN       14
#define D_FRW_PRODUCTREV_LEN        8
#define D_FRW_SERIALNUM_LEN         14
#define D_FRW_CRC1SIZE              56
#define D_FRW_CHKSUM_OFFSET         0x44
/*! @} */

/*!
 * @name Event index
 * @note Event index definition used in f_fmngr_matrix_tbl
 * @{
 */
#define D_FRW_EVTIDX_LMCRUN_TOUT        0
#define D_FRW_EVTIDX_LOADUPD_REQ        1
#define D_FRW_EVTIDX_FLSHERASE_RSP      2
#define D_FRW_EVTIDX_FLSHWRT_RSP        3
#define D_FRW_EVTIDX_FLSHERASE_TOUT     4
#define D_FRW_EVTIDX_FLSHWRT_TOUT       5
#define D_FRW_EVTIDX_CHGBTSIDE_REQ      6
/*! @} */

/*!
 * @name MAXIMUM constant
 * @note MAXIMUM constant definition
 * @{
 */
#define D_FRW_STATMAX                  2                    /*!< Maximum task state number                                    */
#define D_FRW_EVTMAX                   6                    /*!< Maximum event number                                         */
#define D_FRW_MSGLENMAX                512                  /*!< Maximum length of msg received by FRW task                   */
/*! @} */

/*!
 * @name Event index
 * @note Event index definition used in f_frwr_matrix_tbl
 * @{
 */
#define D_FRW_EVTIDX_FLSHERASE_REQ			0
#define D_FRW_EVTIDX_FLSHWRITE_REQ			1
#define D_FRW_EVTIDX_FLSHREAD_REQ			2
#define D_FRW_EVTIDX_FLSHLOGWR_IND			3
#define D_FRW_EVTIDX_TRBLOGW_REQ			4
#define D_FRW_EVTIDX_FWINFO_SAVE_REQ		5
/*! @} */

/*!
 * @name EEPROM index 
 * @note EEPROM index  definition
 * @{
 */
#define D_FRW_THRDSTA_INIT          0                       /*!< Initial                                                      */
#define D_FRW_THRDSTA_RUN           1                       /*!< Runing                                                       */
/*! @} */

/*!
 * @name running history log
 * @{
 */
#define D_FRW_RUNHIS_BODYSIZE_MAX   48                      /*!< ??????????????????MAX                                              */
#define D_FRW_RUNHIS_FACENUM_MAX    1024                    /*!< ??????MAX                                                      */
#define D_FRW_RUNHIS_FLG_RCV        0x00                    /*!< ??????????????????(??????)                                           */
#define D_FRW_RUNHIS_FLG_SND        0x01                    /*!< ??????????????????(??????)                                           */
#define D_FRW_RUNHIS_FLG_SND_NG     0xff                    /*!< ??????????????????(??????)(NG)                                       */
/*! @} */

/*!
 * @name ?????????????????????????????????
 * @{
 */
#define D_FRW_COMPDATA              0x00010000              /*!< ?????????????????????                                               */
/*! @} */

/*!
 * @name PS???QSPI-FLASH FHM????????????????????????????????????
 * @{
 */
#define D_FRW_FWINFO_AREA_FACE      0x0000                  /*!< 0???                                                          */
#define D_FRW_FWINFO_AREA_OFFSET    0x00F00000              /*!< ????????????????????????????????????                                     */
/*! @} */

/*!
 * @brief	running history
 * @note	Contents:running history
 */
typedef struct
{
    UCHAR           setFlg;                                 /*!< ???????????????                                                   */
    UCHAR           month;                                  /*!< ???                                                           */
    UCHAR           day;                                    /*!< ???                                                           */
    UCHAR           hour;                                   /*!< ???                                                           */
    UCHAR           min;                                    /*!< ???                                                           */
    UCHAR           sec;                                    /*!< ???                                                           */
    UCHAR           msec;                                   /*!< ?????????                                                       */
    UCHAR           sndrcv_flg;                             /*!< ??????/???????????????                                              */
    UINT            event_id;                               /*!< ????????????ID                                                   */
    USHORT          task_id;                                /*!< ?????????ID                                                     */
    USHORT          sig_kind;                               /*!< ????????????                                                     */
}T_FRW_RUNHIS_HEAD;

/*!
 * @brief	running history information head
 * @note	Contents:running history information head	\n
 */
typedef struct
{
	T_FRW_RUNHIS_HEAD	head;                               /*!< ?????????                                                       */
	CHAR 				body[40];                           /*!< ?????????                                                       */
}T_FRW_RUNHIS_INF;

/*!
 * @brief	running history information table
 * @note	Contents:running history information table	\n
 */
typedef struct
{
    UINT                write_idx;                          /*!< ??????????????????                                                 */
    T_FRW_RUNHIS_INF    inf[D_FRW_RUNHIS_FACENUM_MAX];      /*!< ??????                                                         */
}T_FRW_RUNHIS_TBL;

#endif
/***************************************************************************************************************************//**
 *
 * @} addtogroup RRH_PF_FRW
 *
 ******************************************************************************************************************************/
