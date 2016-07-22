run: compile
	./$(src)

compile:
	gcc $(src).c -lncurses -o $(src)

compile-debug:
	gcc -g $(src).c -lncurses -o $(src)-debug

debug: compile-debug
	valgrind --leak-check=full ./$(src)

clean:
	rm -r *.dSYM
	ls | grep -v "\." |  grep -v Makefile | xargs rm
