IDIR =../include
ODIR=../obj
LDIR=../lib
BDIR=../bin

CC=g++ -std=c++11 -O3
CFLAGS=-I$(IDIR) `pkg-config --cflags opencv libfreenect`
LIBS=`pkg-config --libs opencv libfreenect` -lboost_system -lfreenect -lpthread

_DEPS = open_kinect.h mutex.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = camera.o open_kinect.o mutex.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(IDIR)/%.cpp $(DEPS)
	$(CC) -c -o$@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o$@ $< $(CFLAGS)

$(BDIR)/camera: $(OBJ)
	$(CC) -o$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/camera.o
	rm -f $(ODIR)/open_kinect.o
	rm -f $(ODIR)/mutex.o
	rm -f $(BDIR)/camera

