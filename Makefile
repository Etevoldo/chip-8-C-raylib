LINKER_FLAGS := -lopengl32 -lraylib -lgdi32 -lwinmm 
INCLUDE_PATH := "include"
LIBRARY_PATH := "lib"
BUILD_DIR := build
DEBUG := -g
FLAGS := -Wall
OBJ := main

${OBJ}.exe: ${BUILD_DIR}/${OBJ}.o ${BUILD_DIR}/stack.o 
	gcc -o $@ ${BUILD_DIR}/${OBJ}.o ${FLAGS} ${DEBUG} -L${LIBRARY_PATH} ${LINKER_FLAGS}

${BUILD_DIR}/${OBJ}.o: ${OBJ}.c stack.c
	gcc ${OBJ}.c -c ${DEBUG} ${FLAGS} -I${INCLUDE_PATH} -o $@

${BUILD_DIR}/stack.o: stack.c
	gcc stack.c -c ${DEBUG} ${FLAGS} -I${INCLUDE_PATH} -o $@