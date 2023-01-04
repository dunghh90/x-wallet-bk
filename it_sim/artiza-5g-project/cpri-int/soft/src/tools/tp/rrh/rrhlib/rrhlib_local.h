#ifndef __RRHLIB_LOCAL_H__
#define __RRHLIB_LOCAL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <errno.h> 
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <mruby.h>
#include <mruby/proc.h>
#include <mruby/array.h>
#include <mruby/string.h>
#include <mruby/compile.h>
#include <mruby/dump.h>
#include <mruby/variable.h>
#include "f_rrh_inc.h"
#include "BPF_COM_END.h"
#include "BPF_COM_PLOG.h"
#include "BPF_HM_DEVC.h"
#include "RFIC_PMC.h"
#include "BPF_RU_IPCM.h"
#include "BPF_RM_SVRM.h"
#include "rrhApi_Rfp.h"
#include "rrhApi_Inv.h"
#include "f_rrh_reg_flash.h"
#include "f_du_reg_eep.h"


void rrhlib_soc_install(mrb_state* mrb ,struct RClass * module_class);
void f_rrhlib_dmsg(const char* filename , const int line,  const char* format, ...);
#define rrhlib_dmsg(...) f_rrhlib_dmsg( __FILE__ , __LINE__ , __VA_ARGS__ )

void rrhlib_file_install(mrb_state* mrb ,struct RClass * module_class);
void rrhlib_device_access_install(mrb_state* mrb ,struct RClass * module_class);
void rrhlib_api_install(mrb_state* mrb ,struct RClass * module_class);
void rrhlib_tbl_access_install(mrb_state* mrb ,struct RClass * module_class);

extern unsigned int rrhlib_dev_assignflg;
#ifdef RRHLIB_MEM_CHK
int rrhlib_memchecker( unsigned char* chkbufp );
#define rrhlib_memchecker(addr) f_rrhlib_memchecker(addr)
#else
#define rrhlib_memchecker(addr) 0
#endif


/********************************************************************************************************************/
/* Device access define                                                                                             */
/********************************************************************************************************************/
/* 共通の定義           */
#define D_DBG_TP_OK                             0
#define D_DBG_TP_NG                             1
#define D_DBG_TP_TRUE                           0               	/**< TRUE                       */
#define D_DBG_TP_FALSE                          -1              	/**< FALSE                      */
/* 共通パラメータ */
#define D_DBG_TP_DL_DIRNAME                   "/tmp/"             	/**< ダウンロードディレクトリ   */
#define D_DBG_TP_INIT_CRCPOLY1                  0x1021          	/*!< CRC16計算用    x^{16}+x^{12}+x^5+1         */
#define D_DBG_TP_PROCESS_DESC                   D_RRH_PROCID_DBG	/* DBGのプロセスID */

/* FLASH */
#define D_DBG_TP_BOOT_OFFSET          			D_RRH_QSPI_FLASH_OFFSET_BOOT      /**< boot                       */
#define D_DBG_TP_FIRM_OFFSET           			D_RRH_QSPI_FLASH_OFFSET_OS        /**< OS                         */
#define D_DBG_TP_RFIC_RX_OFFSET       			D_RRH_QSPI_FLASH_CLEMENT_RX          /**< RFIC(RX)                   */
#define D_DBG_TP_RFIC_TX_OFFSET        			D_RRH_QSPI_FLASH_CLEMENT_TX          /**< RFIC(TX)                   */
#define D_DBG_TP_FPGA_CP_OFFSET        			D_RRH_QSPI_FLASH_CPRI_FPGA        /**< FPGA(CP)                   */

#define D_DBG_TP_BOOT_SIZE                      0x00100000      /**< boot                       */
#define D_DBG_TP_FIRM_SIZE                      0x00600000      /**< OS                         */
#define D_DBG_TP_RFIC_RX_SIZE                   0x00080000      /**< RFIC(RX)                   */
#define D_DBG_TP_RFIC_TX_SIZE                   0x00080000      /**< RFIC(TX)                   */
#define D_DBG_TP_FPGA_CP_SIZE                   0x00D00000      /**< FPGA(CP)                   */

/* SPI */
#define D_DBG_TP_SPI_DEVICE_NUM					5

/* 共有メモリ */
#define D_DBG_TP_SHM_ALMLOG_CARINFO				1				/* CARINFOの種別番号 		*/
#define D_DBG_TP_SHM_ALMLOG_STATUSINFO			2				/* STATUSINFOの種別番号 	*/
#define D_DBG_TP_SHM_MNGTBL_FLAG				11				/* FLAGの種別番号 			*/
#define D_DBG_TP_SHM_MNGTBL_TRAINING			12				/* TRAININGの種別番号 		*/


#define D_DBG_TP_SHM_FLG_DBG					1				/* デバッグモードフラグ		*/
#define D_DBG_TP_SHM_FLG_TEMP					2				/* 温度補償フラグ			*/
#define D_DBG_TP_SHM_FLG_NOMOD					3				/* 無変調フラグ				*/
#define D_DBG_TP_SHM_FLG_ALMMSK					4				/* ALMマスク				*/


#define D_DBG_TP_SHM_CARINFO_NUM				7				/* CARINFOのパラメータ数 */
#define D_DBG_TP_SHM_STATUSINFO_NUM				6				/* STATUSINFOのパラメータ数 */
#define D_DBG_TP_SHM_FLAG_NUM					2				/* フラグの読み取りパラメータ数(デバッグモード、) */
#define D_DBG_TP_SHM_TRAINING_NUM				1				/* トレーニングの読み取りパラメータ数 */

/* レジスタサイズ */
#define D_DBG_TP_EEPROM_SIZE_MAX                0x7FF           /**< EEPROM最大サイズ           */
#define D_DBG_TP_CPLD_SIZE_MAX                  0x88            /**< CPLD最大サイズ             */
#define D_DBG_TP_FLASH_SIZE_MAX                 0x01FFFFFF      /**< FLASHオフセット最大サイズ  */

/* API呼び出し用 */
#define D_DBG_TP_API_WAITTIME					0

/*装置種別*/

#ifndef BPF_HM_DEVC_SUB6
#define BPF_HM_DEVC_SUB6						D_DU_EEP_DEV_KIND_5GDU_SUB6			/** 5G-DU治具(sub6)    */
#endif
#ifndef BPF_HM_DEVC_MMW
#define BPF_HM_DEVC_MMW							D_DU_EEP_DEV_KIND_5GDU_MMW				/** 5G-DU治具(mmW)     */
#endif

#define D_BPF_HM_DEVC_FREQ_MASK					0x00FF0000		/* 装置種別 : 対応周波数取得マスク値	*/
#define D_BPF_HM_DEVC_FREQ_SHFT					16				/* 装置種別 : 対応周波数取得ソフト値	*/
#define D_BPF_HM_DEVC_KIND_MASK					0x000000FF		/* 装置種別 : 装置種別取得マスク値		*/

#define M_BPF_HM_DEVC_FREQ_GET(val)		(UINT)( (val & D_BPF_HM_DEVC_FREQ_MASK) >> D_BPF_HM_DEVC_FREQ_SHFT )
#define M_BPF_HM_DEVC_KIND_GET(val)		(UINT)( val & D_BPF_HM_DEVC_KIND_MASK )

#endif
