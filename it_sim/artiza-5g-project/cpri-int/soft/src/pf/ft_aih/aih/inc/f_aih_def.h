/********************************************************************************************************************/
/**
 * @skip  $ld:$
 * @file  f_aih_def.h
 * @brief AIH  constant definition
 * @date  2019/08/09 FJ)Takahashi Create
 * @date  2019/09/13 FJ)Takahashi modify IT2 issue
 * @date  2019/09/18 FJ)Takahashi modify CR
 *
 * All Rights Reserved, Copyright FUJITSU LIMITED 2019
 */
/********************************************************************************************************************/

/*!
 * @addtogroup RRH_PF_AIH
 * @{
 */

#ifndef F_AIH_DEF
#define F_AIH_DEF

#define D_AIH_NUM_0xFF 0xFF

/**********************************************************************
 * AIH thread Status スレッド状態                                     *
 **********************************************************************/
#define E_AIH_STAT_INI              0
#define E_AIH_STAT_IDLE             1
#define E_AIH_STAT_ALDSEND          2
#define E_AIH_STAT_ALDRCV           3
#define E_AIH_STAT_NUM              4

/**********************************************************************
 * local event definition 内部起動要因                                *
 **********************************************************************/
#define E_AIH_EVT_MSI_TRANSMIT_NTF  1
#define E_AIH_EVT_ALD_SEND_IRQ      2
#define E_AIH_EVT_ALD_RCV_IRQ       3
#define E_AIH_EVT_ALD_ERR_IRQ       4
#define E_AIH_EVT_ALD_RCVING_IRQ    5
#define E_AIH_EVT_ALD_RCV_TIMEOUT   6
#define E_AIH_EVT_MSI_PWRCTRL_NTF   7
#define E_AIH_EVT_MSI_PWRGET_NTF    8
#define E_AIH_EVT_NUM               9

/**********************************************************************
 * AIH Timer value                                                    *
 **********************************************************************/
#define D_AIH_ALDTOUT_TIMVAL		400   /* ALD受信待ち最小時間(400ms) */

/**********************************************************************
 * Related running history                                            *
 **********************************************************************/
#define E_AIH_FLG_RCVON             1     /* ALD受信中falg ON */
#define E_AIH_FLG_RCVOFF            0     /* ALD受信中flag OFF */

/**********************************************************************
 * Address & Control fields                                           *
 **********************************************************************/
#define E_AIH_CTRL_ALDON            0x00000010   /* ALD電源ON bit  */
#define E_AIH_CTRL_ALDOFF           0x00000000   /* ALD電源OFF bit */
#define E_AIH_CTRL_FIFORST_0        0x00000000   /* 受信FIFO reset手順1 */
#define E_AIH_CTRL_FIFORST_1        0x00000001   /* 受信FIFO reset手順2 */

/**********************************************************************
 * interface field                                                    *
 **********************************************************************/
#define E_AIH_PORT_ID               0    /* msi if値 ALD port id  */
#define E_AIH_IF_ALDON              1    /* msi if値 ALD電源ON    */
#define E_AIH_IF_ALDOFF             0    /* msi if値 ALD電源OFF   */
#define E_AIH_IF_CRCOK              0    /* msi if値 CRC-OK       */
#define E_AIH_IF_CRCNG              1    /* msi if値 CRC-NG       */
#define E_AIH_IF_ALDSIZEOK          0    /* msi if値 ALD SIZE OK  */
#define E_AIH_IF_ALDSIZENG          1    /* msi if値 ALD SIZE NG  */
#define E_AIH_IF_NOTSENDING         0    /* msi if値 ALD送信IDLE  */
#define E_AIH_IF_SENDING            1    /* msi if値 ALD送信中    */

/**********************************************************************
 * length                                                             *
 **********************************************************************/
#define D_AIH_REJ_MSG_SIZE         32    /* REJ msg length */
#define D_AIH_ALDBLOG_MSG_SIZE     1200  /* msg log areaのsize */
#define D_AIH_MSG_MIN_SIZE         2     /* msgの最小サイズ */
#define D_RRH_LOG_ALD_LOG_NUM      500   /* ALD LOGの配列数 */

/**********************************************************************
 * Statics operation type                                                        *
 **********************************************************************/
#define D_AL2_STATICS_INC 0		/* increase*/
#define D_AL2_STATICS_SET 1 		/* set*/
#define D_AL2_STATICS_CLR 2		/* clear*/


/**********************************************************************
 * AISG bit order swap												  *
 **********************************************************************/
#define D_AL2_SWAP16(x) ((((x) << 8) & 0xff00) | \
						 (((x) >> 8) & 0x00ff))
						 
#define D_AL2_SWAP32(x) ((((x) & 0x000000ff) << 24) | \
						 (((x) & 0x0000ff00) <<  8) | \
						 (((x) & 0x00ff0000) >>  8) | \
						 (((x) & 0xff000000) >> 24))




/*********************************
src\sys\inc_arch\f_du_reg_aisg.h   ALD電源制御レジスタアドレス  *************
********************************/
#define D_DU_REG_AISG_ALDPWR	D_DU_REG_CTB_PWR	/* ALD電源制御REG addr	*/
#define D_DU_REG_AISG_FIFORST	D_DU_REG_AISG_RXAISG/* AISG Rx FIFO RESET addr */
#define E_AIH_CTRL_CRCERR		0x00000008			/* 受信CRC ERR bit		*/
#define E_AIH_CTRL_LENERR		0x00000007			/* 受信LENGTH ERR bit	*/

/*MSI ID  ALDデータ転送通知ID  *************************************/
#define MSI_O_RAN_ALD_ALD_COMMUNICATION_REQ		0x00010101	/**< 受信通知 */
#define MSI_O_RAN_ALD_ALD_COMMUNICATION_CFM		0x00020202	/**< 送信通知 */
#define MSI_O_RAN_ALD_ALD_COMMUNICATION_REJ		0x00030303	/**< MSI送信通知 */
/*        ALD電源制御ID  *******************************************/
#define MSI_O_RAN_ALD_PORT_EDIT_REQ				0x04010101	/**< MSI受信通知 */
#define MSI_O_RAN_ALD_PORT_EDIT_REJ				0x04020202	/**< MSI送信通知 */
#define MSI_O_RAN_ALD_PORT_EDIT_CFM				0x04030303	/**< MSI送信通知 */


#endif

/* @} */


