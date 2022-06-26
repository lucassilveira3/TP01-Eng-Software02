#include "reportModeController.hpp"        
#include <stdexcept>

using std::runtime_error;

ReportModeController::ReportModeController(DatabaseConnection& db_connection)
    : db_connection_(db_connection) {

}

vector<vector<string>> ReportModeController::allSales() {
    QueryResults results = db_connection_.execute("SELECT strftime(\"%Y-%m-%d\", date) AS date, name AS `product name`, SellProducts.amount, price FROM Sells JOIN SellProducts ON Sells.id=sellId JOIN Products ON productId=Products.id");
    return process_results(results);
}

vector<vector<string>> ReportModeController::salesOnPeriod(string period_start,
    string period_end) {
    string sql_statement = db_connection_.prepareStatement(
        "SELECT strftime(\"%Y-%m-%d\", date) AS date, name AS `product name`, SellProducts.amount, price FROM Sells JOIN SellProducts ON Sells.id=sellId JOIN Products ON productId=Products.id WHERE strftime(\"%Y-%m-%d\", date) BETWEEN ? AND ?",
        "ss",
        period_start,
        period_end
    );
    QueryResults results = db_connection_.execute(sql_statement);
    return process_results(results);
}

vector<vector<string>> ReportModeController::mostSoldItems() {
    QueryResults results = db_connection_.execute("SELECT name, sum(SellProducts.amount) AS quantity FROM SellProducts JOIN Products ON productId=id GROUP BY productId ORDER BY quantity DESC");
    return process_results(results);
}

vector<vector<string>> ReportModeController::highestSalesDays() {
    QueryResults results = db_connection_.execute("SELECT strftime(\"%Y-%m-%d\", date) AS date, count(*) AS `num sales` FROM Sells GROUP BY strftime(\"%Y-%m-%d\", date) ORDER BY `num sales` DESC");
    return process_results(results);
}

vector<vector<string>> ReportModeController::lowestSalesDays() {
    QueryResults results = db_connection_.execute("SELECT strftime(\"%Y-%m-%d\", date) AS date, count(*) AS `num sales` FROM Sells GROUP BY strftime(\"%Y-%m-%d\", date) ORDER BY `num sales`");
    return process_results(results);
}

vector<vector<string>> ReportModeController::process_results(QueryResults& results) {
    if(!results.success()) {
        throw runtime_error("Failed to query sale data from the database!\nError: " + results.status_message());
    }
    int num_rows = results.num_rows(), num_columns = results.num_columns();
    vector<vector<string>> result_table;
    result_table.reserve(num_rows + 1);
    result_table.push_back(results.column_names());
    for(int i = 1; i < num_rows + 1; i++) {
        result_table.emplace_back(num_columns, "");
        int j = 0;
        for(auto column : results.rows()[i - 1]) {
            result_table[i][j] = column.second;
            j++;
        }
    }
    return result_table;
}
