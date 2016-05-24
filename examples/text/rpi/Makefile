CFLAGS+=-O3 -march=armv6 -mfpu=vfp -mfloat-abi=hard -Wall
LDFLAGS+=-lsigil -lGLESv2
INCLUDES+=-I../../../include
LIBS+=-L/opt/vc/lib
SRC=../text.cpp
BIN=text

all: $(SRC)
	$(CXX) -o $(BIN) $(SRC) $(CFLAGS) $(INCLUDES) $(LIBS) $(LDFLAGS)

clean:
	-rm $(BIN)
