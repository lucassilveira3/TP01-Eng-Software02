#include <iostream>
#include <exception>
#include "mainController.hpp"

using namespace std;

int main(){
    try {
    DatabaseConnection connection = startDatabaseConnection();
    } catch(std::exception& error) {
        cerr << "Fatal application error encountered:\n" << error.what() << "\nExiting...\n";
        return 1;
    } 
    cout << "Success!" << endl;
    return 0;
}