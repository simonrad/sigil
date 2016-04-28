
CFLAGS+=-O3 -march=armv6 -mfpu=vfp -mfloat-abi=hard -Wall -Wextra
LDFLAGS+=-lpigu -lGLESv2 -lEGL -lbcm_host -lrt
INCLUDES+=-I../include -I/opt/vc/include
LIBS+=-L../lib -L/opt/vc/lib
SRC=cube.cpp
OBJ=${SRC:.cpp=.bin}

$(OBJ): %.bin: %.cpp
	$(CXX) $< -o $@ $(CFLAGS) $(INCLUDES) $(LIBS) $(LDFLAGS)

clean:
	-rm *.bin
