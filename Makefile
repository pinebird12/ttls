server: src/server.c
	gcc -o build/server.bin src/server.c

client: src/client.c
	gcc -o build/client.bin src/client.c
