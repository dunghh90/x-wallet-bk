#------------------------------------------------------
# Makefile for devc library
#------------------------------------------------------


all: alllib 
clean: allclean

alllib:
	cd ./src;make -f Makefile_recv
	cd ./src;make -f Makefile_send
	cd ./src;make -f Makefile_mpproxy

allclean:
	cd ./src;make -f Makefile_recv clean
	cd ./src;make -f Makefile_send clean
	cd ./src;make -f Makefile_mpproxy clean

