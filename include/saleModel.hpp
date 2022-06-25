#ifndef SALE_MODEL_H
#define SALE_MODEL_H
#include <string>
#include <vector>
#include "databaseConnection.hpp"
#include "dateTime.hpp"
#include "productModel.hpp"

using std::string, std::vector;

struct ProductEntry {
    ProductEntry(ProductModel product_model, int product_amount) :
        product(product_model), amount(product_amount) {

    }

    ProductModel product;
    int amount;
};

class SaleModel {
    public:
        static SaleModel getSale(DatabaseConnection& db_connection, int id);
        static vector<SaleModel> getAllSales(DatabaseConnection& db_connection);
        static SaleModel createSale(DatabaseConnection& db_connection,
            const vector<ProductEntry>& products, double total);
        static void removeSale(DatabaseConnection& db_connection, int id);
        int id() const;
        DateTime timestamp() const;
        double totalValue() const;
        const vector<ProductEntry>& products() const;
        vector<ProductEntry> products();
    private:
        SaleModel(DatabaseConnection& db_connection, int id, DateTime timestamp,
            double total_value, const vector<ProductEntry>& products);

        int id_;
        DateTime timestamp_;
        double total_value_;
        vector<ProductEntry> products_;
        DatabaseConnection& db_connection_;
};

#endif