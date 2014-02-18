// OBS nogle object navne er lavet me smA, ved ikke om der kan ske noget ved det...

// Data from MCU
exports.empty = {
    name: 'Empty',												// Data name
	ID: 0,														// Data ID		
	active: 0,													// Data active ?
	rounddec: 2,												// Round to decimal
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},// Conv to human numbers
	datalength: 0,												// Lenght in bits
    max: 9999,													// Max exp. value
    min: 0														// Min exp. value	
};
exports.fuelPressure = {
    name: 'Fuel Press.',
	ID: 1,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.statusLapCount = {
    name: 'StatusLapCount',
	ID: 2,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.statusInjSum = {
    name: 'StatusInjSum',
	ID: 3,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.lastGearShift = {
    name: 'LastGearShift',
	ID: 4,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.motorOilTemp = {
    name: 'MotorOilTemp',
	ID: 5,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.oilPressure = {
    name: 'OilPressure',
	ID: 6,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.statusTime = {
    name: 'StatusTime',
	ID: 7,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.statusLapTime = {
    name: 'StatusLapTime',
	ID: 8,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.gearOilTemp = {
    name: 'GearOilTemp',
	ID: 9,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.statusTraction = {
    name: 'StatusTraction',
	ID: 10,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.statusGas = {
    name: 'StatusGas',
	ID: 11,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.statusLambdaV2 = {
    name: 'StatusLambdaV2',
	ID: 12,
	active: 0,
	rounddec: 2,
	conv: function (x) {
		if( x > 32768) x = -(65535 - x); 
		return (70-x/64).toFixed(this.rounddec);
	},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.statusCamTrigP1 = {
    name: 'StatusCamTrigP1',
	ID: 13,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.statusCamTrigP2 = {
    name: 'StatusCamTrigP2',
	ID: 14,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.statusChokerAdd = {
    name: 'StatusChokerAdd',
	ID: 15,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.statusLambdaPWM = {
    name: 'StatusLambdaPWM',
	ID: 16,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.waterTemp = {
    name: 'WaterMotor temp.',
	ID: 17,
	active: 1,
	rounddec: 0,
	conv: function (x) {return ((x*(-150.0/3840)+120)).toFixed(this.rounddec);},
	datalength: 16,
    max: 150,
    min: 0
};
exports.airTemp = {
    name: 'ManifoldAir temp.',
	ID: 18,
	active: 1,
	rounddec: 1,
	conv: function (x) {return (x*(-150.0/3840)+120).toFixed(this.rounddec);},
	datalength: 16,
    max: 120,
    min: 0
};
exports.potmeter = {
    name: 'Potmeter (0-100%)',
	ID: 19,
	active: 1,
	rounddec: 2,
	conv: function (x) {return ((x-336)/26.9).toFixed(this.rounddec);},
	datalength: 16,
    max: 100,
    min: 0
};
exports.rpm = {
    name: 'RPM',
	ID: 20,
	active: 1,
	rounddec: 0,
	conv: function (x) {return (x*0.9408).toFixed(this.rounddec);},
	datalength: 16,
    max: 15000,
    min: 0
};
exports.triggerErr = {
    name: 'TriggerErr',
	ID: 21,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.camAngle1 = {
    name: 'CamAngle1',
	ID: 22,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.camAngle2 = {
    name: 'CamAngle2',
	ID: 23,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.roadSpeed = {
    name: 'RoadSpeed (km/h)',
	ID: 24,
	active: 1,
	rounddec: 2,
	conv: function (x) {return (x*0.01).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.mapSensor = {
    name: 'Manifold press. (mBar)',
	ID: 25,
	active: 1,
	rounddec: 2,
	conv: function (x) {return (x*0.75).toFixed(this.rounddec);},
	datalength: 16,
    max: 1300,
    min: 0
};
exports.batteryV = {
    name: 'Batt. volt',
	ID: 26,
	active: 1,
	rounddec: 1,
	conv: function (x) {return (x*(1/210)+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 20,
    min: 0
};
exports.lambdaV = {
    name: 'Lambda (<1 => Rich)',
	ID: 27,
	active: 1,
	rounddec: 2,
	conv: function (x) {if(x>32768){x = -(65535-x);} return ((70-x/64)/100).toFixed(this.rounddec);},
	datalength: 16,
    max: 2,
    min: 0
};
exports.load = {
    name: 'Load',
	ID: 28,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.injectorTime = {
    name: 'InjectorTime',
	ID: 29,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (-0.75*x+120).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.ignitionTime = {
    name: 'IgnitionTime',
	ID: 30,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (-0.75*x+120).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.dwellTime = {
    name: 'DwellTime',
	ID: 31,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.gX = {
    name: 'GX',
	ID: 32,
	active: 1,
	rounddec: 2,
	conv: function (x) {if(x>32768){x = -(65535-x);} return (x*(1/16384)).toFixed(this.rounddec);},
	datalength: 16,
    max: 2,
    min: -2
};
exports.gY = {
    name: 'GY',
	ID: 33,
	active: 1,
	rounddec: 2,
	conv: function (x) {if(x>32768){x = -(65535-x);} return (x*(1/16384)).toFixed(this.rounddec);},
	datalength: 16,
    max: 2,
    min: -2
};
exports.gZ = {
    name: 'GZ',
	ID: 34,
	active: 1,
	rounddec: 2,
	conv: function (x) {if(x>32768){x = -(65535-x);} return (x*(1/16384)).toFixed(this.rounddec);},
	datalength: 16,
    max: 2,
    min: -2
};
exports.motorFlags = {
    name: 'MotorFlags',
	ID: 35,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 8,
    max: 9999,
    min: 0
};
exports.outBits = {
    name: 'OutBits',
	ID: 36,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 8,
    max: 9999,
    min: 0
};
exports.time = {
    name: 'Time',
	ID: 37,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 8,
    max: 9999,
    min: 0
};
exports.gearUp = {
    name: 'GearChange',
	ID: 38,
	active: 1,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 8,
    max: 9999,
    min: 0
};

exports.fWheelL = {
    name: 'FWheelL',
	ID: 39,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.fWheelR = {
    name: 'FWheelR',
	ID: 40,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.bWheelL = {
    name: 'BWheelL',
	ID: 41,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.bWheelR = {
    name: 'BWheelR',
	ID: 42,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.gearboardtemp = {
    name: 'GearBoard temp.',
	ID: 52,
	active: 1,
	rounddec: 2,
	conv: function (x) {
		Resistance=((10240000/(1024 - x)) - 10000);
		Temp = Math.log(Resistance);
		Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
		x = Temp - 273.15; 
		return x.toFixed(this.rounddec);
		},
	datalength: 16,
    max: 50,
    min: 0
};
exports.oilpress = {
    name: 'OilPress (0 = Low)',
	ID: 56,
	active: 1,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 1024,
    min: 0
};
exports.water_inlet = {
    name: 'WaterInlet temp.',
	ID: 57,
	active: 1,
	rounddec: 0,
	conv: function (x) {return (127.5*Math.exp(-0.003286*x)).toFixed(this.rounddec);},
	datalength: 16,
    max: 150,
    min: 0
};
exports.water_outlet = {
    name: 'WaterOutlet temp.',
	ID: 58,
	active: 1,
	rounddec: 0,
	conv: function (x) {return (127.5*Math.exp(-0.003286*x)).toFixed(this.rounddec);},
	datalength: 16,
    max: 150,
    min: 0
};

exports.gpssat = {
    name: 'GPS-sattelites',
	ID: 59,
	active: 1,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 8,
    max: 10,
    min: 0
};
exports.gpsspeed = {
    name: 'GPS-speed (km/h)',
	ID: 60,
	active: 1,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 120,
    min: 0
};
exports.gpstime_h = {
    name: 'GPS-time-h',
	ID: 61,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 25,
    min: 0
};
exports.gpstime_m = {
    name: 'GPS-time-m',
	ID: 62,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 61,
    min: 0
};
exports.gpstime_s = {
    name: 'GPS-time-s',
	ID: 63,
	active: 1,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 61,
    min: 0
};
exports.gear_neutral = {
    name: 'GearNeutral',
	ID: 64,
	active: 1,
	rounddec: 2,
	conv: function (x) {if(x>100){x=1;} return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 1,
    min: 0
};

exports.gear_est = {
    name: 'GearEst',
	ID: 65,
	active: 1,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 7,
    min: 0
};
exports.debug = {
    name: 'Debug',
	ID: 66,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
exports.valueIdLength = {
    name: 'ValueIdLength',
	ID: 67,
	active: 0,
	rounddec: 2,
	conv: function (x) {return (x*1+0).toFixed(this.rounddec);},
	datalength: 16,
    max: 9999,
    min: 0
};
