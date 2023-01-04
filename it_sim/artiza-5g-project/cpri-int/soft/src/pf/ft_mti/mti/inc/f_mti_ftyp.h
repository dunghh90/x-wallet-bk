/*!
 * @skip		$ld:$
 * @file		f_mti_ftyp.h
 * @brief		MTIスレッド プロトタイプ宣言
 * @date		2019/05/13 FJT)Taniguchi Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

/*!
 * @addtogroup RRH_PF_MTI
 * @{
 */
#ifndef F_MTI_FTYP_H
#define F_MTI_FTYP_H

extern VOID f_mti_main( VOID );
extern INT  f_mti_event( ssize_t rlen, UCHAR* rdataBuf , UCHAR* sdataBuf , ssize_t* slen);
extern VOID f_mti_mtLog(const char* Message_p, ... );
extern VOID f_mti_send(UCHAR* senddata_p ,ssize_t slen );

#endif
/* @} */
