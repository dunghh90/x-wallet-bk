#ifndef SAA_COM_INIT_H
#define SAA_COM_INIT_H


/*!
 * @skip        $ld:$
 * @file        saa_com_init.h
 * @brief       SAA_COM SIL-callback parameter get define
 * @date        2018/11/19 KCN)Wakamizu Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup    SAA_COM
 * @brief
 * @note
 * @{
 */

/* @} */


/** @name return値
 *  @{
 */
#define    D_SYS_OK    0    /**< 正常終了        */
#define    D_SYS_NG  (-1)   /**< 異常終了        */
/** @} */

/** @name MAX_validate_list数
 *  @note validateのリスト最大数
 *  @{
 */
#define D_SAA_COM_VALIDATE_MAX 128 /**< 1パラメータを分割した際の要素数最大値 */
/** @} */

/** @name MAX_文字数
 *  @note init_validate文字数最大値
 *  @{
 */
#define D_PARAM_NUM_MAX 256  /**< パラメータ文字数最大値 */
/** @} */

typedef struct{
    char list[D_SAA_COM_VALIDATE_MAX][D_PARAM_NUM_MAX];
}T_SAA_COM_PARAM_LIST_TBL;



/** @name function list
 *  @note 関数定義
 *  @{
 */
extern int   f_saa_com_load_init_data(char *, char *, char *);
extern void  f_saa_com_dump_init_data(void);
extern char *f_saa_com_get_init_data(char *, char *);
extern char *f_saa_com_get_validate_data(char *, char *);
extern int   f_saa_com_conversion_validate_data_max_min(char *, int64_t*, int64_t*);
extern int   f_saa_com_conversion_validate_data_list(char *, T_SAA_COM_PARAM_LIST_TBL*);
extern int   f_saa_com_validate_memaddr( unsigned char* chkbufp );
/** @} */

#endif
/* @} */
