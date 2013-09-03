var bb = require('../bonescript');
var fs = require('fs');
var io = require('socket.io');
var serialport = require('serialport');
var dataType = require('./sensor_config');
require('./server_files/server_functions');

// Serial port
var SerialPort = serialport.SerialPort; // localize object constructor
var baudrate = 115200;
var port = "/dev/tty.usbserial-A700eCo8";
var sp = new SerialPort(port, {
	parser: serialport.parsers.raw,
	baudrate: baudrate
});

// Global vars
var dataTypeKey = -1;
var package_start = new Boolean(false);
var package_start_counter = 0;
var bytesToRead = -1;
var valOut = 0;
var dataTx = Array();
var dataCounter = 0;
var dataType_active = {};

// Client lists (socket connections)
var clientSocketList = Array();
var clientSocketListID  = Array();

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

	for(var i=0;i<datain.length;i++){

		tmp = datain[i];

		//console.log("("+tmp+")");	
		
		// Search data pack. start sequence, if found then next byte is a type
		if((package_start_counter == 0) && (tmp == 255))
			package_start_counter = 1;
		else if((package_start_counter == 1) && (tmp == 123))
			package_start_counter = 2;
		else if((package_start_counter == 2) && (tmp == 10)){
			package_start_counter = 0;
			package_start = 1;
			continue;
		}
				
		// Packet start found, get packet ID
		if (package_start == 1){	
		
			// Reset
			package_start = 0;			
			bytesToRead = -1;
			valOut = 0;
			
			dataTypeKey = getDataType(dataType,tmp);
			
			// Valid data type found
			if(dataTypeKey != -1){		
				bytesToRead = (dataType[dataTypeKey].datalength/8); // Bytes to read
			}
			else
				console.log("Invalid data (ID: "+tmp+")");
			continue;
		}			
		
		// Data bytes 
		if(bytesToRead > 0){	
			valOut = valOut + (tmp << (8*(bytesToRead-1)));	// Shift bytes
			bytesToRead -= 1; // Databyte counter
			continue;
		}	
	
		// No more data bytes, 
		if(bytesToRead == 0){			
		
			// Store the bytes
			if(dataType[dataTypeKey].active == 1)
			{
				var nameTerm = dataType[dataTypeKey].name.rpad(" ", 10); // Dette skal ikke ske
				var name = dataType[dataTypeKey].name;
				var  value = dataType[dataTypeKey].conv(valOut);
				value = Math.min(value, dataType[dataTypeKey].max);
				value = Math.max(value, dataType[dataTypeKey].min);
				
				// Add to data pack.
				dataTx[dataCounter] = new Object();
				dataTx[dataCounter].name = name;
				dataTx[dataCounter].val = value;	
				dataCounter	++;
				
				console.log("ID:\t"+dataType[dataTypeKey].ID+"\tType:\t"+nameTerm+"\tData:\t"+value);	
			}
			
			// Reset
			bytesToRead = -1;	
			valOut = 0;
			
			// Next data byte ?
			dataTypeKey = getDataType(dataType,tmp);	
			// Valid ?
			if(dataTypeKey!=-1){ 
				bytesToRead = (dataType[dataTypeKey].datalength/8);				
			}
			 // No more data, transmit fetched data to client
			 // Pak data her, og kald dataTx
			else{
				// Tx data to all clients
				console.log("Tx data -------------------------------------------------");
				txData(dataTx);
				dataCounter = 0;
				dataTx = [];
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
