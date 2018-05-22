
CC=gcc

CFLAGS=-O3 -Wall -g -pg

LDFLAGS=-L./

all : libpixelvm.a test-emptyimg test-sizeofcolor test-circleimg gen-emptyimg gen-circleimg emptyimg.dat circleimg.dat pixelvmr emptyimg_gen.rgb circleimg_gen.rgb

libpixelvm.a : pixelvm.o
	ar r $@ $^

LIBS = -lpixelvm -lm

test-emptyimg : test-emptyimg.o writefile.o libpixelvm.a
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

test-sizeofcolor : test-sizeofcolor.o libpixelvm.a
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

test-circleimg : LIBS+=-lm

test-circleimg : test-circleimg.o writefile.o libpixelvm.a
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

emptyimg.dat : gen-emptyimg
	@./$^ > $@

circleimg.dat : gen-circleimg
	@./$^ > $@

gen-emptyimg : gen-emptyimg.o writefile.o libpixelvm.a
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

gen-circleimg : gen-circleimg.o writefile.o libpixelvm.a
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

pixelvmr : LIBS+=-lm

pixelvmr : pixelvmr.o writefile.o libpixelvm.a
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

emptyimg_gen.rgb : pixelvmr emptyimg.dat
	@./pixelvmr emptyimg.dat > $@

circleimg_gen.rgb : pixelvmr circleimg.dat
	@./pixelvmr circleimg.dat > $@

clean :
	-rm *.o *.rgb *.dat libpixelvm.a test-emptyimg test-sizeofcolor test-circleimg gen-emptyimg pixelvmr gen-circleimg
