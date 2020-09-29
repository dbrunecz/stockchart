
CFLAGS+=-Wall -Werror
CFLAGS+=-ggdb
#CFLAGS+=-O3

analyze2: LDFLAGS+= -L/usr/X11R6/lib
analyze2: LDLIBS+= -lX11 -lm
analyze2: analyze2.o loadfile.o dbx.o strategy.o histogram.o badsymlist.o moving-average.o

clean:
	-rm -f *.o analyze2
