#ifndef MANAGER_CONTROLLER_HPP
#define MANAGER_CONTROLLER_HPP
#include <string>
#include <vector>
#include "databaseConnection.hpp"

using std::string, std::vector;

class ManagerController{
    public:
        static ManagerController getById(DatabaseConnection& db_connection, int id);
        // Returns the first product matching the given name on the database
        static ManagerController getByName(DatabaseConnection& db_connection, string name);
        static ManagerController createProduct(DatabaseConnection& db_connection,
            string name, double price, int amount);
        static void removeProduct(DatabaseConnection& db_connection, int id);
		int id();
        string name();
        double price();
        int amount();
        void updatePrice(double new_price);
        void updateAmount(int new_amount);
        //void checkStock();
    private:
		ManagerController(int id, string name, double price, int amount,
            DatabaseConnection& db_connection);

        int id_, amount_;
        string name_;
        double price_;
        DatabaseConnection& db_connection_;
};

#endif