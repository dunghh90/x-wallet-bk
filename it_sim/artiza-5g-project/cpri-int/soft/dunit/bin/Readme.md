
# Process modules example
## INIT Process modules
- INITプロセス内の関数をコール、関数シンボル名、引数の順の順で指定  
`INIT.module("f_init_xxx","0") `  

- INITプロセス内のCUnitのテストスイートをコール、スイート名、CUnitの起動モード
`INIT.cunit("test_suites_array_ft_init",0)`
- INITプロセス内のテーブルに値を設定、テーブルシンボル名、開始オフセット、サイズ、値の順で指定  
`INIT.tblset("f_iniw_yyy","0","1","01")`　　

- INITプロセス内のテーブルをリード テーブルシンボル名、開始オフセット、サイズの順で指定
`puts INIT.tblget("f_iniw_yyy","0","1")`  

- INITプロセス内のテーブルのアドレスを取得、テーブルシンボル名  
`puts INIT.tbladdr("f_iniw_yyy")`  

## PF Process modules
- PFプロセス内の関数をコール、関数シンボル名、引数の順の順で指定  
`PF.module("f_main_xxx","0")`  

- PFプロセス内のCUnitのテストスイートをコール、スイート名、CUnitの起動モード   
`PF.cunit("test_suites_array_ft_main",0)`  

- PFプロセス内のCUnitのテストスイートをコール、スイート名、CUnitの起動モード
`PF.tblset("f_mainw_yyy","0","1","01")`  

- PFプロセス内のテーブルをリード テーブルシンボル名、開始オフセット、サイズの順で指定  
`puts PF.tblget("f_mainw_yyy,"0","1")`  

- PFプロセス内のテーブルのアドレスを取得、テーブルシンボル名  
`puts PF.tbladdr("f_mainw_yyy")`  

## MP Process modules
- MPSW内の関数をコール、関数シンボル名、引数の順の順で指定  
`MP.module("f_saa_xxx","0")`  

- MPSWプロセス内のCUnitのテストスイートをコール、スイート名、CUnitの起動モード   
`MP.cunit("test_suites_array_saa",0)`  

- MPSWプロセス内のCUnitのテストスイートをコール、スイート名、CUnitの起動モード
`MP.tblset("f_saaw_yyy","0","1","01")`  

- MPSWプロセス内のテーブルをリード テーブルシンボル名、開始オフセット、サイズの順で指定  
`puts MP.tblget("f_saaw_yyy,"0","1")`  

- MPSWプロセス内のテーブルのアドレスを取得、テーブルシンボル名  
`puts MP.tbladdr("f_saaw_yyy")`  

# Core modules example
## Utility modules
- INIT内のログ有効化  
`Core.dmsg("INIT")`

- PF内のログ有効化  
`Core.dmsg("PF")`

- ログ有効化/無効化  
`Core.dconvert("ON")`  
`Core.dconvert("OFF")`  

- 他のscriptをコール  
`Core.require("./test.rb")`    

- メモリ比較  
`Core.memcmp("src address","dst address","size") `  

- Systemコマンド実行   
`Core.system() `  


## CUnit modules
- CUnit 起動モード取得  
`Core.cmode_get() `  

- CUnit 起動モード設定  
`Core.cmode_set() `  


## SYSTEM CALL access modules
- system call復帰値の初期化  
`Core.sys_rcinit("open")`     

- system call復帰値設定   
`Core.sys_rcset("open","1")`    

- system call復帰値取得  
`Core.sys_rcget("open")`    

## OAL access modules
- OAL API復帰値の初期化   
`Core.oal_rcinit("BPF_RU_IPCM_MSGQ_SEND")`  

- OAL API復帰値の設定  
`Core.oal_rcset("BPF_RU_IPCM_MSGQ_SEND","1")`  

- OAL API復帰値の取得  
`Core.oal_rcget(""BPF_RU_IPCM_MSGQ_SEND")`  

## HAL access modules
- HAL API復帰値の初期化   
`Core.hal_rcinit("BPF_HM_DEVC_RESERVE")`

- HAL API復帰値の設定  
`Core.hal_rcset("BPF_HM_DEVC_RESERVE","1")`  

- HAL API復帰値の取得  
`Core.hal_rcget("BPF_HM_DEVC_RESERVE")`  


## RFIC access modules
- RFIC API復帰値の初期化  
`Core.rfic_rcinit("RFIC_TALISE_OPENHW")`  

- RFIC API復帰値の設定  
`Core.rfic_rcset("RFIC_TALISE_OPENHW","1")`  

- RFIC API復帰値の取得  
`Core.rifc_rcget("RFIC_TALISE_OPENHW")`  

## FPGA Register access modules
-  レジスタ領域の初期化  
`Core.reginit("00")`  

-  レジスタのリード  
`puts Core.regread("80000000") # -> 0x80000000番地のレジスタのリード `   

-  レジスタのライト  
`Core.regwrite("80000000","12345678") # -> 0x80000000番地のレジスタに値をライト `   

- 定期的( デフォルト:1msec )にレジスタに値をライト  
`Core.regworker("80000000","00000000") # -> 定期的にレジスタに値をライト 1msec `    

- 定期的( 10msec )にレジスタに値をライト  
`Core.regworker("80000000","00000000","10")  # -> 定期的にレジスタに値をライト 10msec `     

- 定期的にライトを停止  
`Core.regworkerstop() # -> 定期的にライトを停止`    


## QSPI access module

- QSPIエリア初期化( 初期値 )  
`Core.qspiinit("FF") # QSPI area init by 0xFF`  

- QSPIエリアをファイルで初期化（face,offset,size.filename）  
`Core.qspiinitfile("0","100000","1000","./log.bin") `  

- QSPIエリアライト (face,offset,value)  
`Core.qspiwrite("0","100000","12345678") `

- QSPIエリアリード(face,offset)  
`puts Core.qspiread("0","100000") `

- ###### NOTE:
  face "0"/"1" はPS側QSPI  
  face "2" はPL側QSPI  

## I2C access modules

- I2Cエリア初期化  
`Core.i2cinit("00")`  

- I2CXリード(kind,offset)  
`puts Core.i2cXread("00","01")`  

- I2CXライト(kind,offset,value)  
`Core.i2cXwrite("00","01","02")`  

- ###### NOTE:
   i2cXの部分は以下   
   i2c0 "00":EEP,"01":TEMP   
   i2c1 "00":sfp0 a0 / "01":sfp0 a2  / "02":sfp1 a0 / "03":sfp1 a2  
   i2c2 "00":pgc_v,"01":pgc_h ,"02":rfeep   


## SPI access modules

- SPIエリア初期化  
`Core.spiinit("00")`  

- SPIXリード  
`puts Core.spiXread("00","00010000","0")`  

- SPIXライト  
`Core.spiXwrite("00","00010000","0","1234")`  

- SPIX定期ライト  
`Core.spiXworker("00","00010000","00000000","10")`  

- SPI定期ライト停止  
`Core.spiworkerstop()`  

- ###### NOTE:
   spiXの部分は以下  
   spi0 "00":ptppll,"01":rficpll,"02":rficpll_r  
   spi1 "00":jc-pll,"01":rfic  
   spi2 "00":tx-dac,"01":rx-adc  
   spi3 "00":qmod_txmix,"01":qmod_txvga,"02":qdem,"03":qdem_dac  
     "04":pavg,"05":txvatt,"06":pavg2,"07":txvatt_dac,"08":rxvatt_adc


## Sleep modules
- Sleepする  
`Core.sleep("1") # -> 1sec sleep`  
`Core.sleep("1","100000") # -> 1sec .0x100000nsec sleep`  

## Shared memory modules
- 共有メモリに値を設定する  
`Core.shmset("0","0","4","12345678")`  
   -> Shared memory write E_RRH_SHMID_APL_RST_REASON_LOG(0) offset 0 0x12345678

- 共有メモリの値を取得する  
`puts Core.shmget("0","0","4")`  
  -> Shared memory read  E_RRH_SHMID_APL_RST_REASON_LOG(0) offset 0 4byte  

- 共有メモリのアドレスを取得する  
`puts Core.shmaddr("0")`  
  -> Shared memory adder get E_RRH_SHMID_APL_RST_REASON_LOG(0)`

## Symbol module
- シンボルを生成する  
`puts Core.symalloc("msg","1000")`  
  -> allocate symbol name:msg size:0x1000 byte  

- シンボルに値を設定する  
`Core.symset("msg","0","4","12345678")`   
  -> write symbol name:msg offset:0 size:4byte value:0x12345678  

- シンボルの値を取得する  
`puts Core.symget("msg","0","4")`  
  -> read  symbol name:msg offset:0 size:4byte  

- シンボルのアドレスを取得する  
`puts Core.symaddr("msg")`  
  -> get symbol address

- シンボルを解放する  
`Core.symfree("msg")`  
 -> remove symbol

- すべてのシンボルを解放する  
`Core.symallfree()`  
 -> remove all symbol

## Value change modules
- hexをdecに変換する  
`puts Core.v_to_v("h","d","0010")                        # -> 16`  

- hexをbitに変換する  
`puts Core.v_to_v("h","b","0010")                        # -> 000000000010000`  

- decをhexに変換する  
`puts Core.v_to_v("d","h","0010")                        # -> 0xa`  

- decをbitに変換する  
`puts Core.v_to_v("d","b","0010")                        # -> 1010`  

- bitをdecに変換する  
`puts Core.v_to_v("b","d","0010")                        # -> 2`  

- bitをhexに変換する  
`puts Core.v_to_v("b","h","0010")                        # -> 0x2`  

- 値をORする  
`puts Core.calc("1001","|","0110")                       # -> 00001111`  

- 値をANDする  
`puts Core.calc("1001","&","0110")                       # -> 00000000`  

- 値を加算する  
`puts Core.calc("1001","+","0110")                       # -> 00001111`  

- 値を減算する  
`puts Core.calc("2","-","1")                             # -> 1`  

- 値を掛け算する   
`puts Core.calc("2","*","2")                             # -> 4`  

- 値を/する  
`puts Core.calc("2","/","2")                             # -> 1`  

- 値を%する  
`puts Core.calc("2","%","2")                             # -> 0`  

- 値を右シフトする  
`puts Core.calc("2",">","1")                             # -> 1`  

- 値を左シフトする  
`puts Core.calc("1","<","1")                             # -> 2`  
