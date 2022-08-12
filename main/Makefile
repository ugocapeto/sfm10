include main_objects.h
#
PROGRAM = sfm10
#
CC = g++
CFLAGS = -g -Wall\
-I.. -I../../common
#
all: $(PROGRAM)
#
$(PROGRAM): $(main_objects) ../sfm10.o
	$(CC) -o $@ $(main_objects)\
        ../sfm10.o\
        ../../er9b/er9b.o\
        ../../common/io/io.o\
        ../../common/jpeg/jpeg.o\
        ../../common/math/math.o\
        ../../common/png/png.o\
        ../../common/tiff/tiff.o\
        ../../common/util/util.o\
        ../liblbfgs/lib/liblbfgs.a\
        ../pba/bin/libpba_no_gpu.a\
        -pthread\
        -ltiff\
        -lpng\
        -ljpeg\
        -lz\
        -lm
#
.cpp.o:
	$(CC) -c $(CFLAGS) $<
