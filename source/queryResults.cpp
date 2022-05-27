#include "queryResults.hpp"
#include <sqlite3.h>

QueryResults::QueryResults(
    int status_code,
    string status_message,
    vector<string>&& columns,
    vector<unordered_map<string, string>>&& rows
) : status_code_(status_code), status_message_(status_message),
    column_names_(columns), rows_(rows) { }

bool QueryResults::success() {
    return status_code_ == SQLITE_OK;
}

bool QueryResults::has_rows() {
    return num_rows() > 0;
}

int QueryResults::status_code() {
    return status_code_;
}

string QueryResults::status_message() {
    return status_message_;
}

int QueryResults::num_rows() {
    return rows_.size();
}
int QueryResults::num_columns() {
    return column_names_.size();
}

const vector<string>& QueryResults::column_names() {
    return column_names_;
}

vector<unordered_map<string, string>>& QueryResults::rows() {
    return rows_;
}

const vector<unordered_map<string, string>>& QueryResults::rows() const {
    return rows_;
}
