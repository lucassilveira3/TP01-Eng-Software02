#ifndef CASHIER_CONTROLLER_HPP
#define CASHIER_CONTROLLER_HPP

class CashierController{
    public:
        CashierController();
        ~CashierController();

        void run();
        void openSale();
        void finishSale();
        void addItem();
        void removeItem();
    private:

};

#endif