#include "saleModel.hpp"
#include <stdexcept>

using std::runtime_error;

SaleModel::SaleModel(
    DatabaseConnection& db_connection,
    int id,
    DateTime timestamp,
    double total_value,
    const vector<ProductEntry>& products
    ) : id_(id), timestamp_(timestamp), total_value_(total_value),
        products_(products), db_connection_(db_connection) {

}

SaleModel SaleModel::getSale(DatabaseConnection& db_connection, int id) {
    string sql_statement = db_connection.prepareStatement(
            "SELECT * FROM Sells WHERE id=?", "i", id);
    QueryResults sale_data = db_connection.execute(sql_statement);
    if(!sale_data.success()) {
        throw runtime_error("Failed to retrieve sale with id=" + std::to_string(id) +
            "!\nDatabase error encountered: " + sale_data.status_message());
    }
    if(sale_data.num_rows() != 1) {
        throw runtime_error("Error retrieving sale with id=" + std::to_string(id) + 
            "!\nNot enough rows returned by the database");
    }
    // Processing the sell data
    DateTime timestamp(sale_data.rows()[0]["date"]);
    double total_value = std::stod(sale_data.rows()[0]["total"]);
    // Processing the products data
    sql_statement = db_connection.prepareStatement("SELECT * FROM SellProducts sellId=?", "i", id);
    QueryResults sale_products_data = db_connection.execute(sql_statement);
    if(!sale_products_data.success()) {
        throw runtime_error("Failed to retrieve sale with id=" + std::to_string(id) +
            "!\nDatabase error encountered: " + sale_data.status_message());
    }
    if(sale_products_data.num_rows() < 1) {
        throw runtime_error("Error retrieving sale with id=" + std::to_string(id) + 
            "!\nNot enough rows returned by the database");
    }
    vector<ProductEntry> products;
    products.reserve(sale_products_data.num_rows());
    for(auto row : sale_products_data.rows()) {
        products.emplace_back(
            ProductModel::getById(db_connection, std::stoi(row["productId"])),
            std::stoi(row["amount"])
        );
    }
    return SaleModel(db_connection, id, timestamp, total_value, products);
}

vector<SaleModel> SaleModel::getAllSales(DatabaseConnection& db_connection) {
    QueryResults sale_ids = db_connection.execute("SELECT id FROM Sells");
    if(!sale_ids.success()) {
        throw runtime_error("");
    }
    vector<SaleModel> sales;
    sales.reserve(sale_ids.num_rows());
    for(auto row : sale_ids.rows()) {
        sales.emplace_back(getSale(db_connection, std::stoi(row["id"])));
    }
    return sales;
}

SaleModel SaleModel::createSale(DatabaseConnection& db_connection,
    const vector<ProductEntry>& products, double total) {
        DateTime sale_date = DateTime::now();
        string sql_statement = db_connection.prepareStatement(
            "INSERT INTO Sells (date, total) VALUES (?, ?)", "td", sale_date, total);
        QueryResults result = db_connection.execute(sql_statement);
        if(!result.success()) {
            throw runtime_error("Failed to create new sale!\nDatabase error encountered (code " +
                std::to_string(result.status_code()) + "): " + result.status_message());
        }
        int new_sell_id = db_connection.lastIdInserted();
        for(const ProductEntry& product_entry : products) {
            string product_insert_statement = db_connection.prepareStatement(
                "INSERT INTO SellProducts VALUES (?, ?, ?)",
                "iii",
                new_sell_id,
                product_entry.product.id(),
                product_entry.amount
            );
            QueryResults result = db_connection.execute(sql_statement);
            if(!result.success()) {
                throw runtime_error("Failed to associate product with id=" +
                    std::to_string(product_entry.product.id()) + " with the created sell (with id="
                        + std::to_string(new_sell_id));
            }
        }
        return SaleModel(db_connection, new_sell_id, sale_date, total, products);
}

void SaleModel::removeSale(DatabaseConnection& db_connection, int id) {
    string sql_statement = db_connection.prepareStatement("DELETE FROM Sells WHERE id=?", "i", id);
    QueryResults result = db_connection.execute(sql_statement);
    if(!result.success()) {
        throw runtime_error("Failed to delete product with id=" + std::to_string(id) +
            " from the database!\nDatabase error encountered (code " +
                std::to_string(result.status_code()) + "): " + result.status_message());
    }
}

int SaleModel::id() const {
    return id_;
}

DateTime SaleModel::timestamp() const {
    return timestamp_;
}

double SaleModel::totalValue() const {
    return total_value_;
}

const vector<ProductEntry>& SaleModel::products() const {
    return products_;
}

vector<ProductEntry> SaleModel::products() {
    return products_;
}
