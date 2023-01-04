/*!
 *  @skip   $Id:$
 *  @file   l2_dbg_cpriMsgUnsub.c
 *  @brief  Remove subscriber
 *  @date   2013/11/15 FFCS)hongj Create for zynq
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */


 /*!
 * @addtogroup RRH_L2_DBG
 * @{
 */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "l2_com_inc.h"
#include "rrhApi_L2.h"
#include "tm_lb_def.h"
#include "tm_lb_typ.h"
#include "tm_lb_ext.h"
#include "m_lb_def.h"
#include "m_lb_typ.h"
#include "m_lb_ext.h"

/********************************************************************************************************************/
/*!
 *  @brief  Remove subscriber
 *  @note   Remove subscriber from Cpri message subscription table
 *  @param  *inf_p [in]  Pointer of buffer
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/11/15 FFCS)hongj Create for zynq
 *  @date   2015/8/10 TDIPS)ikeda rev.27999 LPB 17ƒŠƒ“ƒNC³ ’ÊMŽí•Ê‚Íl—¶‚µ‚Ä‚¢‚È‚¢
 */
/********************************************************************************************************************/
VOID l2_dbg_cpriMsgUnsub(UINT *inf_p)
{
    T_API_CPRIMSG_UNSUB_IND *cpriunsub_p;
    UINT count;
    INT cursor_bak;
    INT moveto;
    INT movefrom;
    INT movearea_end;
    INT counter;
    USHORT linkno;

    cpriunsub_p = (T_API_CPRIMSG_UNSUB_IND *)inf_p;
    linkno = cpriunsub_p->link_num;
    /* Remove subscriber*/
    cursor_bak = lbw_L3DstrbtTbl_cur[linkno];
    for (count = 0; count < cursor_bak; count++)
    {
        if (lbw_L3DstrbtTbl[linkno][count][1] == cpriunsub_p->header.uiSrcPQueueID)
        {
            lbw_L3DstrbtTbl[linkno][count][0] = 0x0fffffff;
            lbw_L3DstrbtTbl[linkno][count][1] = 0x0fffffff;
            lbw_L3DstrbtTbl_cur[linkno]--;
        }
        
    }
    if (cursor_bak == lbw_L3DstrbtTbl_cur[linkno])
    {
         cm_Assert( CMD_ASL_DBGLOW, cpriunsub_p->header.uiSrcPQueueID,
            (CHAR*)"[l2_dbg_cpriMsgUnsub]Queue id of unsubscribing is wrong." );
		 return;
    }

    /* pack lbw_L3DstrbtTbl */
    /* find first removed item  */
    for (count = 0; count < cursor_bak; count++)
    {
        if ((lbw_L3DstrbtTbl[linkno][count][0] != 0x0fffffff)
            || (lbw_L3DstrbtTbl[linkno][count][1] != 0x0fffffff))
        {
            continue;
        }
        break;
    }

    counter = 0;
    moveto = count;
    movefrom = count + 1;
    while(movefrom < cursor_bak)
    {
        /* find next subscription item  */
        for (; movefrom < cursor_bak; movefrom++)
        {
            if ((lbw_L3DstrbtTbl[linkno][movefrom][0] != 0x0fffffff)
                || (lbw_L3DstrbtTbl[linkno][movefrom][1] != 0x0fffffff))
            {
                counter++;
                break;
            }            
        }

        /* count the size of subscription items */
        movearea_end = movefrom + 1;
        for (; movearea_end < cursor_bak; movearea_end++)
        {
            if ((lbw_L3DstrbtTbl[linkno][movearea_end][0] != 0x0fffffff)
                || (lbw_L3DstrbtTbl[linkno][movearea_end][1] != 0x0fffffff))
            {
                counter++;
                continue;
            }
            break;
        }

        if (counter > 0) /* There is subscription item to move */
        {
            memcpy(&lbw_L3DstrbtTbl[linkno][moveto][0], &lbw_L3DstrbtTbl[linkno][movefrom][0],
            		sizeof(lbw_L3DstrbtTbl[linkno][0]) * counter);
        }
        moveto = moveto + counter;
        movefrom = movearea_end;
        counter = 0;
    }
	memset(&lbw_L3DstrbtTbl[linkno][lbw_L3DstrbtTbl_cur[linkno]][0], 0,
		sizeof(lbw_L3DstrbtTbl[linkno][0]) * (LBD_L3MSGNUM - lbw_L3DstrbtTbl_cur[linkno]));

    return;
}


/* @} */

