#ifndef GDDuckDB_CLASS_H
#define GDDuckDB_CLASS_H

//#include <Reference.hpp>
#include "duckdb/duckdb.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/node2d.hpp>

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/marshalls.hpp>
#include <godot_cpp/classes/project_settings.hpp>

//#include <vfs/gdsqlite_vfs.h>
#include <cstring>
#include <fstream>
#include <memory>
#include <sstream>
#include <vector>

//using namespace godot;
//using namespace duckdb;


namespace godot {
using DuckDB = duckdb::DuckDB;

class GDDuckDB : public Node2D {
    GDCLASS(GDDuckDB, Node2D);

private:
    DuckDB *db;
    // duckdb::Connection *con;

protected:
    static void _bind_methods();

public:
    GDDuckDB();

    ~GDDuckDB();

    // Functions
    Array hello_world();

    void _process(double delta);    
};

}

#endif // ! GDDuckDB_CLASS_H