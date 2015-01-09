#### libHFILL makefile
SOURCEDIR=./src
INCLUDEDIR=inc -I $(shell root-config --cflags)
LIBS=$(shell root-config --libs)
OBJECTDIR=obj
VPATH=$(SOURCEDIR)
vpath %.h
vpath %.cc

CC=g++
CFLAGS=-Wall -O3 -shared -fPIC
LDFLAGS=-c -Wall -O3 -fPIC

all: release

release: libHFILL.so
uninstall: clean

clean:
	rm -rf libHFILL.so
	rm -rf $(OBJECTDIR)/*.o

lib_obj = hfill.o md5.o setup.o
lib_inc = hfill.h md5.h
lib_src = $(addprefix $(OBJECTDIR)/, $(lib_obj))
lib_hea = $(addprefix $(INCLUDEDIR)/, $(lib_inc))

libHFILL.so: $(lib_src)	
	$(CC) $(CFLAGS) -I $(INCLUDEDIR) $^ $(LIBS) \
	-o $@
	@echo "==> $@ done!"

$(OBJECTDIR)/%.o: %.cc
	$(CC) $(LDFLAGS) -I $(INCLUDEDIR) $< -o $@
