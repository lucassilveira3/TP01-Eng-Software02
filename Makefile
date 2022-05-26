CC= g++
CFLAGS= -Wall -std=c++17 -ggdb
INC= -I include/
LIB= -l sqlite3
EXEC_NAME= tp01

all: build

build: main.o cashierController.o managerController.o reportModeController.o databaseDriver.o
	$(CC) $(CFLAGS) $^ -o $(EXEC_NAME) $(LIB)
	
main.o: main.cpp
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@

cashierController.o: source/cashierController.cpp
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@
	
# mainController.o: source/mainController.cpp
# 	$(CC) $(CFLAGS) $(INC) source/mainController.cpp
	
managerController.o: source/managerController.cpp
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@
	
reportModeController.o: source/reportModeController.cpp
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@

databaseDriver.o: source/databaseDriver.cpp
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@

run:
	./tp01
	
clean:
	rm -rf *.o tp01
