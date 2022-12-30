CC=clang
CFLAGS= -O3

ALL:	SHA256	MAIN

SHA256:
		$(CC) $(CFLAGS) test.c sha256.c -o sha256
MAIN:
		$(CC) $(CFLAGS) main.c sha256.c -o main

clean:
		rm -r sha256
		rm -r main