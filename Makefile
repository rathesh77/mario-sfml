COMPILER = g++
LIB = SFML/build/lib/
INCLUDE = SFML/include/
MODULES = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-main
EXECUTABLE = main
SOURCES = mario.cpp game.cpp main.cpp

clean: main.exe
	./clean.sh ${EXECUTABLE}.exe

compile:
	g++ -L${LIB} -I${INCLUDE} -I./include ${SOURCES} ${MODULES} -o ${EXECUTABLE}

run: main.exe
	./${EXECUTABLE}