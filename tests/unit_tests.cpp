#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "dateTime.hpp"
#include "cashierController.hpp"
#include "managerController.hpp"
#include "databaseConnection.hpp"
#include "mainController.hpp"

TEST_SUITE("DateTime class tests") {
    TEST_CASE("Construct from string") {
        DateTime date1("2020-06-05", "%Y-%m-%d");
        CHECK(date1.day() == 5);
        CHECK(date1.month() == 6);
        CHECK(date1.year() == 2020);
        DateTime date2("23/06/2022", "%d/%m/%Y");
        CHECK(date2.day() == 23);
        CHECK(date2.month() == 6);
        CHECK(date2.year() == 2022);
    }

    TEST_CASE("Construct from timestamp") {
        tm ts;
        ts.tm_year = 122;
        ts.tm_mon = 5;
        ts.tm_mday = 23;
        ts.tm_hour = 18;
        ts.tm_min = 0;
        ts.tm_sec = 30;
        time_t timestamp = mktime(&ts);
        DateTime date(timestamp);
        CHECK(date.day() == 23);
        CHECK(date.month() == 6);
        CHECK(date.year() == 2022);
        CHECK(date.timestamp() == timestamp);
    }

    TEST_CASE("String representation") {
        DateTime date("23-06-2022 18:47:30", "%d-%m-%Y %H:%M:%S");
        CHECK(date.to_string("%Y-%m-%d") == "2022-06-23");
        CHECK(date.to_string("%d/%m/%Y - %H:%M") == "23/06/2022 - 18:47");
        CHECK(date.to_string("%Y-%m-%d %H:%M:%S") == "2022-06-23 18:47:30");
    }
}

TEST_SUITE("CashierController class test") {
    TEST_CASE("Checking no sale open") {
        DatabaseConnection db = startDatabaseConnection();
        CashierController cash(db);
        CHECK(cash.hasOpenSell() == false);
    }

    TEST_CASE("Checking open sale") {
        DatabaseConnection db = startDatabaseConnection();
        CashierController cash(db);
        cash.openSale();
        CHECK(cash.hasOpenSell() == true);
    }

    TEST_CASE("Checking open sale after finished") {
        DatabaseConnection db = startDatabaseConnection();
        CashierController cash(db);
        cash.openSale();
        cash.finishSale();
        CHECK(cash.hasOpenSell() == false);
    }

    TEST_CASE("Checking open sale after cancelled") {
        DatabaseConnection db = startDatabaseConnection();
        CashierController cash(db);
        cash.openSale();
        cash.cancelSale();
        CHECK(cash.hasOpenSell() == false);
    }
}
