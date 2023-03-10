/*!
 * @skip  $ld:$
 * @file  f_rrh_reg_fb.h
 * @brief レジスタアドレス定義ヘッダ(FB)
 * @date  2015/3/27 FJT) Taniguchi
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_RRH
 * @{
 */
#ifndef F_RRH_REG_FB_H
#define F_RRH_REG_FB_H

/*!
 * @name FBレジスタアドレス
 * @note FBレジスタアドレス, この値は直接使用してはいけない
 * @{
 */
#define	D_RRH_REG_FB_BR							0x00000400
#if defined (OPT_RRH_ARCH_A10)
#define	D_RRH_REG_FB_SWCNTX						0xD2002000
#define	D_RRH_REG_FB_ALUTINIEN					0xD2002004
#define	D_RRH_REG_FB_SWTIME						0xD2002008
#define	D_RRH_REG_FB_SWINTVL					0xD200200C
#define	D_RRH_REG_FB_SWGUARD					0xD2002010
#define	D_RRH_REG_FB_ACMODEA					0xD2002020
#define	D_RRH_REG_FB_ACLENA						0xD2002024
#define	D_RRH_REG_FB_HSMODEA					0xD2002030
#define	D_RRH_REG_FB_HSLENA						0xD2002034
#define	D_RRH_REG_FB_HSCONTINUED				0xD2002038
#define	D_RRH_REG_FB_ACDCO						0xD2002040
#define	D_RRH_REG_FB_ACDLY						0xD2002044
#define	D_RRH_REG_FB_ACOLP						0xD2002048
#define	D_RRH_REG_FB_AKRMDLY					0xD2002074
#define	D_RRH_REG_FB_AKRMOLP					0xD2002078
#define	D_RRH_REG_FB_AKRMUPD					0xD200207C
#define	D_RRH_REG_FB_AKRMDLYMONA				0xD2002084
#define	D_RRH_REG_FB_AKRMOLPMONA				0xD2002088
#define	D_RRH_REG_FB_AKRMUPDMONA				0xD200208C
#define	D_RRH_REG_FB_ACTEST						0xD2002090
#define	D_RRH_REG_FB_ACWARNA					0xD20020A0
#define	D_RRH_REG_FB_BUFLENX					0xD20020D0
#define	D_RRH_REG_FB_BUFLENFLCTX				0xD20020D4
#define	D_RRH_REG_FB_REFPSMPL					0xD20020E0
#define	D_RRH_REG_FB_PSWSMPL					0xD20020E4
#define	D_RRH_REG_FB_POWLAT						0xD20020E8
#define	D_RRH_REG_FB_POWCTRL					0xD20020EC
#define	D_RRH_REG_FB_FPERRX						0xD20020F0
#define	D_RRH_REG_FB_REFPFRMA					0xD2002100
#define	D_RRH_REG_FB_FPERRA						0xD2002104
#define	D_RRH_REG_FB_DPDPFRMA					0xD2002108
#define	D_RRH_REG_FB_PSWREFA					0xD2002110
#define	D_RRH_REG_FB_PSWFBA						0xD2002114
#define	D_RRH_REG_FB_AFSDORPT					0xD2002120
#define	D_RRH_REG_FB_FSDOCTRL					0xD2002124
#define	D_RRH_REG_FB_FSDOMASK					0xD2002128
#define	D_RRH_REG_FB_FSDOMODE					0xD200212C
#define D_RRH_REG_FB_FSDOMAXX					0xD2002134
#define	D_RRH_REG_FB_FBDEMA						0xD2002140
#define	D_RRH_REG_FB_FBROT						0xD2002144
#define	D_RRH_REG_FB_DEMGAINA					0xD2002148
#define	D_RRH_REG_FB_FBDLY						0xD2002180
#define	D_RRH_REG_FB_DFIL						0xD2002184
#define	D_RRH_REG_FB_FBDLYACA					0xD2002188
#define	D_RRH_REG_FB_REFNCOA					0xD20021A0
#define	D_RRH_REG_FB_SSPMU1						0xD2002200
#define	D_RRH_REG_FB_SSPMU2						0xD2002204
#define	D_RRH_REG_FB_SSPMU3						0xD2002208
#define	D_RRH_REG_FB_HISTOGRAM					0xD200220C
#define	D_RRH_REG_FB_LUT0XCFG					0xD2002240
#define	D_RRH_REG_FB_LUT1XCFG					0xD2002244
#define	D_RRH_REG_FB_LUT2XCFG					0xD2002248
#define	D_RRH_REG_FB_LUT3XCFG					0xD200224C
#define	D_RRH_REG_FB_LUT4XCFG					0xD2002250
#define	D_RRH_REG_FB_DLTOFST					0xD2002260
#define	D_RRH_REG_FB_DLTCLP						0xD2002264
#define	D_RRH_REG_FB_DLTOFSTMONA				0xD2002268
#define	D_RRH_REG_FB_DLTRELIM					0xD200226C
#define	D_RRH_REG_FB_CALCMODEX					0xD2002300
#define	D_RRH_REG_FB_ITDATAX					0xD2002320
#define	D_RRH_REG_FB_ITRSTX						0xD2002324
#define	D_RRH_REG_FB_ITMBA						0xD2002328
#define	D_RRH_REG_FB_IT1X						0xD2003240
#define	D_RRH_REG_FB_IT2X						0xD2002244
#define	D_RRH_REG_FB_IT3X						0xD2002248
#define	D_RRH_REG_FB_IT1SUMX					0xD2002250
#define	D_RRH_REG_FB_IT2SUMX					0xD2002254
#define	D_RRH_REG_FB_IT3SUMX					0xD2002258
#define	D_RRH_REG_FB_IT1MSKLOX					0xD2002260
#define	D_RRH_REG_FB_IT2MSKLOX					0xD2002264
#define	D_RRH_REG_FB_IT3MSKLOX					0xD2002268
#define	D_RRH_REG_FB_IT1MSKHIX					0xD2002270
#define	D_RRH_REG_FB_IT2MSKHIX					0xD2002274
#define	D_RRH_REG_FB_IT3MSKHIX					0xD2002278
#define	D_RRH_REG_FB_RFSWSET1X					0xD20023C0
#define	D_RRH_REG_FB_RFSWSET2X					0xD20023C4
#define	D_RRH_REG_FB_TFLP						0xD20023F4
#define	D_RRH_REG_FB_FBMEMKEY					0xD20027F0
#define	D_RRH_REG_FB_FBLASTMODI					0xD20027F4
#define	D_RRH_REG_FB_FBTEST						0xD20027F8
#define	D_RRH_REG_FB_FBMONSELX					0xD20027FC

#elif defined (OPT_RRH_ARCH_ZYNQ)
#define	D_RRH_REG_FB_SWCNTX						0x82002000
#define	D_RRH_REG_FB_ALUTINIEN					0x82002004
#define	D_RRH_REG_FB_SWTIME						0x82002008
#define	D_RRH_REG_FB_SWINTVL					0x8200200C
#define	D_RRH_REG_FB_SWGUARD					0x82002010
#define	D_RRH_REG_FB_ACMODEA					0x82002020
#define	D_RRH_REG_FB_ACLENA						0x82002024
#define	D_RRH_REG_FB_HSMODEA					0x82002030
#define	D_RRH_REG_FB_HSLENA						0x82002034
#define	D_RRH_REG_FB_HSCONTINUED				0x82002038
#define	D_RRH_REG_FB_ACDCO						0x82002040
#define	D_RRH_REG_FB_ACDLY						0x82002044
#define	D_RRH_REG_FB_ACOLP						0x82002048
#define	D_RRH_REG_FB_AKRMDLY					0x82002074
#define	D_RRH_REG_FB_AKRMOLP					0x82002078
#define	D_RRH_REG_FB_AKRMUPD					0x8200207C
#define	D_RRH_REG_FB_AKRMDLYMONA				0x82002084
#define	D_RRH_REG_FB_AKRMOLPMONA				0x82002088
#define	D_RRH_REG_FB_AKRMUPDMONA				0x8200208C
#define	D_RRH_REG_FB_ACTEST						0x82002090
#define	D_RRH_REG_FB_ACWARNA					0x820020A0
#define	D_RRH_REG_FB_BUFLENX					0x820020D0
#define	D_RRH_REG_FB_BUFLENFLCTX				0x820020D4
#define	D_RRH_REG_FB_REFPSMPL					0x820020E0
#define	D_RRH_REG_FB_PSWSMPL					0x820020E4
#define	D_RRH_REG_FB_POWLAT						0x820020E8
#define	D_RRH_REG_FB_POWCTRL					0x820020EC
#define	D_RRH_REG_FB_FPERRX						0x820020F0
#define	D_RRH_REG_FB_REFPFRMA					0x82002100
#define	D_RRH_REG_FB_FPERRA						0x82002104
#define	D_RRH_REG_FB_DPDPFRMA					0x82002108
#define	D_RRH_REG_FB_PSWREFA					0x82002110
#define	D_RRH_REG_FB_PSWFBA						0x82002114
#define	D_RRH_REG_FB_AFSDORPT					0x82002120
#define	D_RRH_REG_FB_FSDOCTRL					0x82002124
#define	D_RRH_REG_FB_FSDOMASK					0x82002128
#define	D_RRH_REG_FB_FSDOMODE					0x8200212C
#define	D_RRH_REG_FB_FSDOMAXX					0x82002134
#define	D_RRH_REG_FB_FBDEMA						0x82002140
#define	D_RRH_REG_FB_FBROT						0x82002144
#define	D_RRH_REG_FB_DEMGAINA					0x82002148
#define	D_RRH_REG_FB_FBDLY						0x82002180
#define	D_RRH_REG_FB_DFIL						0x82002184
#define	D_RRH_REG_FB_FBDLYACA					0x82002188
#define	D_RRH_REG_FB_REFNCOA					0x820021A0
#define	D_RRH_REG_FB_SSPMU1						0x82002200
#define	D_RRH_REG_FB_SSPMU2						0x82002204
#define	D_RRH_REG_FB_SSPMU3						0x82002208
#define	D_RRH_REG_FB_HISTOGRAM					0x8200220C
#define	D_RRH_REG_FB_LUT0XCFG					0x82002240
#define	D_RRH_REG_FB_LUT1XCFG					0x82002244
#define	D_RRH_REG_FB_LUT2XCFG					0x82002248
#define	D_RRH_REG_FB_LUT3XCFG					0x8200224C
#define	D_RRH_REG_FB_LUT4XCFG					0x82002250
#define	D_RRH_REG_FB_DLTOFST					0x82002260
#define	D_RRH_REG_FB_DLTCLP						0x82002264
#define	D_RRH_REG_FB_DLTOFSTMONA				0x82002268
#define	D_RRH_REG_FB_DLTRELIM					0x8200226C
#define	D_RRH_REG_FB_CALCMODEX					0x82002300
#define	D_RRH_REG_FB_ITDATAX					0x82002320
#define	D_RRH_REG_FB_ITRSTX						0x82002324
#define	D_RRH_REG_FB_ITMBA						0x82002328
#define	D_RRH_REG_FB_IT1X						0x82003240
#define	D_RRH_REG_FB_IT2X						0x82002244
#define	D_RRH_REG_FB_IT3X						0x82002248
#define	D_RRH_REG_FB_IT1SUMX					0x82002250
#define	D_RRH_REG_FB_IT2SUMX					0x82002254
#define	D_RRH_REG_FB_IT3SUMX					0x82002258
#define	D_RRH_REG_FB_IT1MSKLOX					0x82002260
#define	D_RRH_REG_FB_IT2MSKLOX					0x82002264
#define	D_RRH_REG_FB_IT3MSKLOX					0x82002268
#define	D_RRH_REG_FB_IT1MSKHIX					0x82002270
#define	D_RRH_REG_FB_IT2MSKHIX					0x82002274
#define	D_RRH_REG_FB_IT3MSKHIX					0x82002278
#define	D_RRH_REG_FB_RFSWSET1X					0x820023C0
#define	D_RRH_REG_FB_RFSWSET2X					0x820023C4
#define	D_RRH_REG_FB_TFLP						0x820023F4
#define	D_RRH_REG_FB_FBMEMKEY					0x820027F0
#define	D_RRH_REG_FB_FBLASTMODI					0x820027F4
#define	D_RRH_REG_FB_FBTEST						0x820027F8
#define	D_RRH_REG_FB_FBMONSELX					0x820027FC

#else
#endif
/*!
 * @name FBレジスタアドレス算出マクロ
 * @note FBレジスタアドレス算出マクロ, アドレスはこのマクロを使用する br は0 ~ 3である
 * @{
 */
#define M_RRH_REG_FB_GETADDR(ofs,br)		( UINT )( ofs  + ( br * D_RRH_REG_FB_BR) )
#define M_RRH_REG_FB_SWCNTX( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_SWCNTX       , br ))
#define M_RRH_REG_FB_ALUTINIEN( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_ALUTINIEN    , br ))
#define M_RRH_REG_FB_SWTIME( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_SWTIME       , br ))
#define M_RRH_REG_FB_SWINTVL( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_SWINTVL      , br ))
#define M_RRH_REG_FB_SWGUARD( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_SWGUARD      , br ))
#define M_RRH_REG_FB_ACMODEA( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_ACMODEA      , br ))
#define M_RRH_REG_FB_ACLENA( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_ACLENA       , br ))
#define M_RRH_REG_FB_HSMODEA( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_HSMODEA      , br ))
#define M_RRH_REG_FB_HSLENA( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_HSLENA       , br ))
#define M_RRH_REG_FB_HSCONTINUED( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_HSCONTINUED  , br ))
#define M_RRH_REG_FB_ACDCO( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_ACDCO        , br ))
#define M_RRH_REG_FB_ACDLY( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_ACDLY        , br ))
#define M_RRH_REG_FB_ACOLP( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_ACOLP        , br ))
#define M_RRH_REG_FB_AKRMDLY( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_AKRMDLY      , br ))
#define M_RRH_REG_FB_AKRMOLP( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_AKRMOLP      , br ))
#define M_RRH_REG_FB_AKRMUPD( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_AKRMUPD      , br ))
#define M_RRH_REG_FB_AKRMDLYMONA( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_AKRMDLYMONA  , br ))
#define M_RRH_REG_FB_AKRMOLPMONA( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_AKRMOLPMONA  , br ))
#define M_RRH_REG_FB_AKRMUPDMONA( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_AKRMUPDMONA  , br ))
#define M_RRH_REG_FB_ACTEST( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_ACTEST       , br ))
#define M_RRH_REG_FB_ACWARNA( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_ACWARNA      , br ))
#define M_RRH_REG_FB_BUFLENX( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_BUFLENX      , br ))
#define M_RRH_REG_FB_BUFLENFLCTX( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_BUFLENFLCTX  , br ))
#define M_RRH_REG_FB_REFPSMPL( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_REFPSMPL     , br ))
#define M_RRH_REG_FB_PSWSMPL( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_PSWSMPL      , br ))
#define M_RRH_REG_FB_POWLAT( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_POWLAT       , br ))
#define M_RRH_REG_FB_POWCTRL( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_POWCTRL      , br ))
#define M_RRH_REG_FB_FPERRX( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_FPERRX       , br ))
#define M_RRH_REG_FB_REFPFRMA( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_REFPFRMA     , br ))
#define M_RRH_REG_FB_FPERRA( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_FPERRA       , br ))
#define M_RRH_REG_FB_DPDPFRMA( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_DPDPFRMA     , br ))
#define M_RRH_REG_FB_PSWREFA( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_PSWREFA      , br ))
#define M_RRH_REG_FB_PSWFBA( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_PSWFBA       , br ))
#define M_RRH_REG_FB_AFSDORPT( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_AFSDORPT     , br ))
#define M_RRH_REG_FB_FSDOMAXX( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_FSDOMAXX     , br ))
#define M_RRH_REG_FB_FBDEMA( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_FBDEMA       , br ))
#define M_RRH_REG_FB_FBROT( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_FBROT        , br ))
#define M_RRH_REG_FB_DEMGAINA( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_DEMGAINA     , br ))
#define M_RRH_REG_FB_FBDLY( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_FBDLY        , br ))
#define M_RRH_REG_FB_DFIL( br )				( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_DFIL         , br ))
#define M_RRH_REG_FB_FBDLYACA( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_FBDLYACA     , br ))
#define M_RRH_REG_FB_REFNCOA( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_REFNCOA      , br ))
#define M_RRH_REG_FB_SSPMU1( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_SSPMU1       , br ))
#define M_RRH_REG_FB_SSPMU2( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_SSPMU2       , br ))
#define M_RRH_REG_FB_SSPMU3( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_SSPMU3       , br ))
#define M_RRH_REG_FB_HISTOGRAM( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_HISTOGRAM    , br ))
#define M_RRH_REG_FB_LUT0XCFG( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_LUT0XCFG     , br ))
#define M_RRH_REG_FB_LUT1XCFG( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_LUT1XCFG     , br ))
#define M_RRH_REG_FB_LUT2XCFG( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_LUT2XCFG     , br ))
#define M_RRH_REG_FB_LUT3XCFG( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_LUT3XCFG     , br ))
#define M_RRH_REG_FB_LUT4XCFG( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_LUT4XCFG     , br ))
#define M_RRH_REG_FB_DLTOFST( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_DLTOFST      , br ))
#define M_RRH_REG_FB_DLTCLP( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_DLTCLP       , br ))
#define M_RRH_REG_FB_DLTOFSTMONA( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_DLTOFSTMONA  , br ))
#define M_RRH_REG_FB_DLTRELIM( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_DLTRELIM     , br ))
#define M_RRH_REG_FB_CALCMODEX( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_CALCMODEX    , br ))
#define M_RRH_REG_FB_ITDATAX( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_ITDATAX      , br ))
#define M_RRH_REG_FB_ITRSTX( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_ITRSTX       , br ))
#define M_RRH_REG_FB_ITMBA( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_ITMBA        , br ))
#define M_RRH_REG_FB_IT1X( br )				( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_IT1X         , br ))
#define M_RRH_REG_FB_IT2X( br )				( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_IT2X         , br ))
#define M_RRH_REG_FB_IT3X( br )				( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_IT3X         , br ))
#define M_RRH_REG_FB_IT1SUMX( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_IT1SUMX      , br ))
#define M_RRH_REG_FB_IT2SUMX( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_IT2SUMX      , br ))
#define M_RRH_REG_FB_IT3SUMX( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_IT3SUMX      , br ))
#define M_RRH_REG_FB_IT1MSKLOX( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_IT1MSKLOX    , br ))
#define M_RRH_REG_FB_IT2MSKLOX( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_IT2MSKLOX    , br ))
#define M_RRH_REG_FB_IT3MSKLOX( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_IT3MSKLOX    , br ))
#define M_RRH_REG_FB_IT1MSKHIX( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_IT1MSKHIX    , br ))
#define M_RRH_REG_FB_IT2MSKHIX( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_IT2MSKHIX    , br ))
#define M_RRH_REG_FB_IT3MSKHIX( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_IT3MSKHIX    , br ))
#define M_RRH_REG_FB_RFSWSET1X( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_RFSWSET1X    , br ))
#define M_RRH_REG_FB_RFSWSET2X( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_RFSWSET2X    , br ))
#define M_RRH_REG_FB_TFLP( br )				( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_TFLP         , br ))
#define M_RRH_REG_FB_FBMEMKEY( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_FBMEMKEY     , br ))
#define M_RRH_REG_FB_FBLASTMODI( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_FBLASTMODI   , br ))
#define M_RRH_REG_FB_FBTEST( br )			( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_FBTEST       , br ))
#define M_RRH_REG_FB_FBMONSELX( br )		( M_RRH_REG_FB_GETADDR( D_RRH_REG_FB_FBMONSELX    , br ))
/* @} */

/*!
 * @name レジスタ値
 * @note レジスタ値
 * @{
 */
/* DEMGAIN */
#define D_RRH_FB_DEMGAIN_I					0x00003FFF
#define D_RRH_FB_DEMGAIN_Q					0x3FFF0000

/* FBDLY */
#define D_RRH_FB_FBDLY_CLK					0x0003FF00
#define D_RRH_FB_FBDLY_FIL					0x000000FE

/* FBDLY */
#define D_RRH_FB_POWLAT_LAT					0x00000001

/* @} */


#endif
/* @} */
