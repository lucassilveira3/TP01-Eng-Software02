#include "databaseConnection.hpp"
#include <sstream>

DatabaseException::DatabaseException(string message, int error_code, string sql_message) {
    custom_message_ = message;
    error_code_ = error_code;
    sql_message_ = sql_message;
    std::ostringstream display_message_stream;
    display_message_stream << custom_message_ << std::endl << "Error code: " << error_code << std::endl << "Error message: " << sql_message_ << std::endl;
    display_message_ = display_message_stream.str();
}

const char* DatabaseException::what() const noexcept {
    return display_message_.c_str();
}

int DatabaseException::error_code() {
    return error_code_;
}

string DatabaseException::sql_message() {
    return sql_message_;
}

void DatabaseConnection::connect(string database_path) {
    int status_code = sqlite3_open(database_path.c_str(), &db_handle_);
    if(status_code != SQLITE_OK) {
        throw DatabaseException("Failed to open database file \"" + database_path + "\"!",
        status_code, sqlite3_errmsg(db_handle_));
    }
}

/* 
    Auxiliary struct used to pass parameters to the row processing method, called
    by sqlite3 for every row returned by the database for the given query. This
    struct is used to control the operation of the processing function and to
    store the data after the processing.
*/
struct RowProcessingParams {
    int row_number;
    bool first_call;
    vector<string>& column_names;
    vector<unordered_map<string, string>>& result_rows;
};


QueryResults DatabaseConnection::execute(string sql_statement) {
    char* error_message = nullptr;
    vector<string> column_names;
    vector<unordered_map<string, string>> rows;
    RowProcessingParams params{0, true, column_names, rows};
    int status_code = sqlite3_exec(db_handle_, sql_statement.c_str(),
        DatabaseConnection::process_result_row, (void*)&params, &error_message);
    string status_message;
    if(status_code != SQLITE_OK) {
        status_message = error_message;
        sqlite3_free(error_message);
    }else {
        status_message = "OK";
    }
    return QueryResults(status_code, status_message, std::move(column_names), std::move(rows));
}

void DatabaseConnection::close_connection() {
    int status_code = sqlite3_close(db_handle_);
    if(status_code != SQLITE_OK) {
        throw DatabaseException("Failed to close database connection!",
            status_code, sqlite3_errmsg(db_handle_));
    }
}

DatabaseConnection::~DatabaseConnection() {
    sqlite3_close(db_handle_);
}


int DatabaseConnection::process_result_row(
    void* processing_params,
    int num_columns,
    char** row_data,
    char** column_names
) {
    RowProcessingParams* params = (RowProcessingParams*)processing_params;
    // We should only fill the column names array on the first call to the function
    if(params->first_call) {
        for(int i = 0; i < num_columns; i++) {
            params->column_names.emplace_back(column_names[i]);
        }
        params->first_call = false;
    }
    unordered_map<string, string> row;
    for(int i = 0; i < num_columns; i++) {
        row.emplace(column_names[i], (row_data[i] ? row_data[i] : "NULL"));
    }
    params->result_rows.emplace_back(std::move(row));
    params->row_number++;
    return 0;
}
