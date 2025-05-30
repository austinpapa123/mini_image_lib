CXX = g++
CXXFLAGS = -std=c++11 -Wall
SRC = src/image.cpp main.cpp
OBJ = $(SRC:.cpp=.o)
INC = -Iinclude

all: app

app: $(SRC)
	$(CXX) $(CXXFLAGS) $(INC) -o app $(SRC)

clean:
	rm -f app *.o
