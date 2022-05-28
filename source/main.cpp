#include <iostream>
#include <exception>
#include "mainController.hpp"
#include "productModel.hpp"

using namespace std;

int main(){
    try {
        DatabaseConnection connection = startDatabaseConnection();
        cout << "Success opening the database!" << endl;
        ProductModel m = ProductModel::createProduct(connection, "Banana", 2.50, 100);
        cout << "Created product " << m.name() << " with id=" << m.id() <<  endl;
        ProductModel m2 = ProductModel::createProduct(connection, "Maçã", 5.20, 5);
        cout << "Created product " << m2.name() << " with id=" << m2.id() <<  endl;
        m2.updateAmount(10);
        m2.updatePrice(6.20);
        cout << "Connection closed! Success!" << endl;
    } catch(std::exception& error) {
        cerr << "Fatal application error encountered:\n" << error.what() << "\nExiting...\n";
        return 1;
    } 
    return 0;
}