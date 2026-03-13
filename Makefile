main:
	g++ main.cpp ./src/external/rlImGui.cpp -o main.out -lraylib -limgui
	./main.out
