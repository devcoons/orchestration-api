# Project Name
PRJNAME=throughput
VERSION := 0.3

# Filepaths
SRCDIR=src
INCDIR=inc
LIBDIR=lib
BINDIR=../Executable
INSTALL=/usr/bin

# Toolchain
CC=gcc
CXX=g++

DIRS := . $(shell find $(SRCDIR) -type d)
GARBAGE_PATTERNS := *.o
GARBAGE := $(foreach DIR,$(DIRS),$(addprefix $(DIR)/,$(GARBAGE_PATTERNS)))

# Find source files
C_FILES := $(shell find -L $(SRCDIR) -name '*.cpp')
O_FILES := $(C_FILES:.cpp=.o)

# Find header dirs
INC=$(shell find -L $(INCDIR) -name '*.h' -exec dirname {} \; | uniq)
INC+=$(shell find -L $(LIBDIR) -name '*.h' -exec dirname {} \; | uniq)
INCLUDES=$(INC:%=-I%)

# Compiler Flags
PFLAGS=-fPIC -O3 -g -pthread
LDFLAGS:=$(LDFLAGS)
CFLAGS=-std=c++11 -shared -Wl,-soname,lib$(PRJNAME).so
EFLAGS=-fno-inline  -O0
LFLAGS=-lrt -lm -lpthread

# Compiling Process
all: lib$(PRJNAME).so.$(VERSION) clean

lib$(PRJNAME).so.$(VERSION): $(O_FILES)
	$(CXX) $(PFLAGS) $(LDFLAGS) $(INCLUDES) $(CFLAGS) -o $(BINDIR)/$@ $^ $(EFLAGS) $(LFLAGS)

%.o: %.cpp
	$(CXX) $(PFLAGS) $(CFLAGS) -c $(INCLUDES) -o $@ $< $(LFLAGS)

clean:
	$(shell find -L $(SRCDIR) -name '*.o' -type f -delete)

cleanall:
	$(shell find -L $(SRCDIR) -name '*.o' -type f -delete)
	$(RM) $(BINDIR)/*

install:
	$(shell mkdir /usr/include/throughput)
	$(shell cp $(BINDIR)/lib$(PRJNAME).so.$(VERSION) /usr/lib/)
	$(shell find -L $(INCDIR) -name '*.h' -exec cp {} '/usr/include/throughput/' ';')
	$(shell ln -s /usr/lib/lib$(PRJNAME).so.$(VERSION) /usr/lib/lib$(PRJNAME).so)
	$(shell ldconfig)
	@echo "Installation Completed";
uninstall:
	$(shell rm /usr/lib/lib$(PRJNAME).so)
	$(shell rm /usr/lib/lib$(PRJNAME).so.*.*)
	$(shell rm -r  /usr/include/throughput/)
	$(shell ldconfig)
	@echo "Uninstallation Completed";
