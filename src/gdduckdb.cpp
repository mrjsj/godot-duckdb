#include "gdduckdb.h"
//#include "duckdb/duckdb.hpp"

#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/utility_functions.hpp>


using namespace godot;
using namespace std;
//using namespace duckdb;

void GDDuckDB::_bind_methods() {
        // Bind the select_from_table method to be callable from Godot
        ClassDB::bind_method(D_METHOD("hello_world"), &GDDuckDB::hello_world);

        ClassDB::bind_method(D_METHOD("open_db"), &GDDuckDB::open_db);
        ClassDB::bind_method(D_METHOD("close_db"), &GDDuckDB::close_db);
        ClassDB::bind_method(D_METHOD("connect"), &GDDuckDB::connect);
        ClassDB::bind_method(D_METHOD("disconnect"), &GDDuckDB::disconnect);
        ClassDB::bind_method(D_METHOD("query", "sql_query"), &GDDuckDB::query);
        
        // TODO: Implement query_chunk
        //ClassDB::bind_method(D_METHOD("query_chunk", "sql_query"), &GDDuckDB::query_chunk);


        ClassDB::bind_method(D_METHOD("set_query_result", "query_result"), &GDDuckDB::set_query_result);
        ClassDB::bind_method(D_METHOD("get_query_result"), &GDDuckDB::get_query_result);
        ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "query_result", PROPERTY_HINT_ARRAY_TYPE, "Dictionary"), "set_query_result", "get_query_result");


}

GDDuckDB::GDDuckDB() {
    db = nullptr;
    con = nullptr;
    query_result = TypedArray<Dictionary>();
}

GDDuckDB::~GDDuckDB() {
	/* Clean up the function_registry */
	function_registry.clear();
	function_registry.shrink_to_fit();
	/* Close the database connection if it is still open */
    disconnect();
    close_db();
}

bool GDDuckDB::hello_world() {
    duckdb_result result;
    duckdb_state status = duckdb_query(con, "SELECT 'Hello, World!'", &result); 

    if (status == DuckDBError) {
        UtilityFunctions::printerr("GDDuckDB Error: Failed to run query.");
        duckdb_destroy_result(&result);
        return false;
    }

    if (duckdb_row_count(&result) > 0 && duckdb_column_count(&result) > 0) {
        // Extract the string value from the first column of the first row
        char *value = duckdb_value_varchar(&result, 0, 0);
        if (value) {
            // Print the value using UtilityFunctions
            UtilityFunctions::print(value);
            // Free the value after use
            duckdb_free(value);
        }
    }

    duckdb_destroy_result(&result);
    return true;
}

bool GDDuckDB::query_chunk(const String &sql_query) {
    duckdb_result result;
    duckdb_state status = duckdb_query(con, sql_query.utf8(), &result);

    if (status == DuckDBError) {
        UtilityFunctions::printerr("GDDuckDB Error: Failed to run query: " + String(duckdb_result_error(&result)));
        duckdb_destroy_result(&result);
        return false;
    }

    idx_t column_count = duckdb_column_count(&result);
    idx_t row_count = duckdb_row_count(&result);
    query_result.clear(); // Clear previous results

    auto chunk_count = duckdb_result_chunk_count(result);

    Dictionary data_dict;
    

    Dictionary col_dict;
    for (idx_t col_idx = 0; col_idx < column_count; ++col_idx) {
        //const char *col_name = duckdb_column_name(&result, col_idx);
        duckdb_type col_type = duckdb_column_type(&result, col_idx);
        col_dict[col_idx] = col_type;
    }

    for (idx_t chunk_idx = 0; chunk_idx < chunk_count; ++chunk_idx) {
        duckdb_data_chunk chunk = duckdb_result_get_chunk(result, chunk_idx);
        for (idx_t col_idx = 0; col_idx < column_count; ++col_idx) {

            const char *col_name = duckdb_column_name(&result, col_idx);
            UtilityFunctions::print(col_name);

            duckdb_vector vector = duckdb_data_chunk_get_vector(chunk, col_idx);

            duckdb_logical_type col_type = duckdb_vector_get_column_type(vector);


            int64_t* col_value = (int64_t*)duckdb_vector_get_data(vector);

            data_dict[String(col_name)] = col_value;
            query_result.push_back(data_dict);

            UtilityFunctions::print("This happened");

            
            UtilityFunctions::print(col_value);

        }
    }
    return true;
}

bool GDDuckDB::query(const String &sql_query) {
    duckdb_result result;
    duckdb_state status = duckdb_query(con, sql_query.utf8(), &result);

    if (status == DuckDBError) {
        UtilityFunctions::printerr("GDDuckDB Error: Failed to run query: " + String(duckdb_result_error(&result)));
        duckdb_destroy_result(&result);
        return false;
    }

    idx_t column_count = duckdb_column_count(&result);
    idx_t row_count = duckdb_row_count(&result);
    query_result.clear(); // Clear previous results

    for (idx_t row_idx = 0; row_idx < row_count; ++row_idx) {
        Dictionary row_dict;

        for (idx_t col_idx = 0; col_idx < column_count; ++col_idx) {

            const char *col_name = duckdb_column_name(&result, col_idx);
            duckdb_type col_type = duckdb_column_type(&result, col_idx);

            Variant col_value;

            if (duckdb_value_is_null(&result, col_idx, row_idx)) {
                col_value = Variant(); // Assign null
            } else {
                switch (col_type) {
                    case DUCKDB_TYPE_BOOLEAN:
                        col_value = Variant((bool)duckdb_value_boolean(&result, col_idx, row_idx));
                        break;
                    case DUCKDB_TYPE_INTEGER:
                        col_value = Variant((int)duckdb_value_int32(&result, col_idx, row_idx));
                        break;
                    case DUCKDB_TYPE_BIGINT:
                        col_value = Variant((int64_t)duckdb_value_int64(&result, col_idx, row_idx));
                        break;
                    case DUCKDB_TYPE_DECIMAL:
                        col_value = Variant(duckdb_decimal_to_double(duckdb_value_decimal(&result, col_idx, row_idx)));
                    case DUCKDB_TYPE_FLOAT:
                        col_value = Variant((float)duckdb_value_float(&result, col_idx, row_idx));
                        break;
                    case DUCKDB_TYPE_DOUBLE:
                        col_value = Variant(duckdb_value_double(&result, col_idx, row_idx));
                        break;
                    case DUCKDB_TYPE_VARCHAR:
                        {
                            char *str_value = duckdb_value_string(&result, col_idx, row_idx).data;
                            col_value = Variant(String(str_value));
                            duckdb_free(str_value);
                        }
                        break;
                    default:
                        UtilityFunctions::printerr("Unhandled column type: (" + String(std::to_string(col_type).c_str()) + ") " + String(duckdb_type_to_string(col_type)));
                        break;
                }
            }
            row_dict[String(col_name)] = col_value;
        }

        query_result.append(row_dict);
    }

    duckdb_destroy_result(&result);
    return true;
}

const char* GDDuckDB::duckdb_type_to_string(duckdb_type type) {
    switch (type) {
        case DUCKDB_TYPE_INVALID: return "DUCKDB_TYPE_INVALID";
        case DUCKDB_TYPE_BOOLEAN: return "DUCKDB_TYPE_BOOLEAN";
        case DUCKDB_TYPE_TINYINT: return "DUCKDB_TYPE_TINYINT";
        case DUCKDB_TYPE_SMALLINT: return "DUCKDB_TYPE_SMALLINT";
        case DUCKDB_TYPE_INTEGER: return "DUCKDB_TYPE_INTEGER";
        case DUCKDB_TYPE_BIGINT: return "DUCKDB_TYPE_BIGINT";
        case DUCKDB_TYPE_UTINYINT: return "DUCKDB_TYPE_UTINYINT";
        case DUCKDB_TYPE_USMALLINT: return "DUCKDB_TYPE_USMALLINT";
        case DUCKDB_TYPE_UINTEGER: return "DUCKDB_TYPE_UINTEGER";
        case DUCKDB_TYPE_UBIGINT: return "DUCKDB_TYPE_UBIGINT";
        case DUCKDB_TYPE_FLOAT: return "DUCKDB_TYPE_FLOAT";
        case DUCKDB_TYPE_DOUBLE: return "DUCKDB_TYPE_DOUBLE";
        case DUCKDB_TYPE_DECIMAL: return "DUCKDB_TYPE_DECIMAL";
        case DUCKDB_TYPE_TIMESTAMP: return "DUCKDB_TYPE_TIMESTAMP";
        case DUCKDB_TYPE_DATE: return "DUCKDB_TYPE_DATE";
        case DUCKDB_TYPE_TIME: return "DUCKDB_TYPE_TIME";
        case DUCKDB_TYPE_INTERVAL: return "DUCKDB_TYPE_INTERVAL";
        case DUCKDB_TYPE_VARCHAR: return "DUCKDB_TYPE_STRING";
        case DUCKDB_TYPE_BLOB: return "DUCKDB_TYPE_BLOB";
        case DUCKDB_TYPE_ENUM: return "DUCKDB_TYPE_ENUM";
        case DUCKDB_TYPE_LIST: return "DUCKDB_TYPE_LIST";
        case DUCKDB_TYPE_MAP: return "DUCKDB_TYPE_MAP";
        case DUCKDB_TYPE_STRUCT: return "DUCKDB_TYPE_STRUCT";
        case DUCKDB_TYPE_TIMESTAMP_TZ: return "DUCKDB_TYPE_TIMESTAMP_TZ";
        // Add other cases as needed
        default: return "UNKNOWN_TYPE";
    }
}

bool GDDuckDB::connect() {
    if (db) {

        duckdb_state status = duckdb_connect(db, &con);
        if (status == DuckDBError) {
            UtilityFunctions::printerr("GDDuckDB Error: Failed to open database.");
            return false;
        }
        return true;
    }

    UtilityFunctions::printerr("GDDuckDB Error: Can't connect if database is not opened. Please run open_db first.");
    return false;
}


bool GDDuckDB::open_db() {
    // Use the C API to open a DuckDB database
    //duckdb_database db;
    duckdb_state status = duckdb_open(nullptr, &db);
    if (status == DuckDBError) {
        UtilityFunctions::printerr("GDDuckDB Error: Failed to open database.");
        return false;
    }
    return true;
}


void GDDuckDB::set_query_result(const TypedArray<Dictionary> &p_query_result) {
	query_result = p_query_result;
}


TypedArray<Dictionary> GDDuckDB::get_query_result() const {
	return query_result.duplicate(true);
}


bool GDDuckDB::disconnect() {
        if (!con) {
            UtilityFunctions::printerr("GDDuckDB Error: Can't disconnect when no connection is open!");
            return false;
        }
        duckdb_disconnect(&con);
        return true;
}


bool GDDuckDB::close_db() {
    if (con) {
        UtilityFunctions::printerr("GDDuckDB Error: Can't close database if connection is still open!");
        return false;
    }

    if (!db) {
        UtilityFunctions::printerr("GDDuckDB Error: Can't close database when no database is currently connected!");
        return false;
    }

    //duckdb_close(&db);
    return true;

}
