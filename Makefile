LINKER_FLAGS := -lopengl32 -lraylib -lgdi32 -lwinmm 
INCLUDE_PATH := "include"
LIBRARY_PATH := "lib"
BUILD_DIR := build

main.exe: ${BUILD_DIR}/main.o
	gcc ${BUILD_DIR}/main.o -o game.exe -L${LIBRARY_PATH} ${LINKER_FLAGS}

${BUILD_DIR}/main.o: main.c
	gcc main.c -c -I${INCLUDE_PATH} -o $@