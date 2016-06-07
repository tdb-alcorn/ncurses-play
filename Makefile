compile:
	gcc $(src).c -lncurses -o $(src)

run: compile
	./$(src)
