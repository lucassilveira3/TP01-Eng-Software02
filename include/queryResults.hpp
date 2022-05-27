#ifndef QUERY_RESULTS_H
#define QUERY_RESULTS_H
#include <string>
#include <vector>
#include <unordered_map>

using std::string, std::vector, std::unordered_map;

class QueryResults {
    public:
        QueryResults(int status_code, string status_message,
            vector<string>&& column_names, vector<unordered_map<string, string>>&& rows);
        bool success();
        bool has_rows();
        int status_code();
        string status_message();
        int num_rows();
        int num_columns();
        const vector<string>& column_names();
        vector<unordered_map<string, string>>& rows();
        const vector<unordered_map<string, string>>& rows() const;
    private:
        int status_code_;
        string status_message_;
        vector<string> column_names_;
        vector<unordered_map<string, string>> rows_;
};

#endif