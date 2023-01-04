/****************************************************************************/
/*!
 * @skip  $ld:$
 * @file  mnt_com_ftyp.h
 * @brief mnt com data colloction function declaration.
 * @date  2013/11/28 FFCS)hongj Create for zynq
 * @date  2015/08/23 FPT)Tung Add function sending message between process/thread
 * @date  2015/12/07 TDIPS)sasaki Update
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */
 /****************************************************************************/
#ifndef MNT_COM_FTYP
#define MNT_COM_FTYP

/** The function of sending message betheen process / thread */
extern INT l3_com_sendMsg( UINT awSrcTQueueID, UINT awDstPQueueID, UINT awDstTQueueID, VOID* apMsgBuff, UINT awMsgSize );
extern VOID l3_com_ushortsort_asc( USHORT elm_num, USHORT elm_siz, USHORT *elm );
extern VOID l3_com_heapsort(VOID* array, UINT n, size_t size, INT (*cmp_val)(VOID*, VOID*), VOID (*swap_val)(VOID*, VOID*));
extern INT l3_com_cmp_ushort(VOID* val1, VOID* val2);
extern VOID l3_com_swap_ushort(VOID* val1, VOID* val2);

#endif	/*MNT_COM_FTYP*/
