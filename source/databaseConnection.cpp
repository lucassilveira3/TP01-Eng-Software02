#include "databaseConnection.hpp"
#include <sstream>
#include <cstring>
#include <cstdarg>
#include "dateTime.hpp"

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


string DatabaseConnection::prepareStatement(const string& sql_statement,
     const string& parameter_types, ...) {
        int num_parameters = parameter_types.size();
        sqlite3_stmt* statement;
        std::va_list parameters;
        va_start(parameters, parameter_types);
        int status_code = sqlite3_prepare_v2(db_handle_, sql_statement.c_str(), 
            sql_statement.length(), &statement, nullptr);
        if(status_code != SQLITE_OK) {
            throw DatabaseException("Failed to prepare SQL statement \"" + sql_statement + "\"",
                status_code, sqlite3_errmsg(db_handle_));
        }
        for(int i = 0; i < num_parameters; i++) {
            switch(parameter_types[i]) {
                case 's': 
                {
                    const char* parameter = va_arg(parameters, const char*);
                    int status_code = sqlite3_bind_text(statement, i + 1,
                        parameter, strlen(parameter), SQLITE_STATIC);
                    if(status_code != SQLITE_OK) {
                        throw DatabaseException("Failed to bind string parameter to the SQL statement!",
                            status_code, sqlite3_errmsg(db_handle_));
                    }
                    break;
                }
                case 'i':
                {
                    int parameter = va_arg(parameters, int);
                    int status_code = sqlite3_bind_int(statement, i + 1, parameter);
                    if(status_code != SQLITE_OK) {
                        throw DatabaseException("Failed to bind integer parameter to the SQL statement!",
                            status_code, sqlite3_errmsg(db_handle_));
                    }
                    break;
                }
                case 'd':
                {
                    double parameter = va_arg(parameters, double);
                    int status_code = sqlite3_bind_double(statement, i + 1, parameter);
                    if(status_code != SQLITE_OK) {
                        throw DatabaseException("Failed to bind double (real) parameter to the SQL statement!",
                            status_code, sqlite3_errmsg(db_handle_));
                    }
                    break;
                }
                case 't':
                {
                    DateTime* parameter = va_arg(parameters, DateTime*);
                    const string date_string = parameter->to_string();
                    int status_code = sqlite3_bind_text(statement, i + 1,
                        date_string.c_str(), date_string.size(), SQLITE_STATIC);
                    if(status_code != SQLITE_OK) {
                        throw DatabaseException("Failed to bind datetime parameter to the SQL statement!",
                            status_code, sqlite3_errmsg(db_handle_));
                    }
                    break;
                }
                case 'n':
                {
                    int status_code = sqlite3_bind_null(statement, i + 1);
                    if(status_code != SQLITE_OK) {
                        throw DatabaseException("Failed to bind null parameter to the SQL statement!",
                            status_code, sqlite3_errmsg(db_handle_));
                    }
                    break;
                }
                default: 
                    throw std::runtime_error(string("Failed to bind SQL statement!\nAn invalid parameter type \"") + parameter_types[i] + "\" was provided!");
            }
        }
        return string(sqlite3_expanded_sql(statement));
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
        if(error_message != nullptr) {
            status_message = error_message;
            sqlite3_free(error_message);
        }else {
            status_message = sqlite3_errmsg(db_handle_);
        }
    }else {
        status_message = "OK";
    }
    return QueryResults(status_code, status_message, std::move(column_names), std::move(rows));
}

int DatabaseConnection::lastIdInserted() {
    return sqlite3_last_insert_rowid(db_handle_);
}

void DatabaseConnection::close_connection() {
    int status_code = sqlite3_close(db_handle_);
    if(status_code != SQLITE_OK) {
        throw DatabaseException("Failed to close database connection!",
            status_code, sqlite3_errmsg(db_handle_));
    }
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
