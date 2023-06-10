all: compile link run

run:
	main.exe

compile:
	g++ -Isrc/include -c main.cpp -DSFML_STATIC

link:
	g++ main.o -o main -L"src/lib" -lsfml-graphics -lsfml-window -lsfml-system