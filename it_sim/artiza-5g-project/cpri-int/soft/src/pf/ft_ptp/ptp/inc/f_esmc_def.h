/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	ESMC  DEFINE
 *  @date	2018/8/28
 *
 *  COPYRIGHT FUJITSU LIMITED 2018
 */
/******************************************************************************************************************************/

#ifndef F_PTP_ESMC_DEF_H_
#define F_PTP_ESMC_DEF_H_

/* SSM Code */
#define ESMC_QL_NONE				0x00
#define ESMC_QL_PRS					0x01
#define ESMC_QL_PRC					0x02
#define ESMC_QL_SSU_A				0x04
#define ESMC_QL_SSU_B				0x08
#define ESMC_QL_ST2					0x07
#define ESMC_QL_ST3					0x0A
#define ESMC_QL_EEC2				0x0A
#define ESMC_QL_SEC					0x0B
#define ESMC_QL_EEC1				0x0B
#define ESMC_QL_ST3E				0x0D
#define ESMC_QL_DNU					0x0F

#define SSM_REV_MASK				0x0F

/* 使用ポート定義 */
#define PORT_NOUSE					0
#define PORTS0_USE					1
#define PORTS1_USE					2


#endif /* F_PTP_ESMC_DEF_H_ */
