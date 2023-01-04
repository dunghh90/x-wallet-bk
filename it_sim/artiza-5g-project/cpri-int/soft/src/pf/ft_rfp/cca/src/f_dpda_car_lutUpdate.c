
#include "f_dpda_inc.h"


/*!
 * @brief 関数機能概要:LUT設定
 * @note  関数処理内容.
 *          -# LUT設定を行う
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(ANT.A~ANT.H)
 * @return int
 * @date 2018/09/10 KCN)南條 Create
 */
int f_dpda_car_lutUpdate(unsigned int uiAntNum)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_car_lutUpdate called, uiAntNum = %d", uiAntNum);

    unsigned int ret = BPF_HM_DEVC_COMPLETE;
    unsigned int i;
    unsigned int regAddrMap[26];
    unsigned int write_data[26];

    /* LUT設定先のレジスタアドレスマップ作成 */
    regAddrMap[0]   = 0xB1200000;      /* ADR1ADJ  */
    regAddrMap[1]   = 0xB1200004;      /* ADR2ADJ  */
    regAddrMap[2]   = 0xB1200008;      /* ADR3ADJ  */
    regAddrMap[3]   = 0xB1200010;      /* ADR1CNT  */
    regAddrMap[4]   = 0xB1200014;      /* ADR2CNT  */
    regAddrMap[5]   = 0xB1200018;      /* ADR3CNT  */
    regAddrMap[6]   = 0xB1200040;      /* TRTIME1  */
    regAddrMap[7]   = 0xB1200044;      /* TRTIME2  */
    regAddrMap[8]   = 0xB1200048;      /* TRTIME3  */
    regAddrMap[9]   = 0xB120004C;      /* TRTIME4  */
    regAddrMap[10]  = 0xB1200050;     /* TRTIME5  */
    regAddrMap[11]  = 0xB1200054;     /* TRTIME6  */
    regAddrMap[12]  = 0xB1200058;     /* TRTIME7  */
    regAddrMap[13]  = 0xB1200060;     /* TRSLUTEN  */
    regAddrMap[14]  = 0xB1200080;     /* LUT0     */
    regAddrMap[15]  = 0xB1200084;     /* LUT1     */
    regAddrMap[16]  = 0xB1200088;     /* LUT2     */
    regAddrMap[17]  = 0xB120008C;     /* LUT3     */
    regAddrMap[18]  = 0xB1200090;     /* LUT4     */
    regAddrMap[19]  = 0xB1200094;     /* LUT5     */
    regAddrMap[20]  = 0xB1200098;     /* LUT6     */
    regAddrMap[21]  = 0xB120009C;     /* LUT7     */
    regAddrMap[22]  = 0xB12000A0;     /* LUT8     */
    regAddrMap[23]  = 0xB1200100;     /* APDON    */
    regAddrMap[24]  = 0xB1203080;     /* SSPMU1   */
    regAddrMap[25]  = 0xB1203084;     /* SSPMU2   */

    /* LUT設定先のWrite Dataマップ作成 */
    write_data[0]   = 0x4AE4DB46;      /* ADR1ADJ  */
    write_data[1]   = 0x00200000;      /* ADR2ADJ  */
    write_data[2]   = 0x4AE4DB46;      /* ADR3ADJ  */
    write_data[3]   = 0x00000000;      /* ADR1CNT  */
    write_data[4]   = 0x00000201;      /* ADR2CNT  */
    write_data[5]   = 0x00000002;      /* ADR3CNT  */
    write_data[6]   = 0x0000008B;      /* TRTIME1  */
    write_data[7]   = 0x000006F5;      /* TRTIME2  */
    write_data[8]   = 0x00000780;      /* TRTIME3  */
    write_data[9]   = 0x00000780;      /* TRTIME4  */
    write_data[10]  = 0x00000780;      /* TRTIME5  */
    write_data[11]  = 0x00000000;      /* TRTIME6  */
    write_data[12]  = 0x00000000;      /* TRTIME7  */
    write_data[13]  = 0x000000F8;      /* TRSLUTEN  */
    write_data[14]  = 0x01FF01FF;      /* LUT0     */
    write_data[15]  = 0x01FF01FF;      /* LUT1     */
    write_data[16]  = 0x01FF01FF;      /* LUT2     */
    write_data[17]  = 0x01FF01FF;      /* LUT3     */
    write_data[18]  = 0x01FF01FF;      /* LUT4     */
    write_data[19]  = 0x01FF01FF;      /* LUT5     */
    write_data[20]  = 0x01FF01FF;      /* LUT6     */
    write_data[21]  = 0x01FF01FF;      /* LUT7     */
    write_data[22]  = 0x01FF01FF;      /* LUT8     */
    write_data[23]  = 0x00000001;      /* APDON    */
    write_data[24]  = 0x18000100;      /* SSPMU1   */
    write_data[25]  = 0x01000040;      /* SSPMU2   */

    for(i = 0; i < ( sizeof(regAddrMap)/sizeof(regAddrMap[0]) ) ; i++)
    {
        /* ANT:A~D */
        regAddrMap[i] = regAddrMap[i] + (uiAntNum * 0x400);
        ret = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, regAddrMap[i], &write_data[i]);
        if(ret != BPF_HM_DEVC_COMPLETE)
        {
            BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "LUT write ant = %d , regAddrMap[%d]=%d , error=0x%08x",uiAntNum, i, regAddrMap[i], ret);
            return D_DPDA_RTN_NG;
        }
    }

    return D_DPDA_RTN_COMPLETE;
}

/* @} */

