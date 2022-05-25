CC= g++
CFLAGS= -Wall -c
INC= -I include/

# tp01: main.cpp cashierController.o mainController.o managerController.o reportModeController.o
tp01: main.cpp cashierController.o managerController.o reportModeController.o
#	$(CC) main.cpp cashierController.o mainController.o managerController.o reportModeController.o -o tp01
	$(CC) main.cpp cashierController.o managerController.o reportModeController.o -o tp01
	
cashierController.o: source/cashierController.cpp
	$(CC) $(CFLAGS) $(INC) source/cashierController.cpp
	
# mainController.o: source/mainController.cpp
# 	$(CC) $(CFLAGS) $(INC) source/mainController.cpp
	
managerController.o: source/managerController.cpp
	$(CC) $(CFLAGS) $(INC) source/managerController.cpp
	
reportModeController.o: source/reportModeController.cpp
	$(CC) $(CFLAGS) $(INC) source/reportModeController.cpp
	
run:
	./tp01
	
clean:
	rm -rf *.o tp01
