#Home Directory
HOME = /home/jsk


#Pythia Directories
PYTHIA8=$(HOME)/pythia8245
PYTHIALIB=$(PYTHIA8)/lib
PYTHIAINC=$(PYTHIA8)/include
PYTHIAINCPYTHIA8=$(PYTHIA8)/include/Pythia8

#HepMC Directories
HEPMC=$(HOME)/hepmc
HEPMCLIB=$(HEPMC)/lib
HEPMCINC=$(HEPMC)/include

#JSON DIRECTORIES (json.hpp is in include/)
CURRDIR := $(shell pwd)

INCLUDE=-I$(PYTHIAINC) -I./ -I$(PYTHIAINCPYTHIA8) -I./ -I$(HEPMCINC) -I$(HEADDIR)
LDLIBS=-Wl,--no-as-needed -ldl

CC=g++
CFLAGS=-std=c++17 -Wno-deprecated-declarations -O2
#LDFLAGS= -lpythia8 -L$(PYTHIALIB) -lz -L$(LDLIBS) -Wl,-rpath,
#LDFLAGS= -lpythia8 -L$(PYTHIALIB) -L$(LDLIBS) -Wl,-rpath,
#LDFLAGS=  -L$(PYTHIALIB) -L$(HEPMCLIB) -Wl,-R$(HEPMCLIB) -L$(LDLIBS)  -lpythia8 -ldl -lz  -lHepMC  #-rpath,
LDFLAGS=  -L$(PYTHIALIB) -L$(HEPMCLIB) -Wl,-R$(HEPMCLIB) -L$(LDLIBS)  -lpythia8 -lz -Wl,-rpath $(PYTHIALIB)   -lHepMC  #-rpath,

SRCDIR=src
HEADDIR=include
LIBDIR=build
BINDIR=bin

BIN=$(BINDIR)/main
SRC=$(shell find . -name '*.cc')
TMP=$(subst $(SRCDIR),$(LIBDIR), $(SRC))
OBJ=$(patsubst %.cc,%.o,$(TMP))

.PHONY: all main clean DetEditor

all: main DetEditor

main: $(BIN)

$(BIN): $(OBJ)
	@[ ! -d $(BINDIR) ] & mkdir -p $(BINDIR)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

$(LIBDIR)/%.o: $(SRCDIR)/%.cc
	@[ ! -d $(dir $@) ] & mkdir -p $(dir $@)
	$(CC) -o $@ -c $^ $(INCLUDE) $(CFLAGS)

DetEditor: $(BINDIR)/DetEditor

$(BINDIR)/DetEditor: $(SRCDIR)/DetEditor.cxx
	$(CC) -o $@ -std=c++17 $< -lstdc++fs

clean:
	rm -rf build bin/main bin/DetEditor
