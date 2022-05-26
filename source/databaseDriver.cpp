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


void DatabaseDriver::connect(string database_path) {
    int status_code = sqlite3_open(database_path.c_str(), &db_handle_);
    if(status_code != SQLITE_OK) {
        throw DatabaseException("Failed to open database file \"" + database_path + "\"!",
        status_code, sqlite3_errmsg(db_handle_));
    }
}

void DatabaseDriver::execute(string sql_statement) {

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
