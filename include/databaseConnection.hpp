#ifndef DATABASE_DRIVER_H
#define DATABASE_DRIVER_H
#include <sqlite3.h>
#include <iostream>
#include <stdexcept>
#include "queryResults.hpp"

using std::string;

class DatabaseException : public std::exception {
    public:
        DatabaseException(string message, int error_code, string sql_message);
        virtual const char* what() const noexcept override;
        int error_code();
        string sql_message();
        virtual ~DatabaseException() override = default;
    private:
        int error_code_;
        string display_message_, custom_message_, sql_message_;
};

class DatabaseConnection {
    public:
        DatabaseConnection() = default;
        void connect(string database_path);
        QueryResults execute(string sql_statement);
        void close_connection();
        ~DatabaseConnection();
    private:
        static int process_result_row(void* processing_params, int num_columns,
            char** row_data, char** column_names);

        sqlite3* db_handle_;
};

#endif