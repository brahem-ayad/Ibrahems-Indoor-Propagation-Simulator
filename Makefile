main:
	g++ main.cpp -lraylib
	./a.out

button:
	g++ button.cpp -lraylib
	./a.out

print_nl:
	ls main.cpp include/*.h | grep -v "portable-file-dialogs.h" | xargs wc -l
