
# CFLAGS+=-O3 -march=armv6 -mfpu=vfp -mfloat-abi=hard -Wall -Wextra
LDFLAGS+=-lsigil -lGLESv2
INCLUDES+=-I../../../include
LIBS+=-L/opt/vc/lib
SRC=../text.cpp
OBJ=${SRC:.cpp=.bin}

$(OBJ): %.bin: %.cpp
	$(CXX) $< -o $@ $(CFLAGS) $(INCLUDES) $(LIBS) $(LDFLAGS)

clean:
	-rm *.bin
