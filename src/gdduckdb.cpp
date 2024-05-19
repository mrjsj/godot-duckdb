#include "gdduckdb.h"
//#include "duckdb/duckdb.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/utility_functions.hpp>


using namespace godot;
using namespace std;
using DuckDB = duckdb::DuckDB;
//using namespace duckdb;

void GDDuckDB::_bind_methods() {
        // Bind the select_from_table method to be callable from Godot
        ClassDB::bind_method(D_METHOD("hello_world"), &GDDuckDB::hello_world);
}

GDDuckDB::GDDuckDB() {
    DuckDB db(nullptr);
    // Connection con(db);
}

GDDuckDB::~GDDuckDB() {
    // delete connection;
    //delete db;
}

Array GDDuckDB::hello_world() {
    // Create a DuckDB instance and connection
    // db = duckdb::DuckDB(nullptr);
    // connection = duckdb::Connection(*db);

    // Execute the query
    // auto result = connection->Query("SELECT 'Hello, World!'");
    
    // // Check if the query was successful
    // if (result->HasError()) {
    //     ERR_PRINT("Query failed: " + String(result->GetError().c_str()));
    //     return "";
    // }

    // // Fetch the result (assuming only one row and one column is returned)
    // if (!result->) {
    //     auto value = result->GetValue(0, 0).ToString();
    //     results.push_back(String(value.c_str()));
    // }
    Array results;
    //auto result = connection->duckdb::Connection::Query("SELECT 'Hello, World!'");
    // if (result->HasError()) {
    //     //ERR_PRINT("Query failed: " + String(result->GetError().c_str()));
    //     cerr << result->GetError() << endl;
    //     return results;
    // } else {
    //     auto value = result->GetValue(0, 0).ToString();
    //     results.push_back(String(value.c_str()));
    // }

    UtilityFunctions::print("It works?");

    return results;
}

void GDDuckDB::_process(double delta){
    UtilityFunctions::print("Hello from process!");
}