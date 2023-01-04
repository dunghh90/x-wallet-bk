/*!
* @skip $Id:$
* @file rcfp.h
* @brief Call Processing Header
* @date 2008/09/03 FFCS)Chenhong Create.
* @date 2013/03/21 ALPHA)Matsunobu B25 development.
*
* All Rights Reserved, Copyright (C) FUJITSU LIMITED 2008-2013
*/

/** @addtogroup RRH_PF_RFP
 *  @{
 */
 
 
#ifndef F_RFCP_INC_H
#define F_RFCP_INC_H

/*	#include "dc_tr.sig"	*/
#include "f_sys_inc.h"						/*!< common Header								*/
#include "f_com_inc.h"						/*!< common Header								*/
#include "f_mpsw_msg_o-ran_processing_element_def.h"
#include "f_mpsw_msg_o-ran_processing_element_typ.h"
#include "f_mpsw_msg_o-ran_uplane_conf_def.h"
#include "f_mpsw_msg_o-ran_uplane_conf_typ.h"
#include "f_rfcp_def.h"						/*!< RFCP thread Define Header					*/
#include "f_rfcp_typ.h"						/*!< RFCP thread type header					*/
#include "f_rfcp_ftyp.h"					/*!< RFCP thread's function declaration file	*/
#include "f_rfcp_ext.h"						/*!< RFCP ROM and RAM table extern declaration	*/
#include "f_rfcp_matrix.h"					/*!< RFCP Matrix data							*/
#include "f_rfcp_matrix_local.h"			/*!< RFCP external reference(Matrix data)		*/
#include "f_rfcp_rom.h"						/*!< RFCP ROM data								*/
#include "f_rfcp_rom_local.h"				/*!< RFCP ROM data(local)						*/
#include "f_rfcp_ram.h"						/*!< RFCP RAM data								*/
#include "f_rfcp_ram_local.h"				/*!< RFCP RAM data(local)						*/
#include "../../pf/ft_rfp/cca/inc/f_rfp_typ.h"
#include "rrhApi_Com.h"
#include "rrhApi_Rfp.h"
#include "BPF_COM.h"
#include "BPF_COM_LOG.h"
#include "BPF_RU_IPCM.h"
#include "BPF_RM_SVRM.h"

#endif
/** @} */

