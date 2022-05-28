#include "productModel.hpp"
#include <stdexcept>

using std::runtime_error;

ProductModel::ProductModel(
int id,
string name,
double price,
int amount,
DatabaseConnection& db_connection
) : id_(id), amount_(amount), name_(name), price_(price), db_connection_(db_connection) { }

ProductModel ProductModel::getById(DatabaseConnection& db_connection, int id) {
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
    return ProductModel(id, name, price, amount, db_connection);
}

ProductModel ProductModel::getByName(DatabaseConnection& db_connection, string name) {
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
    return ProductModel(id, name, price, amount, db_connection);
}

vector<ProductModel> ProductModel::getAll(DatabaseConnection& db_connection) {
    QueryResults results = db_connection.execute("SELECT * FROM Products");
    if(!results.success()) {
        throw runtime_error("Failed to retrieve all products!\nDatabase error (code " + 
            std::to_string(results.status_code()) + ") encountered: " + results.status_message());
    }
    if(!results.has_rows()) {
        throw runtime_error("Error retrieving all products from the database!\nNo rows returned!");
    }
    // Processing the returned rows
    vector<ProductModel> products;
    products.reserve(results.num_rows());
    for(int i = 0; i < results.num_rows(); i++) {
        int id = std::stoi(results.rows()[0]["id"]);
        string name = results.rows()[0]["name"];
        double price = std::stod(results.rows()[0]["price"]);
        int amount = std::stoi(results.rows()[0]["amount"]);
        products.emplace_back(ProductModel(id, name, price, amount, db_connection));
    }
    return products;
}

ProductModel ProductModel::createProduct(
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
    return ProductModel(db_connection.lastIdInserted(), name, price, amount, db_connection);
}

void ProductModel::removeProduct(DatabaseConnection& db_connection, int id) {
    string sql_statement = db_connection.prepareStatement("DELETE FROM products WHERE id=?", "i", id);
    QueryResults result = db_connection.execute(sql_statement);
    if(!result.success()) {
        throw runtime_error("Failed to delete product with id=" + std::to_string(id) +
            "from the database!\nDatabase error encountered (code " +  std::to_string(result.status_code()) +
                "): " + result.status_message());        
    }
}

int ProductModel::id() {
    return id_;
}

string ProductModel::name() {
    return name_;
}

double ProductModel::price() {
    return price_;
}

int ProductModel::amount() {
    return amount_;
}

void ProductModel::rename(string new_name) {
    string sql_statement = db_connection_.
        prepareStatement("UPDATE Products SET name=? WHERE id=?", "si", new_name.c_str(), id_);
    QueryResults result = db_connection_.execute(sql_statement);
    if(!result.success()) {
        throw runtime_error("Failed to update product with id=" + std::to_string(id_) + 
            "!\nDatabase error encountered (code " + std::to_string(result.status_code()) + 
                "): " + result.status_message());
    }
    name_ = new_name;
}

void ProductModel::updatePrice(double new_price) { 
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

void ProductModel::updateAmount(int new_amount) {
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
