#### libHFILL makefile
SOURCEDIR=./src
INCLUDEDIR=../inc
OBJECTDIR=obj
VPATH=$(SOURCEDIR)
vpath %.h ../inc
vpath %.cc

CC=g++
CF=gfortran
CFLAGS=-Wall -Wl,-rpath,../
LDFLAGS=-c -Wall

all: release

release: test test_fort
uninstall: clean

clean:
	rm -rf test test_fort
	rm -rf test.o*

lib_src = test.o

test: $(lib_src)
	$(CC) $(CFLAGS) -I $(INCLUDEDIR) $^ -L../ -lHFILL \
	-o $@
	@echo "==> $@ done!"

test_fort: test.o2
	$(CF) $(CFLAGS) -I $(INCLUDEDIR) $^ -L../ -lHFILL \
	-o $@
	@echo "==> $@ done!"


%.o: %.cc
	$(CC) $(LDFLAGS) -I $(INCLUDEDIR) $< -o $@

%.o2: %.f
	$(CF) $(LDFLAGS) $< -o $@
