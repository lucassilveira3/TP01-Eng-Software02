#ifndef CASHIER_CONTROLLER_HPP
#define CASHIER_CONTROLLER_HPP
#include <string>
#include <vector>
#include "databaseConnection.hpp"
#include "saleModel.hpp"

using std::string, std::vector;

class CashierController{
    public:
        CashierController(DatabaseConnection& db_connection);
        void openSale();
        bool hasOpenSell();
        void addItem(string product_name, int amount);
        void removeItem(string product_name);
        void finishSale();
        void cancelSale();
    private:
        int product_entry_index(string product_name);

        DatabaseConnection& db_connection_;
        vector<ProductEntry> sale_products_;
        double total_accumulated_;
        bool sale_opened_;
};

#endif