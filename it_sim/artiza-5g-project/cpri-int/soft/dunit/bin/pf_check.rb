#
# rrh_pf �v���Z�X�p�e�X�g�V�i���I
#
Core.require("./verbose")
mode = Core.cmode_get()

puts "Table check Start"

rtn = PF.cunit("test_suites_array_ft_check",mode)
if ( rtn == false ) then
 puts "ERROR"
end

puts "Table check End"


