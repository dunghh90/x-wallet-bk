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
 * AIH thread Status �X���b�h���                                     *
 **********************************************************************/
#define E_AIH_STAT_INI              0
#define E_AIH_STAT_IDLE             1
#define E_AIH_STAT_ALDSEND          2
#define E_AIH_STAT_ALDRCV           3
#define E_AIH_STAT_NUM              4

/**********************************************************************
 * local event definition �����N���v��                                *
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
#define D_AIH_ALDTOUT_TIMVAL		400   /* ALD��M�҂��ŏ�����(400ms) */

/**********************************************************************
 * Related running history                                            *
 **********************************************************************/
#define E_AIH_FLG_RCVON             1     /* ALD��M��falg ON */
#define E_AIH_FLG_RCVOFF            0     /* ALD��M��flag OFF */

/**********************************************************************
 * Address & Control fields                                           *
 **********************************************************************/
#define E_AIH_CTRL_ALDON            0x00000010   /* ALD�d��ON bit  */
#define E_AIH_CTRL_ALDOFF           0x00000000   /* ALD�d��OFF bit */
#define E_AIH_CTRL_FIFORST_0        0x00000000   /* ��MFIFO reset�菇1 */
#define E_AIH_CTRL_FIFORST_1        0x00000001   /* ��MFIFO reset�菇2 */

/**********************************************************************
 * interface field                                                    *
 **********************************************************************/
#define E_AIH_PORT_ID               0    /* msi if�l ALD port id  */
#define E_AIH_IF_ALDON              1    /* msi if�l ALD�d��ON    */
#define E_AIH_IF_ALDOFF             0    /* msi if�l ALD�d��OFF   */
#define E_AIH_IF_CRCOK              0    /* msi if�l CRC-OK       */
#define E_AIH_IF_CRCNG              1    /* msi if�l CRC-NG       */
#define E_AIH_IF_ALDSIZEOK          0    /* msi if�l ALD SIZE OK  */
#define E_AIH_IF_ALDSIZENG          1    /* msi if�l ALD SIZE NG  */
#define E_AIH_IF_NOTSENDING         0    /* msi if�l ALD���MIDLE  */
#define E_AIH_IF_SENDING            1    /* msi if�l ALD���M��    */

/**********************************************************************
 * length                                                             *
 **********************************************************************/
#define D_AIH_REJ_MSG_SIZE         32    /* REJ msg length */
#define D_AIH_ALDBLOG_MSG_SIZE     1200  /* msg log area��size */
#define D_AIH_MSG_MIN_SIZE         2     /* msg�̍ŏ��T�C�Y */
#define D_RRH_LOG_ALD_LOG_NUM      500   /* ALD LOG�̔z�� */

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
src\sys\inc_arch\f_du_reg_aisg.h   ALD�d�����䃌�W�X�^�A�h���X  *************
********************************/
#define D_DU_REG_AISG_ALDPWR	D_DU_REG_CTB_PWR	/* ALD�d������REG addr	*/
#define D_DU_REG_AISG_FIFORST	D_DU_REG_AISG_RXAISG/* AISG Rx FIFO RESET addr */
#define E_AIH_CTRL_CRCERR		0x00000008			/* ��MCRC ERR bit		*/
#define E_AIH_CTRL_LENERR		0x00000007			/* ��MLENGTH ERR bit	*/

/*MSI ID  ALD�f�[�^�]���ʒmID  *************************************/
#define MSI_O_RAN_ALD_ALD_COMMUNICATION_REQ		0x00010101	/**< ��M�ʒm */
#define MSI_O_RAN_ALD_ALD_COMMUNICATION_CFM		0x00020202	/**< ���M�ʒm */
#define MSI_O_RAN_ALD_ALD_COMMUNICATION_REJ		0x00030303	/**< MSI���M�ʒm */
/*        ALD�d������ID  *******************************************/
#define MSI_O_RAN_ALD_PORT_EDIT_REQ				0x04010101	/**< MSI��M�ʒm */
#define MSI_O_RAN_ALD_PORT_EDIT_REJ				0x04020202	/**< MSI���M�ʒm */
#define MSI_O_RAN_ALD_PORT_EDIT_CFM				0x04030303	/**< MSI���M�ʒm */


#endif

/* @} */


