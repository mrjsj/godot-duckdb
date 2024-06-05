#ifndef GDDuckDB_CLASS_H
#define GDDuckDB_CLASS_H

#include "duckdb/duckdb.h"

#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/ref_counted.hpp>

//#include <godot_cpp/classes/global_constants.hpp>
// #include <godot_cpp/classes/marshalls.hpp>
// #include <godot_cpp/classes/project_settings.hpp>

#include <cstring>
#include <fstream>
#include <memory>
#include <sstream>
#include <vector>

namespace godot {

class GDDuckDB : public RefCounted {
        GDCLASS(GDDuckDB, RefCounted);

private:

        duckdb_database db;
        duckdb_connection con;
        TypedArray<Dictionary> query_result = TypedArray<Dictionary>();

        std::vector<std::unique_ptr<Callable>> function_registry;
        int64_t verbosity_level = 1;

	bool read_only = false;
        
        CharString path_utf8;
	const char* path = ":memory:";

        const char* duckdb_type_to_string(duckdb_type type);
        Variant map_duckdb_type_to_godot_variant(duckdb_result &result, idx_t col_idx, idx_t row_idx);
        bool is_connection_valid();

protected:
        static void _bind_methods();

public:

        enum VerbosityLevel {
            QUIET = 0,
            NORMAL = 1,
            VERBOSE = 2,
            VERY_VERBOSE = 3
        };

        GDDuckDB();
        ~GDDuckDB();

        // Functions
        bool hello_world();
        bool open_db();
        bool close_db();
        bool connect();
        bool disconnect();
        bool query(const String &sql_query);
        bool query_chunk(const String &sql_query);


        void set_query_result(const TypedArray<Dictionary> &p_query_result);

        TypedArray<Dictionary> get_query_result() const;

        void set_read_only(const bool &_read_only);
        bool get_read_only() const;
        void set_path(const String &_path);
        String get_path() const;

};

}

#endif // ! GDDuckDB_CLASS_H