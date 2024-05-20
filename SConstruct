#!/usr/bin/env python
import os
import sys

target_path = ARGUMENTS.pop("target_path", "demo/addons/godot-duckdb/bin/")
target_name = ARGUMENTS.pop("target_name", "libgdduckdb")

target = "{}{}".format(
    target_path, target_name
)

env = SConscript("godot-cpp/SConstruct")
#env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME']=1

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

# Add duckdb dylib as dependency
dylib_path = 'src/duckdb'
dylib_name = 'libduckdb'
env.Append(LIBPATH=[dylib_path])
env.Append(LIBS=[dylib_name])

# Allow exceptions in code, this is required becausse DuckDB throws errors
env.Append(CCFLAGS=['-fexceptions'])

if env["platform"] == "macos":
    target = "{}.{}.{}.framework/{}.{}.{}".format(
        target,
        env["platform"], 
        env["target"],
        target_name,
        env["platform"],
        env["target"]
    )
else:
    target = "{}{}{}".format(
        target,
        env["suffix"],
        env["SHLIBSUFFIX"]
    )

library = env.SharedLibrary(target=target, source=sources)
Default(library)

