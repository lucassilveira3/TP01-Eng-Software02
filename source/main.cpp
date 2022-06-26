#include <iostream>
#include <exception>
#include "mainController.hpp"
#include "productModel.hpp"

using namespace std;

int main(){
    try {
        DatabaseConnection database_connection = startDatabaseConnection();
        MainController application(database_connection);
        int option;
        while(!application.finished()) {
            application.displayMenu();
            cin >> option;
            application.processOption(option);
        }
    } catch(std::exception& error) {
        cerr << "Fatal application error encountered:\n" << error.what() << "\nExiting...\n";
        return 1;
    } 
    return 0;
}