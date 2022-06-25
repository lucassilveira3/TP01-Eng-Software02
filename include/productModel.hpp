#ifndef PRODUCT_MODEL_H
#define PRODUCT_MODEL_H
#include <string>
#include <vector>
#include "databaseConnection.hpp"

using std::string, std::vector;

class ProductModel {
    public:
        static ProductModel getById(DatabaseConnection& db_connection, int id);
        // Returns the first product matching the given name on the database
        static ProductModel getByName(DatabaseConnection& db_connection, string name);
        static vector<ProductModel> getAll(DatabaseConnection& db_connection);
        static ProductModel createProduct(DatabaseConnection& db_connection,
            string name, double price, int amount);
        static void removeProduct(DatabaseConnection& db_connection, int id);
        ProductModel(const ProductModel&) = default;
        int id() const;
        string name() const;
        double price() const;
        int amount() const;
        void rename(string new_name);
        void updatePrice(double new_price);
        void updateAmount(int new_amount);
    private:
        ProductModel(int id, string name, double price, int amount,
            DatabaseConnection& db_connection);

        int id_, amount_;
        string name_;
        double price_;
        DatabaseConnection& db_connection_;
};

#endif