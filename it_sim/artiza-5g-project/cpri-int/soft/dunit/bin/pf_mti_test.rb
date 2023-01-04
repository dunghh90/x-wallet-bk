#
# rrh_pf プロセス用テストシナリオ
#
Core.require("./verbose")
mode = Core.cmode_get()

puts "PF MTI Threads functions test Start"

Core.i2c0write("00","40","13")
Core.i2c0write("00","41","10")

puts "Init table"

PF.module("f_main_rrhKindGet")
PF.module("f_main_iniTbl")

rtn = PF.module("f_com_mtCtl_init")
rtn = PF.module("f_com_mtCtl_con","1")
rrh = PF.module("f_com_mtCtl_carInfo","0","0","1","38000","100","0")
rrh = PF.module("f_com_mtCtl_carInfo","0","0","1","39000","100","1")

Core.symalloc("sfp_vendor","10")
Core.symset_s("sfp_vendor","0","10","FujitsuFJT123456")
Core.symalloc("sfp_serial","10")
Core.symset_s("sfp_serial","0","10","0123456789ABCDEF")

rrh = PF.module("f_com_mtCtl_sfpInfo",0xE,Core.symaddr("sfp_vendor"),Core.symaddr("sfp_serial"))

Core.sys_rcset("ioctl","-1")

puts "f_mti_main test"

rtn = PF.module("f_mti_main")

Core.symalloc("msg","4000")
Core.symset("msg","0","1","0")
Core.symset("msg","1","1","0")
Core.symset("msg","2","1","30")
Core.symset("msg","3","1","01")
Core.symset("msg","4","4","01234567")
Core.symset("msg","8","4","89ABCDEF")
Core.symset("msg","C","4","76543210")
Core.symset("msg","10","4","0")
Core.symset("msg","14","4","0")
Core.symset("msg","18","4","0")
Core.symset("msg","1C","4","0")
Core.symset("msg","20","4","0")
Core.symset("msg","23","1","1")
Core.symset("msg","24","4","0")
Core.symset("msg","27","1","2")
Core.symset("msg","28","4","0")
Core.symset("msg","2B","1","3")
Core.symset("msg","2C","4","0")
Core.symset("msg","2F","1","4")
Core.sys_rcset("recv",Core.symaddr("msg"))
Core.sys_rcset("accept","0")
Core.sys_rcset("acceptCnt","1")
Core.sys_rcset("recvCnt","1")
Core.sys_rcset("recvLen","30")

puts "f_mti_loop test 3001 OPT_CONNECT_STT_REQ"
rtn = PF.module("f_mti_loop")

Core.symset("msg","2","1","30")
Core.symset("msg","3","1","04")
Core.symset("msg","20","4","abcdef12")
Core.sys_rcset("recv",Core.symaddr("msg"))
Core.sys_rcset("accept","0")
Core.sys_rcset("acceptCnt","1")
Core.sys_rcset("recvCnt","1")
Core.sys_rcset("recvLen","24")

puts "f_mti_loop test 3004 OPT_CONNECT_STP_REQ"
rtn = PF.module("f_mti_loop")

Core.symset("msg","2","1","30")
Core.symset("msg","3","1","11")
Core.symset("msg","20","4","04030201")
Core.symset("msg","24","4","08070605")
Core.sys_rcset("recv",Core.symaddr("msg"))
Core.sys_rcset("accept","0")
Core.sys_rcset("acceptCnt","1")
Core.sys_rcset("recvCnt","1")
Core.sys_rcset("recvLen","28")

puts "f_mti_loop test 3011 EEPROM_REG_READ_REQ"
rtn = PF.module("f_mti_loop")


Core.symset("msg","2","1","30")
Core.symset("msg","3","1","21")
Core.symset("msg","20","4","04030201")
Core.symset("msg","24","4","08070605")
Core.sys_rcset("recv",Core.symaddr("msg"))
Core.sys_rcset("accept","0")
Core.sys_rcset("acceptCnt","1")
Core.sys_rcset("recvCnt","1")
Core.sys_rcset("recvLen","28")

puts "f_mti_loop test 3021 DUSTATUS_GET_REQ"
rtn = PF.module("f_mti_loop")


Core.symset("msg","2","1","30")
Core.symset("msg","3","1","31")
Core.symset("msg","20","4","04030201")
Core.symset("msg","24","4","08070605")
Core.sys_rcset("recv",Core.symaddr("msg"))
Core.sys_rcset("accept","0")
Core.sys_rcset("acceptCnt","1")
Core.sys_rcset("recvCnt","1")
Core.sys_rcset("recvLen","28")

puts "f_mti_loop test 3031 FORCEALM_REQ"
rtn = PF.module("f_mti_loop")

Core.symset("msg","2","1","30")
Core.symset("msg","3","1","41")
Core.symset("msg","20","4","04030201")
Core.symset("msg","24","4","08070605")
Core.symset("msg","28","4","01000000")
Core.sys_rcset("recv",Core.symaddr("msg"))
Core.sys_rcset("accept","0")
Core.sys_rcset("acceptCnt","1")
Core.sys_rcset("recvCnt","1")
Core.sys_rcset("recvLen","2C")

puts "f_mti_loop test 3041 CARDBLK_REQ"
rtn = PF.module("f_mti_loop")

Core.symset("msg","2","1","30")
Core.symset("msg","3","1","51")
Core.symset("msg","20","4","04030201")
Core.symset("msg","24","4","08070605")
Core.symset("msg","28","4","01000000")
Core.sys_rcset("recv",Core.symaddr("msg"))
Core.sys_rcset("accept","0")
Core.sys_rcset("acceptCnt","1")
Core.sys_rcset("recvCnt","1")
Core.sys_rcset("recvLen","2C")

puts "f_mti_loop test 3051 CARDRST_REQ"
rtn = PF.module("f_mti_loop")

Core.symset("msg","2","1","30")
Core.symset("msg","3","1","61")
Core.symset("msg","20","4","04030201")
Core.symset("msg","24","4","08070605")
Core.symset("msg","28","4","FFFFFFFF")
Core.symset("msg","2C","4","00000000")
Core.sys_rcset("recv",Core.symaddr("msg"))
Core.sys_rcset("accept","0")
Core.sys_rcset("acceptCnt","1")
Core.sys_rcset("recvCnt","1")
Core.sys_rcset("recvLen","30")

puts "f_mti_loop test 3061 PAONOFF_REQ"
rtn = PF.module("f_mti_loop")

Core.symset("msg","2","1","30")
Core.symset("msg","3","1","71")
Core.symset("msg","20","4","04030201")
Core.symset("msg","24","4","08070605")
Core.sys_rcset("recv",Core.symaddr("msg"))
Core.sys_rcset("accept","0")
Core.sys_rcset("acceptCnt","1")
Core.sys_rcset("recvCnt","1")
Core.sys_rcset("recvLen","28")

puts "f_mti_loop test 3071 NETCONF_AUTH_RESET_REQ"
rtn = PF.module("f_mti_loop")


Core.symset("msg","2","1","30")
Core.symset("msg","3","1","81")
Core.symset("msg","20","4","04030201")
Core.symset("msg","24","4","08070605")
Core.symset("msg","28","4","0A000000")
Core.sys_rcset("recv",Core.symaddr("msg"))
Core.sys_rcset("accept","0")
Core.sys_rcset("acceptCnt","1")
Core.sys_rcset("recvCnt","1")
Core.sys_rcset("recvLen","2C")

puts "f_mti_loop test 3081 POWADJ_WRITE_REQ"
rtn = PF.module("f_mti_loop")

Core.symset("msg","2","1","30")
Core.symset("msg","3","1","91")
Core.symset("msg","20","4","04030201")
Core.symset("msg","24","4","08070605")
Core.symset("msg","28","4","01000000")
Core.sys_rcset("recv",Core.symaddr("msg"))
Core.sys_rcset("accept","0")
Core.sys_rcset("acceptCnt","1")
Core.sys_rcset("recvCnt","1")
Core.sys_rcset("recvLen","2C")

puts "f_mti_loop test 3091 BEAMID_SET_REQ"
rtn = PF.module("f_mti_loop")

PF.cunit("test_suites_array_ft_mti",mode)

