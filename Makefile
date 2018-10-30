# Это комментарий, который говорит, что переменная CC указывает компилятор, используемый для сборки
CC=gсс

all: src/main.c src/dumper.c
	gcc src/main.c src/dumper.c -o debug/main

shared: src/dumper.c
	gcc -fPIC -c src/dumper.c -o debug/dumper.o
	gcc -shared -o debug/dumper.dll debug/dumper.o