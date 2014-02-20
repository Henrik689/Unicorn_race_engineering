//##############################################################################
// General

// Size of object
Object.size = function(obj) {
    var size = 0, key;
    for (key in obj) {
        if (obj.hasOwnProperty(key)) size++;
    }
    return size;
};

// Capitalize the first letter of string
function capitaliseFirstLetter(string)
{
    return string.charAt(0).toUpperCase() + string.slice(1);
}

//##############################################################################
// User input

// Append select element to select form 
function selectorAppend(selectorID,name)
{
    var newelement = document.createElement('option');
    newelement.text = ""+name;
    newelement.value = name;
    
    var selector = document.getElementById(selectorID);
    selector.add(newelement, null); 
}

//##############################################################################
// Data table
function updateTableData(table,row,column,newdata){
document.getElementById(table).rows[row].cells[column].innerHTML = newdata;
}

function createTable(rows,columns){
    var table = "";
    var columnWidth = (1150/columns).toFixed(0);
	
    table += '<table id="datatableID" border="0" frame="hsides" width="1150px">';
	
	
    for (i=1;i<=rows;i++){
        
        if (i % 2 === 1) 
             table += "<tr>"; 
        else 
            table += "<tr bgcolor = \"F6F6F6\" width=\""+columnWidth+"\">";	
 
        for (j=1;j<=columns;j++)    
            table +=  "<td width=\""+columnWidth+"px\">&nbsp;</td>" ;             
        table +=  "</tr>"; 
    }
    table +=  "</table>";
    return table;
}
//##############################################################################
// Data

// Setup data series, create one object for each dataseries/dataname
// Time and values are initialised to constant
function setupData2(config) {

    for(var i=0;i<config.length;i++) {
        data[i] = {};     
        data[i].name =  config[i].name; 
        data[i].points =  totalPoints; 
        data[i].time = fillTime(0,data[i].points); // Init time array
        data[i].val =  fillData(0,data[i].points); // Init value array
    }    
}

// Soft config array according to name (alphabetic sensor order).
function sortObject(tmpObject) {
    var sortedObject = {};
	var key;
	var a = [];
    
    for (key in tmpObject) {
        if (tmpObject.hasOwnProperty(key)) {
                a.push(key);
        }
    }

    a.sort();

    for (key = 0; key < a.length; key++) {
        sortedObject[a[key]] = tmpObject[a[key]];
    }
    return sortedObject;
}

function initDataSeries(config,points) {
    
    // Array of objects
    var dataseries_tmp = Array(); 
    var i = 0;
	
	config = sortObject(config);
    
    for(var key in config) {  
        
		// Create data object        
		dataseries_tmp[i] = {};  
		
		// Name of dataseries
		if(config[key].hasOwnProperty('name'))
			dataseries_tmp[i].name = config[key].name; 
		else 
			dataseries_tmp[i].name = 'Unknown data';
			
		// Set active to 0
		dataseries_tmp[i].plotactive = 0; 

		// Max value
		if(config[key].hasOwnProperty('max'))
			dataseries_tmp[i].max = config[key].max; 
		else 
			dataseries_tmp[i].max = 1000;
		
		// Min Value
		if(config[key].hasOwnProperty('min'))
			dataseries_tmp[i].min = config[key].min; 
		else 
			dataseries_tmp[i].min = 0;
			
		// Samplings frequncy
		if(config[key].hasOwnProperty('freq'))
			dataseries_tmp[i].freq = config[key].freq; 
		else 
			dataseries_tmp[i].freq = 1;
			
		// New data flag
		dataseries_tmp[i].newdata = 0;
		
		// Table position
		dataseries_tmp[i].tablerow = 0;
		dataseries_tmp[i].tablecol = 0;
		
		// Init data and time
		dataseries_tmp[i].time = fillTime(0,points); // Init time array
		dataseries_tmp[i].val =  fillData(0,points); // Init value array        
		
		i++;
		
    }
    return dataseries_tmp;
}    


// Fill empty data array
function fillData(val,points) {
    var res = [];
    for (var i = 0; i < points; ++i)
        res.push(val);
    return res;
}

// Fill empty time array
function fillTime(offset,points) {
    var res = [];
    for (var i = 0; i < points; ++i)
        res.push(i+offset);
    return res;
}

// Update data
function updateData(datatmp,newval) {
    datatmp.shift();
    datatmp.push(newval);
    return datatmp;
}  

// Data to flot
function flotifyData(array1,array2) {    
    if(array1.length == array2.length) {    
        var res = [];
        for (var i = 0; i < array1.length; ++i)
            res.push([array1[i], array2[i]]);
        return res;    
    }
    else
        return ([0,0]);
}  

//##############################################################################
// Flot plotting 

// Init flot plot object with the plot active dataseries
function initFlot(plot_tmp,name) {  
    
    var flotData = [];
	var labels = [];
	
	var min = 99999;
	var max = -99999;

	for(var key in plot_tmp.dataseries){
		if(plot_tmp.dataseries[key].plotactive == 1){
			
			// Append to flot data
			flotData.push(flotifyData(plot_tmp.dataseries[key].time,plot_tmp.dataseries[key].val));  
			labels.push(plot_tmp.dataseries[key].name);
			
			// Find max/min
			min = Math.min(plot_tmp.dataseries[key].min,min);
			max = Math.max(plot_tmp.dataseries[key].max,max);
		}
	}

	var plotname = "#placeholder"+name;
	var plot = $.plot($(plotname),flotData,setoptions(min,max,labels));
	return plot;
}
// Plot options
function setoptions(ymin,ymax,labels_tmp){
	var labelscount = 0;
	var	labels = labels_tmp;
	var options = {
		series:{shadowSize: 0,label:"1"}, // drawing is faster without shadows
		legend:{show: 1, 
				labelFormatter: function(label, series) {return labels[labelscount++];}, // Label hack, not how it is supposed to be done...
				position: "nw"},    
		yaxes: [{ min: ymin, max: ymax, position: "left"}],
		grid: { show: 1,
				color: 'black',
				backgroundColor: 'white',
				borderColor: 'black'},
		colors: ['red','blue','green','yellow',]
		//xaxes: [{ mode: 'time' }]
	};
	return options;
}