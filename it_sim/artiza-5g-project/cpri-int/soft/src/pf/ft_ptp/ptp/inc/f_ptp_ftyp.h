/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	PTP Application プロトタイプ宣言
 *  @date	2018/4/16
 *
 *  COPYRIGHT FUJITSU LIMITED 2018
 */
/******************************************************************************************************************************/

#ifndef F_PTP_PTP_FTYP_H_
#define F_PTP_PTP_FTYP_H_

VOID f_ptp_main(VOID);
VOID f_ptp_debug_master(VOID);
VOID f_ptp_debug_slave(VOID);

extern VOID f_ptp_status_notif_ptpto( VOID* msgP );
extern VOID f_ptp_status_notif_synceto( VOID* msgP );
extern VOID f_ptp_status_notif_syncto( VOID* msgP );

#endif /* F_PTP_PTP_FTYP_H_ */
