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

#JSON DIRECTORIES
CURRDIR := $(shell pwd)
JSON=$(CURRDIR)/thirdParty/json-develop
JSONINC=$(JSON)/single_include

INCLUDE=-I$(PYTHIAINC) -I./ -I$(PYTHIAINCPYTHIA8) -I./ -I$(HEPMCINC) -I$(JSONINC)
LDLIBS=-Wl,--no-as-needed -ldl

CC=g++
CFLAGS=-std=c++17 -Wno-deprecated-declarations
#LDFLAGS= -lpythia8 -L$(PYTHIALIB) -lz -L$(LDLIBS) -Wl,-rpath,
#LDFLAGS= -lpythia8 -L$(PYTHIALIB) -L$(LDLIBS) -Wl,-rpath,
LDFLAGS=  -L$(PYTHIALIB) -L$(HEPMCLIB) -Wl,-R$(HEPMCLIB) -L$(LDLIBS)  -lpythia8 -ldl -lz  -lHepMC  #-rpath,

SRCDIR=src
HEADDIR=include
LIBDIR=build
BINDIR=bin

BIN=main
SRC=$(shell find . -name '*.cc')
TMP=$(subst $(SRCDIR),$(LIBDIR), $(SRC))
OBJ=$(patsubst %.cc,%.o,$(TMP))

all: $(BIN)

main: $(OBJ)
	@[ ! -d $(BINDIR) ] & mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/$@ $^ $(LDFLAGS) $(CFLAGS)

$(LIBDIR)/%.o: $(SRCDIR)/%.cc
	@[ ! -d $(dir $@) ] & mkdir -p $(dir $@)
	$(CC) -o $@ -c $^ $(INCLUDE) $(CFLAGS)

DetEditor: $(SRCDIR)/DetEditor.cxx
	$(CC) -o $(SRCDIR)/DetEditor -std=c++17 $(SRCDIR)/DetEditor.cxx -lstdc++fs
.PHONY = clean

clean:
	rm -rf build bin/main
