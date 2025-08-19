
# First update .gitmodules to desired branch/version
gd-update:
	git submodule update --init --recursive
	git submodule sync
	git submodule update --remote --merge

duckdb-update:
	wget https://github.com/duckdb/duckdb/releases/download/v1.3.2/libduckdb-linux-amd64.zip
	unzip libduckdb-linux-amd64.zip
	mv duckdb.h src/duckdb/duckdb.h
	mv duckdb.hpp src/duckdb/duckdb.hpp
	mv libduckdb.so src/duckdb/libduckdb.so
	rm libduckdb_static.a
	rm libduckdb-linux-amd64.zip

build-linux:
	scons platform=linux duckdb_lib_name=libduckdb.so -j8