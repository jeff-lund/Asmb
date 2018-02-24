default: main

main: main.c vectors.c vectors.s submission.c vectors.h
	gcc -fno-inline-small-functions -std=gnu99 -g -O0 -o main main.c vectors.s vectors.c submission.c -lm

test: test.c vectors.c vectors.s submission.c vectors.h
	gcc -fno-inline-small-functions -std=gnu99 -g -o test test.c vectors.s vectors.c submission.c -lm

clean:
	rm -f main
	rm -f test
