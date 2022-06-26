#ifndef MAIN_CONTROLLER_HPP
#define MAIN_CONTROLLER_HPP
#include "databaseConnection.hpp"
#include "managerController.hpp"
#include "cashierController.hpp"
#include "reportModeController.hpp"

DatabaseConnection startDatabaseConnection();

class MainController {
    public:
        MainController(DatabaseConnection& db_connection);
        void displayMenu();
        void processOption(int option_code);
        bool finished();
    private:
        void displayCashierSubmenu();
        void displayManagerSubmenu();
        void displayReportSubmenu();
        void displaySalesSubmenu();
        void processCashierOption(int option);
        void processManagerOption(int option);
        void processReportOption(int options);

        CashierController cashier_mode_;
        ManagerController manager_mode_;
        ReportModeController report_mode_;
        bool finished_;
};

#endif