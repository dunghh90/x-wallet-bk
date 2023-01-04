all: alllib
clean: allclean

alllib:
	#ft_com
	cd ../../../pf/ft_com/com;make
	#ft_cpr(debug)
	cd ../../pf_fhm/ft_cpr;make
	#ft_eth
	cd ../ft_eth/eth/Makefile;make
	#ft_fmg
	cd ../ft_fmg/fmg/makefile;make
	#ft_frw
	cd ../ft_frw/frw/makefile;make
	#ft_frwsla
	cd ../ft_frw/frwsla/makefile;make
	#ft_frwre
	cd ../ft_frw/frwre/makefile;make
	#ft_hdl
	cd ../ft_hdl/hdl/makefile;make
	#ft_log
	cd ../ft_log/log/makefile;make
	#ft_rfp
	cd ../../../pf/ft_rfp/dpd/makefile;make
	#ft_rfp(debug)
	cd ../../../pf/ft_rfp/dpd/debug;make
	#ft_rsv(with debug)
	cd ../ft_rsv;make
	#ft_wdt
	cd ../ft_wdt/wdt/Makefile;make
	#ft_main
	cd ../ft_main/main/Makefile;make ; mv ./rrh_pf_fhm ../../../z_makefile

allclean:

	#ft_com
	cd ../../../pf/ft_com/com;make clean
	#ft_cpr
	cd ../../pf_fhm/ft_cpr/cpr/makefile;make clean
	cd ../ft_cpr/nego/makefile;make clean
	cd ../ft_cpr/debug;make clean
	#ft_eth
	cd ../ft_eth/eth/Makefile;make clean
	#ft_fmg
	cd ../ft_fmg/fmg/makefile;make clean
	#ft_frw
	cd ../ft_frw/frw/makefile;make clean
	#ft_frwsla
	cd ../ft_frw/frwsla/makefile;make clean
	#ft_frwre
	cd ../ft_frw/frwre/makefile;make clean
	#ft_hdl
	cd ../ft_hdl/hdl/makefile;make clean
	#ft_log
	cd ../ft_log/log/makefile;make clean
	#ft_rfp
	cd ../../../pf/ft_rfp/dpd/makefile;make clean
	#ft_rfp(debug)
	cd ../../../pf/ft_rfp/dpd/debug;make clean
	#ft_rsv
	cd ../ft_rsv/rsv/makefile;make clean
	cd ../ft_rsv/debug;make clean
	#ft_wdt
	cd ../ft_wdt/wdt/Makefile;make clean
	#ft_main
	cd ../ft_main/main/Makefile;make clean
	rm -rf rrh_pf

