import sys
import os

template = """
[plugin]

name="Godot DuckDB"
description="GDNative wrapper for DuckDB (Godot 4.X+), making it possible to use DuckDB databases as query engine."
author="mrjsj"
version="{version}"
script="godot-duckdb.gd"
"""


if __name__ == "__main__":

    if len(sys.argv) != 2:
        raise Exception("git tag must be supplied")

    tag = sys.argv[-1]
    print("--- TAG ---")
    print(tag)
    print()

    version = tag.split("-")[0].lstrip("v")
    print("--- VERSION ---")
    print(version)
    print()
    
    content = template.strip().format(version=version)
    print("--- CONTENT ---")
    print(content)
    print()
    
    os.makedirs("addons", exist_ok=True)
    with open("addons/plugin.cfg", "w") as f:
        f.write(content)
