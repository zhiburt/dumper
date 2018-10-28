
all: src/main.c src/dumper.c
	gcc src/main.c src/dumper.c -o debug/main