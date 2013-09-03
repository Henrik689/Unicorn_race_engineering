// Size of object
Object.size = function(obj) {
    var size = 0, key;
    for (key in obj) {
        if (obj.hasOwnProperty(key)) size++;
    }
    return size;
};

// Get data type from list by ID
getDataType = function(tmpDataCfg,ID){

	for (var key in tmpDataCfg){
		if(tmpDataCfg[key].ID == ID)
			return key; // ID found
	}	
	// ID not found
	return -1; 
}

// Padding right
String.prototype.rpad = function(padString, length) {
	var str = this;
    while (str.length < length)
        str = str + padString;
    return str;
}