#ifndef REPORT_MODE_CONTROLLER_HPP
#define REPORT_MODE_CONTROLLER_HPP
#include <vector>
#include <string>
#include "databaseConnection.hpp"

using std::string, std::vector;

/*
    This class returns the requested report information on the sales. The
    information is returned as a table, given as a matrix of string, where the
    first line contains the header, with the column names.
*/
class ReportModeController{
    public:
        ReportModeController(DatabaseConnection& db_connection);
        vector<vector<string>> allSales();
        vector<vector<string>> salesOnPeriod(string period_start, string period_end);
        vector<vector<string>> mostSoldItems();
        vector<vector<string>> highestSalesDays();
        vector<vector<string>> lowestSalesDays();
    private:
        vector<vector<string>> process_results(QueryResults& results);

        DatabaseConnection db_connection_;
};

#endif