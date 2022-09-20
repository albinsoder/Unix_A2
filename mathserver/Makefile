IDIR =/include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=/objects
LDIR =../lib

LIBS=-lm

_DEPS = ConnectServer.h ConnectClient.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = UDPclient.o UDPserver.o ConnectClient.o ConnectServer.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 