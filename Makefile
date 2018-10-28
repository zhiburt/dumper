# Это комментарий, который говорит, что переменная CC указывает компилятор, используемый для сборки
CC=gсс

all: src/main.c src/dumper.c
	gcc src/main.c src/dumper.c -o debug/main
