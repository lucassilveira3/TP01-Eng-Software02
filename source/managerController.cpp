#include "managerController.hpp"
#include <stdexcept>
#include "productModel.hpp"

using std::runtime_error;

ManagerController::ManagerController(DatabaseConnection& db_connection)
    : db_connection_(db_connection) {

}

void ManagerController::createProduct(const InventoryEntry& new_entry) {
    ProductModel::createProduct(db_connection_, new_entry.product_name,
        new_entry.price, new_entry.amount);
}

void ManagerController::updatePrice(string product_name, double new_price) {
    ProductModel target_product = ProductModel::getByName(db_connection_, product_name);
    target_product.updatePrice(new_price);
}

void ManagerController::updateInventory(string product_name, int new_amount) {
    ProductModel target_product = ProductModel::getByName(db_connection_, product_name);
    target_product.updateAmount(new_amount);
}

void ManagerController::removeProduct(string product_name) {
    ProductModel target_product = ProductModel::getByName(db_connection_, product_name);
    ProductModel::removeProduct(db_connection_, target_product.id());
}

vector<InventoryEntry> ManagerController::checkInventory() {
    vector<ProductModel> products = ProductModel::getAll(db_connection_);
    vector<InventoryEntry> inventory_table;
    for(ProductModel& product : products) {
        inventory_table.emplace_back(product.name(), product.amount(), product.price());
    }
    return inventory_table;
}
