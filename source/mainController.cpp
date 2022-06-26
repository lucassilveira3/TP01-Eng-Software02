#include "mainController.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <exception>
#include <thread>
#include <chrono>
#include <sstream>
#include <regex>

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

// Auxiliary function to center align the string using a given with
string centerAlign(const string& str, int width) {
    stringstream stream;
    int length = wstring_convert<codecvt_utf8<char32_t>, char32_t>{}.from_bytes(str).size();
    int padding = width - length + 2;
    const string spaces(padding / 2, ' ');
    stream << spaces << str << spaces;
    if(padding > 0 && padding % 2 != 0) {
        stream << " ";
    }
    return stream.str();
}

void prettyPrintTable(const vector<vector<string>>& table) {
    // Treating empty tables and getting the table's dimensions
    if(table.empty()) {
        return;
    }
    int rows = table.size(), columns = table[0].size();
    // Calculating the table columns max with
    vector<int> max_widths(columns, 0);
    for(int j = 0; j < columns; j++) {
        for(int i = 0; i < rows; i++) {
            if(max_widths[j] < ((int) table[i][j].size())) {
                max_widths[j] = table[i][j].size();
            }
        }
    }
    // Defining the print format
    const char* column_separator = "|";
    const char* row_separator = "-:";
    // Printing the header and the separator
    for(int j = 0; j < columns; j++) {
        cout << column_separator << centerAlign(table[0][j], max_widths[j]);
    }
    cout << column_separator << endl;
    for(int j = 0; j < columns; j++) {
        cout << column_separator;
        for(int k = 0; k < max_widths[j] + 1; k++) {
            cout << row_separator[0];
        }
        cout << row_separator[1];
    }
    cout << column_separator << endl;
    // Printing the rows
    for(int i = 1; i < rows; i++) {
        for(int j = 0; j < columns; j++) {
            cout << column_separator << centerAlign(table[i][j], max_widths[j]);
        }
        cout << column_separator << endl;
    }
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
        {
            exit_submenu_ = false;
            while(!exit_submenu_) {
                displayCashierSubmenu();
                cin >> sub_option;
                processCashierOption(sub_option);
            }
            break;
        }
        case 2:
        {
            exit_submenu_ = false;
            while(!exit_submenu_) {
                displayManagerSubmenu();
                cin >> sub_option;
                processManagerOption(sub_option);
            }
            break;
        }
        case 3:
        {
            exit_submenu_ = false;
            while(!exit_submenu_) {
                displayReportSubmenu();
                cin >> sub_option;
                processReportOption(sub_option);
            }
            break;
        }
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
            exit_submenu_ = true;
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
            getline(cin >> ws, name);
            cout << "Enter the product's stock (units): ";
            cin >> amount;
            cout << "Enter the product's price: ";
            cin >> price;
            manager_mode_.createProduct({name, amount, price});
            break;
        }
        case 2:
        {
            string name;
            double new_price;
            cout << "Enter the product name: ";
            getline(cin >> ws, name);
            cout << "Enter the product's new price: ";
            cin >> new_price;
            manager_mode_.updatePrice(name, new_price);
            break;
        }
        case 3:
        {
            string name;
            cout << "Enter the product name: ";
            getline(cin >> ws, name);
            manager_mode_.removeProduct(name);
            break;
        }
        case 4:
        {
            string name;
            int new_amount;
            cout << "Enter the product name: ";
            getline(cin >> ws, name);
            cout << "Enter the product's new amount: ";
            cin >> new_amount;
            manager_mode_.updateInventory(name, new_amount);
            break;
        }
        case 5:
        {
            vector<InventoryEntry> inventory = manager_mode_.checkInventory();
            vector<vector<string>> inventory_table = { { "name", "price", "quantity" } };
            inventory_table.reserve(inventory.size() + 1);
            for(InventoryEntry& entry : inventory) {
                inventory_table.emplace_back(vector<string>{ entry.product_name,
                    to_string(entry.price), to_string(entry.amount) });
            
            }
            cout << endl;
            prettyPrintTable(inventory_table);
            cout << "Press enter to continue...";
            cin.ignore();
            cin.get();
            return;
        }
        case 6:
            cout << "Exiting...\n";
            exit_submenu_ = true;
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
            prettyPrintTable(report_mode_.allSales());
            break;
        case 2:
            prettyPrintTable(report_mode_.mostSoldItems());
            break;
        case 3:
        {
            string period_start, period_end;
            regex date_pattern(R"(\d{4}-\d{2}-\d{2})");
            cout << "Enter the start of the desired period on the \"yyyy-mm-dd\" format: ";
            cin >> period_start;
            if(!regex_match(period_start, date_pattern)) {
                cout << "Invalid input! Please try again...\n";
                break;
            }
            cout << "Enter the end of the desired period on the \"yyyy-mm-dd\" format: ";
            cin >> period_end;
            if(!regex_match(period_end, date_pattern)) {
                cout << "Invalid input! Please try again...\n";
                break;
            }
            prettyPrintTable(report_mode_.salesOnPeriod(period_start, period_end));
            break;
        }
        case 4:
            prettyPrintTable(report_mode_.highestSalesDays());
            break;
        case 5:
            prettyPrintTable(report_mode_.lowestSalesDays());
            break;
        case 6:
            cout << "Exiting...\n";
            exit_submenu_ = true;
            return;
        default:
            cout << "Invalid option! Please, enter a valid option...\n";
            break;
    }
    cout << "Press enter to continue...";
    cin.ignore();
    cin.get();
}
