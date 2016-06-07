compile:
	gcc $(src).c -lncurses -o $(src)

compile-debug:
	gcc -g $(src).c -lncurses -o $(src)-debug

run: compile
	./$(src)

debug: compile-debug
	valgrind --leak-check=yes ./$(src)

clean:
	rm -r *.dSYM
	ls | grep -v "\." |  grep -v Makefile | xargs rm
