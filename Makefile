all:
	gcc myshell.c -o myshell -fsanitize="address"
