main:
	gcc main.c -lraylib
	./a.out

ImGui:
	g++ imgui.cpp ./src/external/rlImGui.cpp -lraylib -limgui
	./a.out

Door:
	g++ door.cpp -lraylib
	./a.out

Triangle:
	g++ triangle.cpp -lraylib
	./a.out
