#ifndef CASHIER_CONTROLLER_HPP
#define CASHIER_CONTROLLER_HPP
#include <string>
#include <vector>
#include "databaseConnection.hpp"

using std::string, std::vector;

class CashierController{
    public:
        static CashierController getById(DatabaseConnection& db_connection, int id);
        // Returns the first product matching the given name on the database
        static CashierController getByName(DatabaseConnection& db_connection, string name);
        static vector<CashierController> getAll(DatabaseConnection& db_connection);
        int id();
        string name();
        double price();
        int amount();
        //
        void openSale();
        void finishSale();
        void addItem();
        void removeItem();
        void registerSale();
    private:
        CashierController();

        int id_, amount_;
        string name_;
        double price_;
        DatabaseConnection& db_connection_;
};

#endif