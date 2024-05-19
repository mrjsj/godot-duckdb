#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
sources = [Glob("src/*.cpp")]

duckdb_include_path = "src/duckdb"
# duckdb_lib_path = "./src/duckdb/"

env.Append(CPPPATH=[duckdb_include_path])
env.Append(HPPPATH=[duckdb_include_path])
# env.Append(LIBPATH=[duckdb_lib_path])
# env.Append(LIBS=['libduckdb.dylib'])

# env.Append(LIBPATH=['demo/bin/'])
# env.Append(LIBS=['libduckdb.dylib'])

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demo/bin/gdduckdb.{}.{}.framework/gdduckdb.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "demo/bin/gdduckdb{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
