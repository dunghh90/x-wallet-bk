/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_com_Fhex_dmp.c
 * @brief  ヘキサダンプ
 * @date   2007/03/20 FJT)Nagasima create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_com.h"
/*! @addtogroup TRIF_TCM
 * @{ */

/****************************************************************************/
/*!
 * @brief  ヘキサダンプ
 * @note   指定ストリームにヘキサダンプする
 * @param  stream [in/out]  出力ストリーム
 * @param  data   [in]  ダンプアドレス
 * @param  length [in]  ダンプサイズ
 * @return None
 * @date   2007/04/25 FJT)Nagasima create.
 * @date   2015/09/03 FPT)Lay modify for FHM
 */
/****************************************************************************/
VOID f_trp_com_Fhex_dmp(                                          /* なし:R */
    FILE* stream,                                       /* 出力ストリーム:I */
    CHAR* data,                                         /* ダンプアドレス:I */
    UINT  length                                          /* ダンプサイズ:I */
) {
    INT line_num;
    INT line_idx;
    INT clmn_num;
    INT clmn_idx;
    CHAR adr_str[128];
    CHAR dat_chr[4];
    CHAR dat_str[128];
    CHAR asc_chr[4];
    CHAR asc_str[128];
    INT tmp;

    /* ストリームがNULLの場合 */
    if (stream == NULL) {
        /* 標準出力に変更 */
        stream = stdout;
    }

    line_num = (INT)(length/16);
    if (length%16 != 0) {
        line_num++;
    }

    for (line_idx=0 ; line_idx<line_num ; line_idx++) {
        sprintf(adr_str, "0x%08X ", (UINT)data);
        if (line_idx == line_num-1) {
            clmn_num = length - (line_num-1)*16;
        } else {
            clmn_num = 16;
        }
        memset(dat_str, 0x00, sizeof(dat_str));
        memset(asc_str, 0x00, sizeof(asc_str));
        for (clmn_idx=0 ; clmn_idx<clmn_num ; clmn_idx++) {
            sprintf(dat_chr, "%02X ", (UCHAR)*(data));
            strcat(dat_str, dat_chr);
            tmp = (*data)&0x000000FF;
            if (isprint(tmp)!=0) {
                sprintf(asc_chr, "%1c", *data);
                strcat(asc_str, asc_chr);
            } else {
                strcat(asc_str, ".");
            }
            if ((clmn_idx+1)%8==0) {
                strcat(dat_str, " ");
                strcat(asc_str, " ");
            }
            data++;
        }
        if (clmn_num != 0) {
            fprintf( stream, "%s  %-50s %-18s\n", adr_str, dat_str, asc_str);
        }
    }

    return;
}

/****************************************************************************/
/*!
 * @brief  ヘキサダンプ
 * @note   指定ストリームにヘキサダンプする
 * @param  data   [in]  ダンプアドレス
 * @param  length [in]  ダンプサイズ
 * @param  offset [in]  ダンプオフセット
 * @return None
 * @date   2008/12/26 FFCS)Shihzh create.
 */
/****************************************************************************/
VOID f_trp_com_Fhex_dmp2(                                         /* なし:R */
    CHAR* data,                                         /* ダンプアドレス:I */
    UINT  length,                                         /* ダンプサイズ:I */
    UINT  offset                                      /* ダンプオフセット:I */
) {
#if 0
    INT line_num;
    INT line_idx;
    INT clmn_num;
    INT clmn_idx;
    CHAR oft_str[128];
    CHAR dat_chr[4];
    CHAR dat_str[128];
    CHAR asc_chr[4];
    CHAR asc_str[128];
    INT tmp;

    line_num = (INT)(length/16);
    if (length%16 != 0) {
        line_num++;
    }

    for (line_idx=0 ; line_idx<line_num ; line_idx++) {
        sprintf(oft_str, "0x%08X ", (UINT)offset);
        if (line_idx == line_num-1) {
            clmn_num = length - (line_num-1)*16;
        } else {
            clmn_num = 16;
        }
        memset(dat_str, 0x00, sizeof(dat_str));
        memset(asc_str, 0x00, sizeof(asc_str));
        for (clmn_idx=0 ; clmn_idx<clmn_num ; clmn_idx++) {
            sprintf(dat_chr, "%02X ", (UCHAR)*(data));
            strcat(dat_str, dat_chr);
            tmp = (*data)&0x000000FF;
            if (isprint(tmp)!=0) {
                sprintf(asc_chr, "%1c", *data);
                strcat(asc_str, asc_chr);
            } else {
                strcat(asc_str, ".");
            }
            if ((clmn_idx+1)%8==0) {
                strcat(dat_str, " ");
                strcat(asc_str, " ");
            }
            data++;
            offset++;
        }
        if (clmn_num != 0) {
            f_com_cmm_dbg_Fprintf("%s  %-50s %-18s\n", oft_str, dat_str, asc_str);
        }
    }
#endif
    return;
}

/****************************************************************************/
/*
 */
/****************************************************************************/
/* @} */  /* group TRIF_TCM */
