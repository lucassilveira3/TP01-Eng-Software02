#include "cashierController.hpp"
#include <stdexcept>

using std::runtime_error, std::string;

CashierController::CashierController(
int id,
string name,
double price,
int amount,
DatabaseConnection& db_connection
) : id_(id), amount_(amount), name_(name), price_(price), db_connection_(db_connection) { }

CashierController CashierController::getById(DatabaseConnection& db_connection, int id) {
    string sql_statement = db_connection.
        prepareStatement("SELECT * FROM Prodcuts WHERE id=?", "i", id);
    QueryResults result = db_connection.execute(sql_statement);
    if(!result.success()) {
        throw runtime_error("Failed to retrieve product with id=" + std::to_string(id) + "!\nDatabase error encountered: " + result.status_message());
    }
    if(result.num_rows() != 1) {
        throw runtime_error("Error retrieving product with id=" + std::to_string(id) + "!\nNot enough rows returned by the database!");
    }
    // Processing the returned row
    string name = result.rows()[0]["name"];
    double price = std::stod(result.rows()[0]["price"]);
    int amount = std::stoi(result.rows()[0]["amount"]);
    return CashierController(id, name, price, amount, db_connection);
}

CashierController CashierController::getByName(DatabaseConnection& db_connection, string name) {
    string sql_statement = db_connection.
        prepareStatement("SELECT * FROM Prodcuts WHERE name=?", "s", name.c_str());
    QueryResults result = db_connection.execute(sql_statement);
    if(!result.success()) {
        throw runtime_error("Failed to retrieve product with name=" + name + "!\nDatabase error encountered: " + result.status_message());
    }
    if(result.num_rows() != 1) {
        throw runtime_error("Error retrieving product with name=" + name + "!\nNot enough rows returned by the database!");
    }
    // Processing the returned row
    int id = std::stoi(result.rows()[0]["id"]);
    double price = std::stod(result.rows()[0]["price"]);
    int amount = std::stoi(result.rows()[0]["amount"]);
    return CashierController(id, name, price, amount, db_connection);
}

vector<CashierController> CashierController::getAll(DatabaseConnection& db_connection) {
    QueryResults results = db_connection.execute("SELECT * FROM Products");
    if(!results.success()) {
        throw runtime_error("Failed to retrieve all products!\nDatabase error (code " + 
            std::to_string(results.status_code()) + ") encountered: " + results.status_message());
    }
    if(!results.has_rows()) {
        throw runtime_error("Error retrieving all products from the database!\nNo rows returned!");
    }
    // Processing the returned rows
    vector<CashierController> products;
    products.reserve(results.num_rows());
    for(int i = 0; i < results.num_rows(); i++) {
        int id = std::stoi(results.rows()[0]["id"]);
        string name = results.rows()[0]["name"];
        double price = std::stod(results.rows()[0]["price"]);
        int amount = std::stoi(results.rows()[0]["amount"]);
        products.emplace_back(CashierController(id, name, price, amount, db_connection));
    }
    return products;
}

int CashierController::id() {
    return id_;
}

string CashierController::name() {
    return name_;
}

double CashierController::price() {
    return price_;
}

int CashierController::amount() {
    return amount_;
}