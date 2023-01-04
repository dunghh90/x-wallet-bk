/*!
 *  @skip   $Id:$
 *  @file   f_com_allRetReset.c
 *  @brief  All Ret Reset function.
 *  @date  2010/10/11 FFCS)Shihzh Create
 *
 *  All Rights Reserved, Copyright (C) FUJITSU LIMITED 2010
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "f_com_inc.h"
#include "f_com_aisg.h"

 
/*!
 *  @brief  All Ret Reset function.
 *  @note   This function processes the following behaviors.\n
 *          - Make AISG broadcast reset command
 *          - Call AISG send function to send command
 *  @return VOID.
 *  @retval none
 *  @date  2010/10/11 FFCS)Shihzh Create
*/
VOID f_com_allRetReset()
{
	UCHAR ucRstCmd[D_SYS_NUM7];		/* address(1) + control(1) + 5 */

	ucRstCmd[D_SYS_NUM0] = 0xFF;	/* Address(0xFF) */
	ucRstCmd[D_SYS_NUM1] = 0xBF;	/* Control(Xid)	 */
	ucRstCmd[D_SYS_NUM2] = 0x81;	/* FI(0x81)		 */
	ucRstCmd[D_SYS_NUM3] = 0xF0;	/* GI(0xF0)		 */
	ucRstCmd[D_SYS_NUM4] = 0x02;	/* GL(2)		 */
	ucRstCmd[D_SYS_NUM5] = 0x07;	/* PI(7:Reset)	 */
	ucRstCmd[D_SYS_NUM6] = 0x00;	/* PL(0)		 */

	/* Send AISG L1 Data */
	f_com_bspAISGSnd((char*)ucRstCmd, sizeof(ucRstCmd),D_SYS_AISG_BAUDRATE_9600,D_SYS_AISG_LITTLE_ENDIAN,0);

    return;
}
/* @} */
