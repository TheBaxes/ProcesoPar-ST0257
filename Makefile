CXXFLAGS=-I./include -std=c++11 -fPIC -Wall -Wextra -pedantic 
LDFLAGS=-pthread -shared

VPATH = src
OUT = lib/libProcesoPar.so
SRC = fuente01.cpp fuente02.cpp fuente03.cpp fuente04.cpp
OBJ = $(SRC:.cpp=.o)

all: $(OUT)

lib:
	mkdir lib

$(OUT): $(OBJ) lib
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(OUT)
