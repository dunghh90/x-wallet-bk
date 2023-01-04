/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	PTP Application DEFINE
 *  @date	2018/4/16
 *
 *  COPYRIGHT FUJITSU LIMITED 2018
 */
/******************************************************************************************************************************/

#ifndef F_PTP_PTP_DEF_H_
#define F_PTP_PTP_DEF_H_

#define ALL_TRACE_LOG_LEVEL_MODID 100

#define PTP_DEBUG_DISP_LOG "/var/log/PtpDbgDisp.log"

#define D_PTP_PLLSB6_WRT1_MAX	196			/* PLL レジスタ 書込み回数 */
#define D_PTP_PLLSB6_WRT2_MAX	4			/* PLL レジスタ 書込み回数 */
#define D_PTP_PLLSB6_WRT3_MAX	3			/* PLL レジスタ 書込み回数 */
#define D_PTP_PLLSB6_WRT4_MAX	3			/* PLL レジスタ 書込み回数 */
#define D_PTP_PLLSB6_WRT5_MAX	3			/* PLL レジスタ 書込み回数 */
#define D_PTP_PLLSB6_WRT6_MAX	3			/* PLL レジスタ 書込み回数 */
#define D_PTP_PLLSB6_WRT7_MAX	8			/* PLL レジスタ 書込み回数 */

#define D_PTP_PLLMMW_WRT1_MAX	196			/* PLL レジスタ 書込み回数 */
#define D_PTP_PLLMMW_WRT2_MAX	4			/* PLL レジスタ 書込み回数 */
#define D_PTP_PLLMMW_WRT3_MAX	3			/* PLL レジスタ 書込み回数 */
#define D_PTP_PLLMMW_WRT4_MAX	3			/* PLL レジスタ 書込み回数 */
#define D_PTP_PLLMMW_WRT5_MAX	3			/* PLL レジスタ 書込み回数 */
#define D_PTP_PLLMMW_WRT6_MAX	3			/* PLL レジスタ 書込み回数 */
#define D_PTP_PLLMMW_WRT7_MAX	8			/* PLL レジスタ 書込み回数 */

#define D_PTP_ZERO				0			/* "0"定数					*/

#define D_PTP_SVCTL_SYNC_STAT_FREERUN		0x10000000			/* Sync Locked Status = Freerun			*/
#define D_PTP_SVCTL_SYNC_STAT_LOCKED		0x20000000			/* Sync Locked Status = Locked			*/
#define D_PTP_SVCTL_SYNC_STAT_HOLDOVER		0x40000000			/* Sync Locked Status = Hold Over		*/

#define	D_PTP_REQ_NTF						0x00000000

#define	D_PTP_REPORT_TIMER					1000				/* タイマ値(1msec → 1s)				*/
#define	D_PTP_SYNC_REPORT					1					/* Sync-状態保護タイマ値(1秒周期)		*/
#define	D_PTP_PTP_REPORT					1					/* PTP状態保護タイマ値(1秒周期)		*/
#define	D_PTP_SYNCE_REPORT					1					/* SyncE状態保護タイマ値(1秒周期)		*/
#define	D_PTP_PTP_REPORT_DEF				10					/* PTP状態保護タイマ値(10秒周期)		*/
#define	D_PTP_SYNCE_REPORT_DEF				10					/* SyncE状態保護タイマ値(10秒周期)		*/


#define	D_PTP_SHIT_BYTE1					 8
#define	D_PTP_SHIT_BYTE2					16
#define	D_PTP_SHIT_BYTE3					24

#define	D_PTP_PRIORITY127					127
#define	D_PTP_CLKCLASS127					127

/*****************   DEFINES   ************************************************/

#define SYNC_LOCKSTATE_MASK			0x70000000	/* Sync Locked Status(B30-B28) Mask	*/
#define PTP_LOCKSTATE_MASK			0x02000000	/* PTP Locked Status(B25) Mask	*/
#define SYNCE_LOCKSTATE_MASK		0x01000000	/* SyncE Locked Status(B24) Mask	 */
#define SYNCE_STATUS_MASK			0x000F0000	/* SyncE Status (B19-B16) Mask */

#define PARAMETER_ERROR				0x00000001
#define SHARED_MEM_ERROR			0x00000002
#define EDIT_FAILED					0x00000003
#define ERR_MSG_SIZE				32

#define PTP_MPLANE_ID_SIZE			20
#define	PTP_IDENTITY				"0x0000000000000000\0"
#define	PTP_EDIT_DATA_UNDEF_16		0xFFFF
#define	PTP_EDIT_DATA_UNDEF_32		0xFFFFFFFF
#define	PTP_EDIT_DATA_DEF_16		0x7FFF

#define SYNCE_ACCEPTANCE_SSM_NUM	11
#define SYNCE_SSM_TIMEOUT_FALSE		0
#define SYNCE_SSM_TIMEOUT_TRUE		1

#endif /* F_PTP_PTP_DEF_H_ */
