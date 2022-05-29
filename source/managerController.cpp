#include "managerController.hpp"
#include <stdexcept>

using std::runtime_error, std::string;

ManagerController::ManagerController(
int id,
string name,
double price,
int amount,
DatabaseConnection& db_connection
) : id_(id), amount_(amount), name_(name), price_(price), db_connection_(db_connection) { }

ManagerController ManagerController::createProduct(
    DatabaseConnection& db_connection,
    string name,
    double price,
    int amount
) {
    string sql_statement = db_connection.prepareStatement(
        "INSERT INTO Products (name, price, amount) VALUES (?, ?, ?)",
        "sdi",
        name.c_str(),
        price,
        amount
    );
    QueryResults result = db_connection.execute(sql_statement);
    if(!result.success()) {
        throw runtime_error("Failed to create new product!\nDatabase error encountered (code " + 
            std::to_string(result.status_code()) + "): " + result.status_message());        
    }
    return ManagerController(db_connection.lastIdInserted(), name, price, amount, db_connection);
}

void ManagerController::removeProduct(DatabaseConnection& db_connection, int id) {
    string sql_statement = db_connection.prepareStatement("DELETE FROM products WHERE id=?", "i", id);
    QueryResults result = db_connection.execute(sql_statement);
    if(!result.success()) {
        throw runtime_error("Failed to delete product with id=" + std::to_string(id) +
            "from the database!\nDatabase error encountered (code " +  std::to_string(result.status_code()) +
                "): " + result.status_message());        
    }
}

int ManagerController::id() {
    return id_;
}

string ManagerController::name() {
    return name_;
}

double ManagerController::price() {
    return price_;
}

int ManagerController::amount() {
    return amount_;
}

void ManagerController::updatePrice(double new_price) { 
    string sql_statement = db_connection_.
        prepareStatement("UPDATE Products SET price=? WHERE id=?", "di"
        , new_price, id_);
    QueryResults result = db_connection_.execute(sql_statement);
    if(!result.success()) {
        throw runtime_error("Failed to update product with id=" + std::to_string(id_) + 
            "!\nDatabase error encountered (code " + std::to_string(result.status_code()) + 
                "): " + result.status_message());
    }
    price_ = new_price;
}

void ManagerController::updateAmount(int new_amount) {
    string sql_statement = db_connection_.
        prepareStatement("UPDATE Products SET amount=? WHERE id=?", "ii"
        , new_amount, id_);
    QueryResults result = db_connection_.execute(sql_statement);
    if(!result.success()) {
        throw runtime_error("Failed to update product with id=" + std::to_string(id_) + 
            "!\nDatabase error encountered (code " + std::to_string(result.status_code()) + 
                "): " + result.status_message());
    }
    amount_ = new_amount;
}