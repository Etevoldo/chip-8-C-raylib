LINKER_FLAGS := -lopengl32 -lraylib -lgdi32 -lwinmm 
INCLUDE_PATH := "include"
LIBRARY_PATH := "lib"
BUILD_DIR := build
DEBUG := -g
FLAGS := -Wall
OBJ_DEPENDENCY := ${BUILD_DIR}/main.o ${BUILD_DIR}/stack.o ${BUILD_DIR}/renderer.o

main.exe: ${OBJ_DEPENDENCY}
	gcc -o $@ ${OBJ_DEPENDENCY} ${FLAGS} ${DEBUG} -L${LIBRARY_PATH} ${LINKER_FLAGS}

${BUILD_DIR}/main.o: main.c stack.c
	gcc main.c -c ${DEBUG} ${FLAGS} -I${INCLUDE_PATH} -o $@

${BUILD_DIR}/stack.o: stack.c
	gcc stack.c -c ${DEBUG} ${FLAGS} -I${INCLUDE_PATH} -o $@

${BUILD_DIR}/renderer.o: renderer.c
	gcc renderer.c -c ${DEBUG} ${FLAGS} -I${INCLUDE_PATH} -o $@