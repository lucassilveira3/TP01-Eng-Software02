#include "mainController.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <exception>

#define DATABASE_FILE_PATH "data.db"
#define DATABASE_CREATE_SCRIPT_PATH "database/initializeScript.sql"

using std::cout, std::endl, std::ifstream, std::string;

void printMenu()  {
    cout << "=======================" << endl;
    cout << "Welcome\n" << endl;
    cout << "1 - Cashier mode" << endl;
    cout << "2 - Manager mode" << endl;
    cout << "3 - Report mode" << endl;
    cout << "4 - Exit" << endl;
    cout << "=======================" << endl;
    cout << "Select mode: " << endl;
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

DatabaseConnection createDatabase() {
    ifstream initialization_script_file(DATABASE_CREATE_SCRIPT_PATH);
    initialization_script_file.seekg(0, std::ios::end);
    size_t script_file_size = initialization_script_file.tellg();
    initialization_script_file.seekg(0);
    string  initialization_script;
    initialization_script.reserve(script_file_size);
    initialization_script.assign(
        std::istreambuf_iterator<char>(initialization_script_file),
        std::istreambuf_iterator<char>()
    );
    DatabaseConnection new_database;
    new_database.connect(DATABASE_FILE_PATH);
    new_database.execute(initialization_script);
    return new_database;
}

DatabaseConnection startDatabaseConnection() {
    DatabaseConnection connection;
    if(!std::filesystem::exists(DATABASE_FILE_PATH)) {
        try {
            connection = createDatabase();
        } catch(DatabaseException& e) {
            throw std::runtime_error(string("Exception encountered while creating a new database:\n") + e.what());
        }
        return connection;
    }
    try {
        connection.connect(DATABASE_FILE_PATH);
    } catch(DatabaseException& e) {
        throw std::runtime_error(string("Exception encountered while opening the database file:\n") + e.what());
    }
    return connection;
}
