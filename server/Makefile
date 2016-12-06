IDIR =../include
ODIR=../obj
LDIR=../lib
BDIR=../bin

CC=g++ -std=c++11 -O3
CFLAGS=-I$(IDIR) `pkg-config --cflags opencv`
LIBS=`pkg-config --libs opencv` -lboost_system -lboost_thread

_OBJ = server.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o$@ $< $(CFLAGS)

$(BDIR)/server: $(OBJ)
	$(CC) -o$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/server.o
	rm -f $(BDIR)/server

