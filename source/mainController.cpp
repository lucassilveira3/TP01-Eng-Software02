#include "mainController.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <exception>
#include <thread>
#include <chrono>

#define DATABASE_FILE_PATH "data.db"
#define DATABASE_CREATE_SCRIPT_PATH "database/initializeScript.sql"
#ifdef _WIN32
#define CLEAR_CMD "cls"
#else
#define CLEAR_CMD "clear"
#endif

using namespace std;

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

MainController::MainController(DatabaseConnection& db_connection) :
    cashier_mode_(db_connection), manager_mode_(db_connection), report_mode_(db_connection) {

}

void MainController::displayMenu()  {
    system(CLEAR_CMD);
    cout << "=======================" << endl;
    cout << "Welcome\n" << endl;
    cout << "1 - Cashier mode" << endl;
    cout << "2 - Manager mode" << endl;
    cout << "3 - Report mode" << endl;
    cout << "4 - Exit" << endl;
    cout << "=======================" << endl;
    cout << "Select mode: ";
}

void MainController::processOption(int option_code) {
    int sub_option;
    switch(option_code) {
        case 1:
            displayCashierSubmenu();
            cin >> sub_option;
            processCashierOption(sub_option);
            break;
        case 2:
            displayManagerSubmenu();
            cin >> sub_option;
            processManagerOption(sub_option);
            break;
        case 3:
            displayReportSubmenu();
            cin >> sub_option;
            processReportOption(sub_option);
            break;
        case 4:
            cout << "Exiting..." << endl;
            finished_ = true;
            break;
        default:
            cout << "Invalid option \"" + std::to_string(option_code) + "\" given!\nPlease, enter a valid option...\n";
            break;
    }
    this_thread::sleep_for(chrono::milliseconds(800));
}

bool MainController::finished() {
    return finished_;
}

void MainController::displayCashierSubmenu() {
    system(CLEAR_CMD);
    cout << "=======================" << endl;
    cout << "Cashier Mode\n" << endl;
    cout << "1 - Start sale" << endl;
    cout << "2 - Exit" << endl;
    cout << "=======================" << endl;
    cout << "Select an option: ";
}

void MainController::displayManagerSubmenu() {
    system(CLEAR_CMD);
    cout << "=======================" << endl;
    cout << "Manager mode\n" << endl;
    cout << "1 - Register product" << endl;
    cout << "2 - Update price" << endl;
    cout << "3 - Remove product" << endl;
    cout << "4 - Update inventory" << endl;
    cout << "5 - Check inventory" << endl;
    cout << "6 - Exit" << endl;
    cout << "=======================" << endl;
    cout << "Select an option: ";
}

void MainController::displayReportSubmenu() {
    system(CLEAR_CMD);
    cout << "=======================" << endl;
    cout << "Report mode\n" << endl;
    cout << "1 - List all sales" << endl;
    cout << "2 - List the most sold items" << endl;
    cout << "3 - List sales on period" << endl;
    cout << "4 - List the days with the most sells" << endl;
    cout << "5 - List the days with the least sells" << endl;
    cout << "6 - Exit" << endl;
    cout << "=======================" << endl;
    cout << "Select an option: ";
}

void MainController::processCashierOption(int option) {
    switch(option) {
        case 1:
            cashier_mode_.openSale();
            break;
        case 2:
            cout << "Exiting...\n";
            break;
    }
    this_thread::sleep_for(chrono::milliseconds(800));
}

void MainController::processManagerOption(int option) {
    switch(option) {
        case 1:
        {
            string name;
            int amount;
            double price;
            cout << "Enter the product name: ";
            getline(cin, name);
            cout << "Enter the product's stock (units): ";
            cin >> amount;
            cout << "Enter the product's price";
            cin >> price;
            manager_mode_.createProduct({name, amount, price});
            break;
        }
        case 2:
        {
            string name;
            double new_price;
            cout << "Enter the product name: ";
            getline(cin, name);
            cout << "Enter the product's new price: ";
            cin >> new_price;
            manager_mode_.updatePrice(name, new_price);
            break;
        }
        case 3:
        {
            string name;
            cout << "Enter the product name: ";
            getline(cin, name);
            manager_mode_.removeProduct(name);
            break;
        }
        case 4:
        {
            string name;
            int new_amount;
            cout << "Enter the product name: ";
            getline(cin, name);
            cout << "Enter the product's new price: ";
            cin >> new_amount;
            manager_mode_.updateInventory(name, new_amount);
            break;
        }
        case 5:
        {
            vector<InventoryEntry> inventory = manager_mode_.checkInventory();
            break;
        }
        case 6:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid option! Please, enter a valid option...\n";
            break;
    }
    this_thread::sleep_for(chrono::milliseconds(800));
}

void MainController::processReportOption(int options) {
    switch(options) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid option! Please, enter a valid option...\n";
            break;
    }
    this_thread::sleep_for(chrono::milliseconds(800));
}
