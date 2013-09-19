var sqlite3 = require('sqlite3').verbose();
var db = new sqlite3.Database("/Users/jepper_broge/Unicorn_race_engineering/g5/car_software/webinterface/webserver/testdb.db");

exports.sensor = (function(){
	//var ready = false;
	var createSchema = function(){
		var sensorTable = [
			"CREATE TABLE IF NOT EXISTS sensors (",
			"id			INTEGER PRIMARY KEY AUTOINCREMENT",
			"					NOT NULL",
			"					UNIQUE,",
			"name 		TEXT,",
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
			//var query = "INSERT INTO sensors(name, value) VALUES(?, ?)";
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
		//var query = "SELECT name, value FROM sensors DESC LIMIT ?"
		db.all(query, [number], function(err, rows){
			if (err) console.trace(err);

			console.log(rows);
			if(!(typeof next === 'undefined')){
				next(err, rows);
			}
		});

	};

	createSchema();

	return {
		insert: insert,
		getByNumber: getByNumber
	}
})();
