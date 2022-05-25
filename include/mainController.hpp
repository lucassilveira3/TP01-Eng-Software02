#ifndef MAIN_CONTROLLER_HPP
#define MAIN_CONTROLLER_HPP

#include <iostream>

void printMenu() {
    std::cout << "=======================" << std::endl;
    std::cout << "Welcome\n" << std::endl;
    std::cout << "1 - Cashier mode" << std::endl;
    std::cout << "2 - Manager mode" << std::endl;
    std::cout << "3 - Report mode" << std::endl;
    std::cout << "4 - Exit" << std::endl;
    std::cout << "=======================" << std::endl;
    std::cout << "Select mode: " << std::endl;
}

void selectCashierType() {
    int cashierType = 0;

    switch(cashierType) {
        case 1:
        break;
        case 2:
        break;
        case 3:
        break;
        case 4:
        break;
    };
}

#endif