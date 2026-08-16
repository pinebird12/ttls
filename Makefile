CC := clang
BUILD_DIR := ./build
SRC_DIR := ./src
SRCS := client.o server.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%)

all : $(OBJS)

$(BUILD_DIR)/client.o : $(SRC_DIR)/client.c $(SRC_DIR)/elgamal.h
	$(CC) $(SRC_DIR)/client.c -o $@

$(BUILD_DIR)/server.o : $(SRC_DIR)/server.c $(SRC_DIR)/elgamal.h
	$(CC) $(SRC_DIR)/server.c -o $@

clean :
	rm -fr build/*.o
