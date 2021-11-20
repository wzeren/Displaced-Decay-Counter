PYTHIA8=/home/wzeren/Desktop/Research/HEP_Softwares/pythia8243
PYTHIALIB=$(PYTHIA8)/lib
PYTHIAINC=$(PYTHIA8)/include
PYTHIAINCPYTHIA8=$(PYTHIA8)/include/Pythia8
INCLUDE=-I$(PYTHIAINC) -I./ -I$(PYTHIAINCPYTHIA8)
LDLIBS=-Wl,--no-as-needed -ldl

CC=g++
CFLAGS=-std=c++11
LDFLAGS= -lpythia8 -L$(PYTHIALIB) -lz -L$(LDLIBS) -Wl,-rpath,

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
	$(CC) -o $(BINDIR)/$@ $^ $(LDFLAGS)

$(LIBDIR)/%.o: $(SRCDIR)/%.cc
	@[ ! -d $(dir $@) ] & mkdir -p $(dir $@)
	$(CC) -o $@ -c $^ $(INCLUDE) $(CFLAGS)

.PHONY = clean 
