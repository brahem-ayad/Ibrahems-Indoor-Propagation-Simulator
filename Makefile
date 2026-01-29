SRC := $(wildcard *.cpp)

main:
	g++ $(SRC) -lraylib -o Program
	./Program
