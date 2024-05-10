#********************************************************************************
#
#	Copyright (c) 2010 Magic Control Technology Co.  All Rights Reserved.
#
#	FILE:
#		Makefile
#
#	HISTORY:
#		2010/04/07		Louis Tsai Created
#*********************************************************************************
CC=gcc
all:
#	$(CC) -o T6evdi main.c t6usbdongle.c  simclist.c -levdi -lusb-1.0 -lturbojpeg -lc -lm -lpthread -lX11 -lXrandr -lXinerama	
	$(CC) -o T6evdi main.c t6usbdongle.c  simclist.c -levdi -lusb-1.0 -lturbojpeg -lc -lm -lpthread
	$(CC) -o T6contrl T6contrl.c t6usbdongle.c -lusb-1.0 -lc -lm
clean:
	rm -f T6evdi  T6contrl *.o *.bak

