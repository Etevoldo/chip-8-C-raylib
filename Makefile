LINKER_FLAGS := -lopengl32 -lraylib -lgdi32 -lwinmm -Wl,--defsym,stat64i32=_stat64
INCLUDE_PATH := "include"
LIBRARY_PATH := "lib"

main.exe: main.o
	gcc main.o -o game.exe -L${LIBRARY_PATH} ${LINKER_FLAGS}

main.o: main.c
	gcc main.c -c -I${INCLUDE_PATH} -o $@