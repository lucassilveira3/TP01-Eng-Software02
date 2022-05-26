#include "databaseDriver.hpp"
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


QueryResults::QueryResults(int status_code, int num_columns, vector<map<string, string>>&& rows) :
    status_code_(status_code), num_columns_(num_columns), rows_(rows) {

}

bool QueryResults::success() {
    return status_code_ == SQLITE_OK;
}

bool QueryResults::has_rows() {
    return num_rows() > 0;
}

int QueryResults::status_code() {
    return status_code_;
}

int QueryResults::num_rows() {
    return rows_.size();
}
int QueryResults::num_columns() {
    return num_columns_;
}
vector<map<string, string>>& QueryResults::rows() {
    return rows_;
}

const vector<map<string, string>>& QueryResults::rows() const {
    return rows_;
}

void DatabaseDriver::connect(string database_path) {
    int status_code = sqlite3_open(database_path.c_str(), &db_handle_);
    if(status_code != SQLITE_OK) {
        throw DatabaseException("Failed to open database file \"" + database_path + "\"!",
        status_code, sqlite3_errmsg(db_handle_));
    }
}

/* 
    Auxiliary struct to help in the processing of the rows returned by the 
*/
struct RowProcessingData {
    int row_number, num_columns;
    vector<map<string, string>>& result_rows;
};


QueryResults DatabaseDriver::execute(string sql_statement) {
    char* error_message = nullptr;
    vector<map<string, string>> rows;
    RowProcessingData auxiliary_data{0, 0, rows};
    int status = sqlite3_exec(db_handle_, sql_statement.c_str(),
        DatabaseDriver::process_result_row, (void*)&auxiliary_data, &error_message);
    return QueryResults(status, auxiliary_data.num_columns, std::move(rows));
}

void DatabaseDriver::close_connection() {
    int status_code = sqlite3_close(db_handle_);
    if(status_code != SQLITE_OK) {
        throw DatabaseException("Failed to close database connection!",
            status_code, sqlite3_errmsg(db_handle_));
    }
}

DatabaseDriver::~DatabaseDriver() {
    sqlite3_close(db_handle_);
}


int DatabaseDriver::process_result_row(
    void* processing_data,
    int num_columns,
    char** row_data,
    char** column_names
) {
    RowProcessingData* auxiliary_data = (RowProcessingData*)processing_data;
    auxiliary_data->num_columns = num_columns;
    map<string, string> row;
    for(int i = 0; i < num_columns; i++) {
        row.emplace(column_names[i], (row_data[i] ? row_data[i] : "NULL"));
    }
    auxiliary_data->result_rows.emplace_back(std::move(row));
    auxiliary_data->row_number++;
    return 0;
}
