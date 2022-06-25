#ifndef MANAGER_CONTROLLER_HPP
#define MANAGER_CONTROLLER_HPP
#include <string>
#include <vector>
#include "databaseConnection.hpp"

using std::string, std::vector;

struct InventoryEntry {
    InventoryEntry(string name, int product_amount, double product_price) :
        product_name(name), amount(product_amount), price(product_price) {

    }

    string product_name;
    int amount;
    double price;
};

class ManagerController{
    public:
        ManagerController(DatabaseConnection& db_connection);
        void createProduct(const InventoryEntry& new_entry);
        void updatePrice(string product_name, double new_price);
        void updateInventory(string product_name, int new_amount);
        void removeProduct(string product_name);
        vector<InventoryEntry> checkInventory();
    private:
        DatabaseConnection& db_connection_;
};

#endif