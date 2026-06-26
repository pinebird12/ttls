all: build/client.bin build/server.bin
build/client.bin: src/client.c
	gcc -o build/client.bin src/client.c
build/server.bin: src/server.c
	gcc -o build/server.bin src/server.c

