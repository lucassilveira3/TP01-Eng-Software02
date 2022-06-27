CC= g++
CFLAGS= -Wall -std=c++17 -ggdb
INC= -I include/
LIB= -l sqlite3
EXEC_NAME= tp01

all: build

unit_tests: unit
	./unit

build: main.o mainController.o managerController.o reportModeController.o saleModel.o productModel.o dateTime.o databaseConnection.o queryResults.o cashierController.o
	$(CC) $(CFLAGS) $^ -o $(EXEC_NAME) $(LIB)

unit: tests/unit/unit_tests.cpp dateTime.o
	$(CC) $(CFLAGS) $(INC) $^ -o $@
	
main.o: source/main.cpp
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@

cashierController.o: source/cashierController.cpp
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@
	
mainController.o: source/mainController.cpp
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@
	
managerController.o: source/managerController.cpp
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@
	
reportModeController.o: source/reportModeController.cpp
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@

saleModel.o: source/saleModel.cpp
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@

productModel.o: source/productModel.cpp
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@

dateTime.o: source/dateTime.cpp
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@

databaseConnection.o: source/databaseConnection.cpp
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@

queryResults.o: source/queryResults.cpp
	$(CC) $(CFLAGS) $(INC) -c $^ -o $@

run:
	./tp01
	
clean:
	rm -rf *.o tp01
