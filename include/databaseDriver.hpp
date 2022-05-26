#ifndef DATABASE_DRIVER_H
#define DATABASE_DRIVER_H
#include <sqlite3.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>

using std::string, std::vector, std::map;

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

class QueryResults {
    public:
        QueryResults(int status_code, int num_columns_, vector<map<string, string>>&& rows);
        bool success();
        bool has_rows();
        int status_code();
        int num_rows();
        int num_columns();
        vector<map<string, string>>& rows();
        const vector<map<string, string>>& rows() const;
    private:
        int status_code_, num_columns_;
        vector<map<string, string>> rows_;
};

class DatabaseDriver {
    public:
        DatabaseDriver() = default;
        void connect(string database_path);
        QueryResults execute(string sql_statement);
        void close_connection();
        ~DatabaseDriver();
    private:
        static int process_result_row(void* processing_data, int num_columns,
            char** row_data, char** column_names);

        sqlite3* db_handle_;
};

#endif