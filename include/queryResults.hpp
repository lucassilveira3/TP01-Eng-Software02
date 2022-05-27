#ifndef QUERY_RESULTS_H
#define QUERY_RESULTS_H
#include <string>
#include <vector>
#include <unordered_map>

using std::string, std::vector, std::unordered_map;

class QueryResults {
    public:
        QueryResults(int status_code, int num_columns_, vector<unordered_map<string, string>>&& rows);
        bool success();
        bool has_rows();
        int status_code();
        int num_rows();
        int num_columns();
        vector<unordered_map<string, string>>& rows();
        const vector<unordered_map<string, string>>& rows() const;
    private:
        int status_code_, num_columns_;
        vector<unordered_map<string, string>> rows_;
};

#endif