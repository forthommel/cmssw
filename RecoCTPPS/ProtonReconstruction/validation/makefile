all: do_fits

do_fits : do_fits.cc
	g++ `root-config --libs` -lMinuit `root-config --cflags` --std=c++11 -Wall -Werror\
		do_fits.cc -o do_fits
