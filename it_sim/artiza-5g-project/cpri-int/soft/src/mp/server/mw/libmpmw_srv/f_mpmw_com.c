/*!
 *  @skip   $Id:$
 *  @file   f_mpsw_com.c
 *  @brief  Mplane Middleware function library for Server
 *  @author    KCN) Nakai Hiroyuki
 *  @date   2018/11/19 FJT)Taniguchi create
 *
 *  All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */


/** @addtogroup MPMW_SRV
 * @{ */

#include <string.h>
#include "f_mpmw_com.h"

/* ====== ログメッセージ振り分けのためのキーワード ====== */

/* notification */
static const char *keywords_notification[] = {
    "f_saa_com_log_notification",
    NULL
};

/* o-ran-supervision: 全てのログ */
static const char *keywords_supervision[] = {
    "u_o_ran_supervision",
    "i_o_ran_supervision",
    NULL
};

/* o-ran-performance-management: notificationのみ */
static const char *keywords_performance_management[] = {
    "u_o_ran_performance_management_measurement_result_stats_send",
    "i_o_ran_performance_management_measurement_result_stats_handler",
    NULL
};

/* ====== メッセージ振り分けテーブル ====== */

typedef struct {
    int offset;
    const char **keywords;
} t_mpmw_srv_com_offset_tbl;

static t_mpmw_srv_com_offset_tbl mod_offset_table[] = {
    {3, keywords_notification},
    {1, keywords_supervision},
    {2, keywords_performance_management}
};


/************************************************************************/
/*!
 *  @brief  メッセージ内容に応じたオフセット値を返す。
 *  @note   特定のキーワードから始まるメッセージをデフォルト以外のログファイルに出力する。
 *  @param  msg メッセージ
 *  @return オフセット値
 *  @date   2019/7/4 KCN)Nakai crate
 */
/************************************************************************/
int f_mpmw_get_offset(const char *msg){

    int i,j;
    const char *keyword = NULL;
    int max = sizeof(mod_offset_table)/sizeof(t_mpmw_srv_com_offset_tbl);

    for(i=0; i<max; i++){
        for(j=0; mod_offset_table[i].keywords[j] != NULL; j++){
            keyword = mod_offset_table[i].keywords[j];
            if(strstr(msg, keyword) != NULL){
                 return mod_offset_table[i].offset;
            }
        }
    }

    return 0;
}

/* 以下はテスト用。使う場合はテスト用にビルドして動かすこと。

 > cc -DTEST <this file>

*/

#ifdef TEST
#include <stdio.h>
void main(int argc, char *argv[]){

    int offset = 0;
    int i = 0;
    const char *s = NULL;

    offset = f_mpmw_get_offset("aaa");
    printf("%d\n",offset);

    const char * test_str[] = {
        "Enter :u_o_ran_supervision",
        "Enter :i_o_ran_supervision",
        "Enter: u_o_ran_performance_management_measurement_result_stats_send",
        "Enter: i_o_ran_performance_management_measurement_result_stats_handler",
        "Enter: u_o_ran_performance_management_init2"
    };

    for(i=0; i< sizeof(test_str)/sizeof(char *); i++){
        s = test_str[i];
        offset = f_mpmw_get_offset(s);
        printf("%s -> offset=%d\n", s, offset);
    }

}
#endif

/* @} */
