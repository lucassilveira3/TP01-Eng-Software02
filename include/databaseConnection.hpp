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
        /*
            This static method receives a string with a SQL statement that has some 
            parameters represented by "?"s. The types of the parameters should also
            be given, in the same order that they are provided, as a string with the
            following codification:
                s - for string (as const char*)
                i - for integer
                d - for double (real)
                t - for date-time (pointer to an instance of the DateTimeClass)
                n - for null
            The parameters are bound in the order that they are given to the method and
            the types must correspond to each parameter given. The prepared sql statement
            is returned as a string
        */
        string prepareStatement(const string& sql_statement, const string& parameter_types, ...);
        int lastIdInserted();
        void close_connection();
        ~DatabaseConnection();
    private:
        static int process_result_row(void* processing_params, int num_columns,
            char** row_data, char** column_names);

        sqlite3* db_handle_;
};

#endif