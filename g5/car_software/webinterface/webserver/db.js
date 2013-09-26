var sqlite3 = require('sqlite3').verbose();
var db = new sqlite3.Database("./testdb.db");

exports.close = function(){
	db.close();
};

exports.sensor = (function(){
	//var ready = false;
	var createSchema = function(){
		var sensorsTable = [
			"CREATE TABLE IF NOT EXISTS sensors (",
			"id			INTEGER PRIMARY KEY AUTOINCREMENT",
			"					NOT NULL",
			"					UNIQUE,",
			"name 		TEXT,",
			"value 		INTEGER,",
			"timestamp	INTEGER NOT NULL",
			")"
		].join("\n");

		db.run(sensorsTable, function(err){
			if (err){
				console.trace(err)
			};
			//ready = true;
		});
	};

	var insert = function(sensors, next){
		db.serialize(function(){
			var query = "INSERT INTO sensors(name, value, timestamp) VALUES(?, ?, ?)";
			var stmt = db.prepare(query, function(err){
				if(err) console.trace(err);
			});

			for(var i=0; i < sensors.length; i++){
				var params = [
					sensors[i].name,
					sensors[i].val,
					sensors[i].timestamp
				]
				stmt.run(params);
			}
			stmt.finalize(next);
		});
	};

	var getByNumber = function(number, next){
		var query = "SELECT name, value, timestamp FROM sensors ORDER BY timestamp DESC LIMIT ?"
		db.all(query, [number], function(err, rows){
			if (err) console.trace(err);

			if(!(typeof next === 'undefined')){
				next(err, rows);
			}
		});

	};

	// We need to create the database schema before
	// we can use the query the database so create
	// before exposing the api
	createSchema();

	return {
		insert: insert,
		getByNumber: getByNumber
	}
})();
