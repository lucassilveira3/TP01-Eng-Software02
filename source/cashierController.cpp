#include "cashierController.hpp"
#include <stdexcept>

using std::runtime_error;

CashierController::CashierController(DatabaseConnection& db_connection) 
    : db_connection_(db_connection), total_accumulated_(0), sale_opened_(false) {

}

void CashierController::openSale() {
    if(sale_opened_) {
        throw runtime_error("Failed to open a new sale!\nAnother sale was opened and never closed!");
    }
    total_accumulated_ = 0;
    sale_products_.clear();
    sale_opened_ = true;
}

void CashierController::addItem(string product_name, int amount) {
    int entry_index = product_entry_index(product_name);
    if(entry_index >= 0) {
        throw runtime_error("Failed to add item named \"" + product_name + 
            "\" to the sale!\nItem already added!");
    }
    ProductModel product = ProductModel::getByName(db_connection_, product_name);
    sale_products_.emplace_back(product, amount);
}

void CashierController::removeItem(string product_name) {
    int entry_index = product_entry_index(product_name);
    if(entry_index < 0) {
        throw runtime_error("A product named \"" + product_name +
            "wasn't found in the current sale's product list!");
    }
    sale_products_[entry_index].amount = 0;
}

void CashierController::finishSale() {
    if(!sale_opened_) {
        throw runtime_error("Failed to finish sale: a sale was never opened!\nPlease, open a sale first!");
    }
    SaleModel::createSale(db_connection_, sale_products_, total_accumulated_);
    sale_opened_ = false;
}

void CashierController::cancelSale() {
    sale_opened_ = false;
}

int CashierController::product_entry_index(string product_name) {
    for(size_t i = 0; i < sale_products_.size(); i++) {
        if(sale_products_[i].product.name() == product_name) {
            return i;
        }
    }
    return -1;
}
