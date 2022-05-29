#ifndef MANAGER_CONTROLLER_HPP
#define MANAGER_CONTROLLER_HPP
#include <string>
#include <vector>
#include "databaseConnection.hpp"

using std::string, std::vector;

class ManagerController{
    public:
        static ManagerController createProduct(DatabaseConnection& db_connection,
            string name, double price, int amount);
        static void removeProduct(DatabaseConnection& db_connection, int id);
        void updatePrice(double new_price);
        void updateAmount(int new_amount);
        //void checkStock();
		int id();
        string name();
        double price();
        int amount();
    private:
		ManagerController(int id, string name, double price, int amount,
            DatabaseConnection& db_connection);

        int id_, amount_;
        string name_;
        double price_;
        DatabaseConnection& db_connection_;
};

#endif