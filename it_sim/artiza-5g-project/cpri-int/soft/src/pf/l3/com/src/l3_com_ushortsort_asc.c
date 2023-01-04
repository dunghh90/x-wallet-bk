/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   l3_com_ushortsort_asc.c
 * @brief  昇順ソート関数(unsigned short用)
 * @date   2015/10/27 FPT)Yen create
 * @date   2015/12/07 TDIPS)sasaki update
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "m_cm_header.h"

/*! @addtogroup RRH_L3_COM
 * @{ */

/****************************************************************************/
/*!
 * @brief  昇順ソート関数(unsigned short用)
 * @note   運用中動作パターンに応じた処理を行う
 * @param  elm_num [in] ソート対象要素数
 * @param  elm_siz [in] ソート対象要素数
 * @param  elm     [in] ソート対象要素配列アドレス
 * @return None
 * @date   2015/10/27 FPT)Yen create
 */
/****************************************************************************/
VOID l3_com_ushortsort_asc(                             /* なし:R */
    USHORT elm_num,                                   /*  ソート対象要素数  */
    USHORT elm_siz,                                   /*  ソート対象要素数  */
    USHORT *elm                             /*  ソート対象要素配列アドレス  */
) 
{
    USHORT* elm_wk;                                 /* 要素入れ替え用ワーク */
    USHORT  i;                                            /* ループカウンタ */
    USHORT  j;                                            /* ループカウンタ */

	if ((elm_num == CMD_NUM0) || (elm_siz == CMD_NUM0)) {
        return;
    }

    /****************/
    /* バッファ取得 */
    /****************/
    elm_wk = malloc(                         /* 終了コード:R */               /* pgr0520 */
        elm_siz * sizeof(USHORT)                       /* バッファサイズ:I */ /* pgr0520 */
    );                                                                        /* pgr0520 */
	cm_MemClr(elm_wk, elm_siz * sizeof(USHORT));


    for(i = CMD_NUM0; i < (USHORT)(elm_num-CMD_NUM1); i++) {
        for(j = elm_num-CMD_NUM1; j > i; j--) {
            if(elm[j*elm_siz] < elm[(j-CMD_NUM1)*elm_siz]){
                /* elm_wk   = elm[j];   */
                cm_MemCpy(
                    elm_wk, 
                    &elm[j*elm_siz],
                    elm_siz * sizeof(USHORT)
                );
                /* elm[j]   = elm[j-1]; */
                cm_MemCpy(
                    &elm[j*elm_siz],
                    &elm[(j-CMD_NUM1)*elm_siz],
                    elm_siz * sizeof(USHORT)
                );
                /* elm[j-1] = elm_wk;   */
                cm_MemCpy(
                    &elm[(j-CMD_NUM1)*elm_siz],
                    elm_wk,
                    elm_siz * sizeof(USHORT)
                );
            }
        }
    }
    
    /****************/
    /* バッファ解放 */
    /****************/
    free(                                             /* なし:R */
        elm_wk                                        /* バッファアドレス:I */
    );
}


/****************************************************************************/
/*!
 * @brief  ヒープ処理関数
 * @note   
 * @param  array    [in] ソート対象要素配列アドレス
 * @param  root     [in] ヒープ処理対象部分木の根となる配列index
 * @param  n        [in] ソート対象要素数
 * @param  size     [in] ソート対象要素サイズ
 * @param  cmp_val  [in] ソート対象要素比較関数へのポインタ
 * @param  swap_val [in] ソート対象要素置換関数へのポインタ
 * @return None
 * @date   2015/12/07 TDIPS)sasaki Create
 */
/****************************************************************************/
VOID l3_com_heapify(VOID* array, UINT root, UINT n, size_t size, INT (*cmp_val)(VOID*, VOID*), VOID (*swap_val)(VOID*, VOID*))
{
	UINT	i;

	while(root < (n / 2))
	{
		i = (root + 1) * 2 - 1;
		if(i < (n - 1))
		{
			if(cmp_val(((UCHAR *)array + i * size), ((UCHAR *)array + (i + 1) * size)) < 0)
			{
				i++;
			}
		}
		if(cmp_val(((UCHAR *)array + root * size), ((UCHAR *)array + i * size)) < 0)
		{
			swap_val(((UCHAR *)array + root * size), ((UCHAR *)array + i * size));
			root = i;
		}
		else
		{
			break;
		}
	}
}

/****************************************************************************/
/*!
 * @brief  配列データソート関数
 * @note   
 * @param  array    [in] ソート対象要素配列アドレス
 * @param  n        [in] ソート対象要素数
 * @param  size     [in] ソート対象要素サイズ
 * @param  cmp_val  [in] ソート対象要素比較関数へのポインタ
 * @param  swap_val [in] ソート対象要素置換関数へのポインタ
 * @return None
 * @date   2015/12/07 TDIPS)sasaki Create
 */
/****************************************************************************/
VOID l3_com_heapsort(VOID* array, UINT n, size_t size, INT (*cmp_val)(VOID*, VOID*), VOID (*swap_val)(VOID*, VOID*))
{
	INT	i;

	for(i = ((n / 2) - 1); i >= 0; i--)
	{
		l3_com_heapify(array, i, n, size, cmp_val, swap_val);
	}
	for(i = n; i > 0; i--)
	{
		swap_val(array, ((char *)array + (i - 1) * size));	
		l3_com_heapify(array, 0, (i - 1), size, cmp_val, swap_val);
	}
}

/****************************************************************************/
/*!
* @brief  配列データ比較関数(USHORT用)
 * @note   
 * @param  val1     [in] 比較対象データ1
 * @param  val2     [in] 比較対象データ2
 * @return INT
 * @retval 0:val1 == val2, 正:val1 > val2, 負:val1 < val2
 * @date   2015/12/07 TDIPS)sasaki Create
 */
/****************************************************************************/
INT l3_com_cmp_ushort(VOID* val1, VOID* val2)
{
	return ((INT)(*(USHORT *)val1) - (*(USHORT *)val2));
}

/****************************************************************************/
/*!
* @brief  配列データ置換関数(USHORT用)
 * @note   
 * @param  val1     [in] 置換対象データ1
 * @param  val2     [in] 置換対象データ2
 * @return None
 * @date   2015/12/07 TDIPS)sasaki Create
 */
/****************************************************************************/
VOID l3_com_swap_ushort(VOID* val1, VOID* val2)
{
	USHORT save = (*(USHORT *)val1);

	(*(USHORT *)val1) = (*(USHORT *)val2);
	(*(USHORT *)val2) = save;
}

/* @} */

/****************************************************************************/
/*
 */
/****************************************************************************/
