#include "queryResults.hpp"
#include <sqlite3.h>

QueryResults::QueryResults(int status_code, int num_columns, vector<map<string, string>>&& rows) :
    status_code_(status_code), num_columns_(num_columns), rows_(rows) { }

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
