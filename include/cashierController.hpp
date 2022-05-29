#ifndef CASHIER_CONTROLLER_HPP
#define CASHIER_CONTROLLER_HPP

#include "databaseConnection.hpp"

class CashierController{
    public:
        CashierController();
		~CashierController();

        void openSale();
        void finishSale();
        void addItem();
        void removeItem();
        void registerSale();
    private:
        
};

#endif