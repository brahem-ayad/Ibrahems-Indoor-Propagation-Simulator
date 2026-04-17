main:
	g++ main.cpp -lraylib
	./a.out

windows:
	x86_64-w64-mingw32-g++ main.cpp -o game.exe \
	-I/usr/x86_64-w64-mingw32/include \
	/usr/x86_64-w64-mingw32/lib/libraylib.dll.a \
	-lopengl32 -lgdi32 -lwinmm -lpthread \
	-static -static-libgcc -static-libstdc++
	cp /usr/x86_64-w64-mingw32/bin/libraylib.dll .
	cp /usr/x86_64-w64-mingw32/bin/libssp-0.dll .

print_nl:
	ls main.cpp include/*.h | grep -v "portable-file-dialogs.h" | xargs wc -l
