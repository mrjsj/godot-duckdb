extends Node

var db : GDDuckDB

func _ready():
	
	db = GDDuckDB.new()
	db.open_db()
	db.connect()
	
	var sql_query = "SELECT 'Hello, world!' as msg;"
	
	db.query(sql_query)

	print(db.query_result)

	db.disconnect()
	db.close_db()
