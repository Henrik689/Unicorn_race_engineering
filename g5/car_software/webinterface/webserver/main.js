console.log("Starting program");
var bb = require('../bonescript');
var fs = require('fs');
var process = require('process');
var serialport = require('serialport');
var io = require('socket.io');
var dataType = require('./sensor_config');
require('./server_files/server_functions');
var db = require("./db");

// Serial port
var SerialPort = serialport.SerialPort; // localize object constructor
var baudrate = 115200;
//var port = "/dev/tty.usbserial-A700eCo8";
var port = "/dev/ttyUSB0";
var sp = new SerialPort(port, {
	parser: serialport.parsers.raw,
	baudrate: baudrate
});

// Global vars
var dataTypeKey = -1;
var package_start = false;
var package_start_counter = 0;
var bytesToRead = -1;
var valOut = 0;
var dataTx = [];
var dataCounter = 0;
var dataType_active = {};

var sensors = [];
var numSensors = 0;

// Client lists (socket connections)
var clientSocketList = [];
var clientSocketListID  = [];

// FROMFILE (data from stored file)
var FROMFILE = 0;

//##############################################################################
setup = function() {
    var server = new bb.Server(80, "./client_files", onconnect);  
    console.log("Start...\n");       

	// Create object with active data
    for(var key in dataType) {
        if(dataType[key].active == 1){
			dataType_active[key] = dataType[key];
        }
	}
    server.begin();    
    var os = require('os')

	var interfaces = os.networkInterfaces();
	var addresses = [];
	for (k in interfaces) {
		for (k2 in interfaces[k]) {
			var address = interfaces[k][k2];
			if (address.family == 'IPv4' && !address.internal) {
				addresses.push(address.address)
			}
		}
	}
	console.log("Access the webpage located on:")
	console.log(addresses)
	//console.log("Beginning Server");
};
//##############################################################################
// New serial data in

sp.on("data", function (data){
	newdata(data);	
});
//##############################################################################
var onconnect = function(socket) {  

	// Array of connected clients
	clientSocketList[socket.id] = socket;
	clientSocketListID[socket.id]  = socket.id;
	
	console.log("\n############################################################################");
	console.log("\nNew client connected ["+Object.size(clientSocketListID)+"]");
	console.log("\n############################################################################");
    
    // Transmit config (active datatypes only), name, min, max, freq, etc.
	socket.emit('config', "" + JSON.stringify(dataType_active));
   
   // On client to server incomming (control)
    socket.on('control', function(datatmp){
		var usercontrol = JSON.parse(datatmp);
		console.log(usercontrol.action+";"+usercontrol.val);
		sp.write("msg\n");
    });   
   
    // On client disconnect
    socket.on('disconnect', function() {
		delete clientSocketList[socket.id];
		delete clientSocketListID[socket.id];
		console.log("\n############################################################################");
		console.log("\nClient disconnected ["+Object.size(clientSocketListID)+"]");
		console.log("\n############################################################################");
    });
};
//##############################################################################
var txData = function(dataTxtmp){  
		// Transmitt to all clients (her kan vaelges lavere frekvens til eks. smartphones).
		for(var key in clientSocketList){
			clientSocketList[key].emit('data', "" + JSON.stringify(dataTxtmp));
		}		
};    
//##############################################################################
var newdata = function(data){
	
	// Data from serialport
	var datain = data;
	var startSequence = [255, 123, 10];

	for(var i=0; i<datain.length; i++){
		var currByte = datain[i]; // the current byte in the stream

		//console.log("("+currByte+")");
		
		// Search data pack. start sequence, if found then next byte is a type
		if((package_start_counter === 0) && (currByte === startSequence[0]))
			package_start_counter = 1;
		else if((package_start_counter === 1) && (currByte === startSequence[1]))
			package_start_counter = 2;
		else if((package_start_counter === 2) && (currByte === startSequence[2])){
			package_start_counter = 0;
			package_start = true;
			continue;
		}
				
		// Packet start found, get packet ID
		if (package_start){	
		
			// Reset
			package_start = false;
			bytesToRead = -1;
			valOut = 0;
			
			dataTypeKey = getDataType(dataType,currByte);
			
			// Valid data type found
			if(dataTypeKey !== -1){		
				bytesToRead = (dataType[dataTypeKey].datalength/8); // Bytes to read
			}
			else
				console.log("Invalid data (ID: "+currByte+")");
			continue;
		}			
		
		// Data bytes 
		if(bytesToRead > 0){	
			valOut = valOut + (currByte << (8*(bytesToRead-1)));	// Shift bytes
			bytesToRead -= 1; // Databyte counter
			continue;
		}	
	
		// No more data bytes, 
		if(bytesToRead === 0){
			var nameTerm = dataType[dataTypeKey].name.rpad(" ", 10); // Dette skal ikke ske
			var name = dataType[dataTypeKey].name;
			var  value = dataType[dataTypeKey].conv(valOut);
			value = Math.min(value, dataType[dataTypeKey].max);
			value = Math.max(value, dataType[dataTypeKey].min);

			var sensor = {
				name: name,
				val: value,
				timestamp: new Date().getTime()
			};
			sensors[numSensors++] = sensor;
		
			// Store the bytes
			if(dataType[dataTypeKey].active === 1){
				// Add to data pack
				dataTx[dataCounter++] = sensor;
				//console.log("ID:\t"+dataType[dataTypeKey].ID+"\tType:\t"+nameTerm+"\tData:\t"+value);
			}
			
			// Reset
			bytesToRead = -1;
			valOut = 0;
			
			// Next data byte ?
			dataTypeKey = getDataType(dataType,currByte);
			// Valid ?
			if(dataTypeKey !== -1){ 
				bytesToRead = (dataType[dataTypeKey].datalength/8);
			}
			 // No more data, transmit fetched data to client
			 // Pak data her, og kald dataTx
			else{
				// Tx data to all clients
				//console.log("Tx data -------------------------------------------------");
				txData(dataTx);
				dataCounter = 0;
				dataTx = [];

				// flush all collected data to db
				db.sensor.insert(sensors);
				sensors = [];
				numSensors = 0;
			}

		}
	}
};
//##############################################################################
// Data from stored file (can be used for testing)
if(FROMFILE == 1){
	var delay = 40;
	var index = 0;
	var storedData;
	var datalen = 0;

	// New stored data in
	var newStoredData = function(){
		// Resemple serial data
		var tmp = Array(storedData[index+0],storedData[index+1],
						storedData[index+2],storedData[index+3],
						storedData[index+4],storedData[index+5]);

		newdata(tmp);
		
		// Increase data index (from file)
		index = index + 6;
		if(index >= datalen)
			index = 0;
			
		setTimeout(newStoredData, delay);
	};

	fs.readFile('server_files/data.txt', function(err,datatmp){
		if(err) {
			console.error("Could not open file: %s", err);
			process.exit(1);
		}
		storedData = datatmp;
		datalen = storedData.length;
		setTimeout(newStoredData, delay);
	});
}
//##############################################################################

bb.run();

process.on('SIGINT', function(){
	console.log( "\ngracefully shutting down from  SIGINT (Crtl-C)" );

	db.close();

	process.exit()
});
