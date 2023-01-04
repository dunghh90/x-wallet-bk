#
# rrh_pf プロセス用テストシナリオ
#
Core.require("./verbose")
mode = Core.cmode_get()


Core.i2c0write("00","40","13")
Core.i2c0write("00","41","11")

PF.module("f_main_iniTbl")

puts "PF CCA Threads functions test Start"

PF.module("f_dpda_init_taskTable")
PF.module("f_dpda_com_setDpdFailSafe","0")
PF.module("f_dpda_com_setDpdFailSafe","1")
PF.module("f_dpda_com_setDpdFailSafe","2")
PF.module("f_dpda_com_setDpdFailSafe","3")


puts "PF CCA Threads functions test END"


