#include <iostream>
#include "databaseConnection.hpp"

using namespace std;

int main(){
    DatabaseConnection db;
    try{
        db.connect("data.db");
        auto r = db.execute("SELECT * FROM Responsavel");
        if(r.success()) {
            cout << "Query successifull!" << endl;
            cout << "Returned " << r.num_columns() << " columns and " << r.num_rows() << " rows!" << endl;
            cout << "Columns: ";
            for(int i = 0; i < r.num_columns(); i++) {
                cout << r.column_names()[i] << ((i == r.num_columns() - 1) ? "\n" : ", ");
            }
            cout << "Results: " << endl;
            for(int i = 0; i < r.num_rows(); i++) {
                cout << "Row " << i << ":\n";
                for(int j = 0; j < r.num_columns(); j++) {
                    cout << r.column_names()[j] << " = " << r.rows()[i][r.column_names()[j]] << endl;
                }
                cout << endl;
            }
            cout << endl;
        }else {
            cout << "Query error! Code: " << r.status_code() << endl;
        }
    } catch(DatabaseException& e) {
        cout << "Database error: " << endl << e.what() << endl;
        return 1;
    } 
    return 0;
}