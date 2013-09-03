LIBRARY IEEE;
USE IEEE.std_logic_1164.all;

ENTITY SCHEMATIC1 IS 

END SCHEMATIC1;



ARCHITECTURE STRUCTURE OF SCHEMATIC1 IS

-- COMPONENTS

COMPONENT injector_honda
	PORT (
	\12V\ : INOUT std_logic;
	GND : INOUT std_logic
	); END COMPONENT;

COMPONENT ECU_fartstrup
	PORT (
	\Ignition #3\ : OUT std_logic;
	\GND Ignition\ : IN std_logic;
	\Fuel Pump Relay\ : OUT std_logic;
	\Idle/EGAS\ : INOUT std_logic;
	\Ignition #2\ : INOUT std_logic;
	\RPM meter\ : OUT std_logic;
	\Airflow +\ : IN std_logic;
	\Intake cam sensor\ : IN std_logic;
	\CAN-L 500kbaud\ : INOUT std_logic;
	\Lambda-\ : INOUT std_logic;
	\Knock sensor +\ : IN std_logic;
	\+5V reference\ : OUT std_logic;
	\RS232 RXD\ : IN std_logic;
	\GND Injection\ : IN std_logic;
	\Error/Shift light\ : OUT std_logic;
	\Injector #2\ : OUT std_logic;
	\Injector #3\ : OUT std_logic;
	KL30 : IN std_logic;
	\GND Shield\ : IN std_logic;
	\Ignition #4\ : OUT std_logic;
	\Ignition #1\ : OUT std_logic;
	\Idle/EGAS #2\ : OUT std_logic;
	Heater : OUT std_logic;
	\GND ECU\ : IN std_logic;
	\IP or TI\ : OUT std_logic;
	\Airflow -\ : IN std_logic;
	\KL15 (ign)\ : IN std_logic;
	\Lambda+\ : IN std_logic;
	\Speed driveline\ : IN std_logic;
	\GND Knock sensor\ : IN std_logic;
	\GND Cam sensor\ : IN std_logic;
	AUX : OUT std_logic;
	\Injector #1\ : OUT std_logic;
	\Exhaust vanos\ : OUT std_logic;
	\Injector #4\ : OUT std_logic;
	\Main power relay\ : OUT std_logic;
	KL87 : IN std_logic;
	\12V=ALS on\ : IN std_logic;
	\Left Speed input\ : IN std_logic;
	\Laptimer input\ : IN std_logic;
	\Wastegate or Intake vanos\ : OUT std_logic;
	\0V=Traction\ : IN std_logic;
	\EGAS trottle,AFR/KnockOut\ : INOUT std_logic;
	\Air temp\ : IN std_logic;
	\Water temp\ : IN std_logic;
	\MAP or Height sensor\ : IN std_logic;
	\Crank sensor +\ : IN std_logic;
	\GND Crank sensor\ : IN std_logic;
	\CAN-H 500kbaud\ : INOUT std_logic;
	\Exh CAM Sensor/Powershift\ : IN std_logic;
	\Right Speed input\ : IN std_logic;
	Potmeter : IN std_logic;
	\Rs meas\ : INOUT std_logic;
	\RS232 TXD\ : OUT std_logic
	); END COMPONENT;

COMPONENT lambda_probe_bosch
	PORT (
	Vs : INOUT std_logic;
	\Cal R\ : INOUT std_logic;
	\H+\ : INOUT std_logic;
	\H-\ : INOUT std_logic;
	\VS/IP\ : INOUT std_logic;
	IP : INOUT std_logic
	); END COMPONENT;

COMPONENT oil_pressure_bosch
	PORT (
	GND : INOUT std_logic;
	Output : INOUT std_logic;
	\5V\ : INOUT std_logic
	); END COMPONENT;

COMPONENT MAPIAT_bosch
	PORT (
	GND : INOUT std_logic;
	\NTC Thermistor\ : INOUT std_logic;
	\5V\ : INOUT std_logic;
	\Output signal\ : INOUT std_logic
	); END COMPONENT;

COMPONENT RESISTOR
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic
	); END COMPONENT;

COMPONENT network_node_unicorn
	PORT (
	\TX+ green/white\ : OUT std_logic;
	\TX- green\ : OUT std_logic;
	\RX+ orange/white\ : IN std_logic;
	\N/A blue\ : INOUT std_logic;
	\n/a blue/white\ : INOUT std_logic;
	\RX- orange\ : INOUT std_logic;
	\n/a brown/white\ : INOUT std_logic;
	\n/a brown\ : INOUT std_logic;
	\12V\ : IN std_logic;
	GND : IN std_logic;
	adc1 : IN std_logic;
	adc2 : IN std_logic;
	adc3 : IN std_logic;
	adc4 : IN std_logic;
	mosi : INOUT std_logic;
	miso : INOUT std_logic;
	reset : INOUT std_logic;
	scp : INOUT std_logic;
	clk : INOUT std_logic;
	\5V ref\ : INOUT std_logic;
	digi1 : INOUT std_logic;
	digi2 : INOUT std_logic;
	digi3 : INOUT std_logic;
	digi4 : INOUT std_logic
	); END COMPONENT;

COMPONENT watertemp_honda
	PORT (
	pin1 : INOUT std_logic;
	pin2 : INOUT std_logic;
	pin3 : INOUT std_logic
	); END COMPONENT;

COMPONENT throttle_honda
	PORT (
	\5V\ : IN std_logic;
	GND : IN std_logic;
	Output : OUT std_logic
	); END COMPONENT;

COMPONENT router_asus_unicorn
	PORT (
	\WAN TX+ green/white\ : OUT std_logic;
	\WAN TX- green\ : OUT std_logic;
	\WAN RX+ orange/white\ : IN std_logic;
	\WAN N/A blue\ : INOUT std_logic;
	\WAN n/a blue/white\ : INOUT std_logic;
	\WAN RX- orange\ : INOUT std_logic;
	\WAN n/a brown/white\ : INOUT std_logic;
	\WAN n/a brown\ : INOUT std_logic;
	\LAN1 TX+ green/white\ : OUT std_logic;
	\LAN1 TX- green\ : OUT std_logic;
	\LAN1 RX+ orange/white\ : IN std_logic;
	\LAN1 N/A blue\ : INOUT std_logic;
	\LAN1 n/a blue/white\ : INOUT std_logic;
	\LAN1 RX- orange\ : INOUT std_logic;
	\LAN1 n/a brown/white\ : INOUT std_logic;
	\LAN1 n/a brown\ : INOUT std_logic;
	\LAN2 TX+ green/white\ : OUT std_logic;
	\LAN2 TX- green\ : OUT std_logic;
	\LAN2 RX+ orange/white\ : IN std_logic;
	\LAN2 N/A blue\ : INOUT std_logic;
	\LAN2 n/a blue/white\ : INOUT std_logic;
	\LAN2 RX- orange\ : INOUT std_logic;
	\LAN2 n/a brown/white\ : INOUT std_logic;
	\LAN2 n/a brown\ : INOUT std_logic;
	\LAN3 TX+ green/white\ : OUT std_logic;
	\LAN3 TX- green\ : OUT std_logic;
	\LAN3 RX+ orange/white\ : IN std_logic;
	\LAN3 N/A blue\ : INOUT std_logic;
	\LAN3 n/a blue/white\ : INOUT std_logic;
	\LAN3 RX- orange\ : INOUT std_logic;
	\LAN3 n/a brown/white\ : INOUT std_logic;
	\LAN3 n/a brown\ : INOUT std_logic;
	\LAN4 TX+ green/white\ : OUT std_logic;
	\LAN4 TX- green\ : OUT std_logic;
	\LAN4 RX+ orange/white\ : IN std_logic;
	\LAN4 N/A blue\ : INOUT std_logic;
	\LAN4 n/a blue/white\ : INOUT std_logic;
	\LAN4 RX- orange\ : INOUT std_logic;
	\LAN4 n/a brown/white\ : INOUT std_logic;
	\LAN4 n/a brown\ : INOUT std_logic;
	\5V\ : IN std_logic;
	GND : IN std_logic;
	\rs232 tx\ : OUT std_logic;
	\rs232 rx\ : IN std_logic
	); END COMPONENT;

COMPONENT switch_cockpit
	PORT (
	pin1 : INOUT std_logic;
	pin2 : INOUT std_logic
	); END COMPONENT;

COMPONENT brake_over_threshold_switch
	PORT (
	pin1 : INOUT std_logic;
	pin2 : INOUT std_logic
	); END COMPONENT;

COMPONENT starter_relay_honda
	PORT (
	pin1 : INOUT std_logic;
	pin2 : INOUT std_logic;
	pin3 : INOUT std_logic;
	pin4 : INOUT std_logic
	); END COMPONENT;

COMPONENT master_switch
	PORT (
	Battery : INOUT std_logic;
	pin1 : INOUT std_logic;
	pin2 : INOUT std_logic;
	Supply : INOUT std_logic;
	pin3 : INOUT std_logic;
	pin4 : INOUT std_logic
	); END COMPONENT;

COMPONENT starter_switch
	PORT (
	pin1 : INOUT std_logic;
	pin2 : INOUT std_logic
	); END COMPONENT;

COMPONENT BATTERY
	PORT (
	\+\ : INOUT std_logic;
	\-\ : INOUT std_logic
	); END COMPONENT;

COMPONENT Fuel_pump
	PORT (
	\12V\ : IN std_logic;
	GND : IN std_logic
	); END COMPONENT;

COMPONENT \Fuel relay\
	PORT (
	\Control signal - 85\ : INOUT std_logic;
	\GND - 86\ : INOUT std_logic;
	\Consumers - 87\ : INOUT std_logic;
	\12V - 30\ : INOUT std_logic
	); END COMPONENT;

COMPONENT starter_honda
	PORT (
	\12V\ : IN std_logic
	); END COMPONENT;

COMPONENT speed_honda
	PORT (
	\5V\ : IN std_logic;
	HALL : OUT std_logic;
	GND : IN std_logic
	); END COMPONENT;

COMPONENT gear_neutral_honda
	PORT (
	pin : INOUT std_logic
	); END COMPONENT;

COMPONENT water_blower_honda
	PORT (
	\12V\ : IN std_logic;
	GND : IN std_logic
	); END COMPONENT;

COMPONENT generator_honda
	PORT (
	pin1 : INOUT std_logic;
	pin2 : INOUT std_logic;
	pin3 : INOUT std_logic
	); END COMPONENT;

COMPONENT generator_relay_honda
	PORT (
	Red1 : OUT std_logic;
	Red2 : OUT std_logic;
	Green1 : OUT std_logic;
	Green2 : OUT std_logic;
	Yellow1 : INOUT std_logic;
	Yellow2 : INOUT std_logic;
	Yellow3 : INOUT std_logic
	); END COMPONENT;

COMPONENT cranksensor_honda
	PORT (
	\VR+\ : INOUT std_logic;
	\VR-\ : INOUT std_logic
	); END COMPONENT;

COMPONENT cam_sensor_honda
	PORT (
	\5V\ : IN std_logic;
	GND : IN std_logic;
	Output : OUT std_logic
	); END COMPONENT;

COMPONENT ignition_coil_honda
	PORT (
	\12V\ : INOUT std_logic;
	GND : INOUT std_logic
	); END COMPONENT;

-- SIGNALS

SIGNAL N78843 : std_logic;
SIGNAL N109326 : std_logic;
SIGNAL N115911 : std_logic;
SIGNAL N80051 : std_logic;
SIGNAL N88847 : std_logic;
SIGNAL N46573 : std_logic;
SIGNAL N79802 : std_logic;
SIGNAL N79560 : std_logic;
SIGNAL N46272 : std_logic;
SIGNAL N42301 : std_logic;
SIGNAL N54758 : std_logic;
SIGNAL N56104 : std_logic;
SIGNAL N41944 : std_logic;
SIGNAL N55419 : std_logic;
SIGNAL N49338 : std_logic;
SIGNAL N93145 : std_logic;
SIGNAL N81341 : std_logic;
SIGNAL N74804 : std_logic;
SIGNAL N50306 : std_logic;
SIGNAL N44456 : std_logic;
SIGNAL N42682 : std_logic;
SIGNAL N60215 : std_logic;
SIGNAL N107761 : std_logic;
SIGNAL \0\ : std_logic;
SIGNAL N49600 : std_logic;
SIGNAL N39566 : std_logic;
SIGNAL N51628 : std_logic;
SIGNAL N44670 : std_logic;
SIGNAL N51322 : std_logic;
SIGNAL N61472 : std_logic;
SIGNAL N50878 : std_logic;
SIGNAL N39693 : std_logic;
SIGNAL N46816 : std_logic;
SIGNAL N81077 : std_logic;
SIGNAL N41611 : std_logic;
SIGNAL N39607 : std_logic;
SIGNAL N80816 : std_logic;
SIGNAL N80558 : std_logic;
SIGNAL N63948 : std_logic;
SIGNAL N80303 : std_logic;
SIGNAL \ECU IGNITON\ : std_logic;
SIGNAL N117208 : std_logic;
SIGNAL N120692 : std_logic;
SIGNAL N116865 : std_logic;
SIGNAL N117384 : std_logic;
SIGNAL N116377 : std_logic;
SIGNAL GND : std_logic;
SIGNAL \12V\ : std_logic;
SIGNAL N124070 : std_logic;
SIGNAL \STARTER SUPPLY\ : std_logic;
SIGNAL N117563 : std_logic;
SIGNAL N127592 : std_logic;
SIGNAL \RS232 TX\ : std_logic;
SIGNAL N111577 : std_logic;
SIGNAL N134102 : std_logic;
SIGNAL N109835 : std_logic;
SIGNAL N111434 : std_logic;
SIGNAL N109735 : std_logic;
SIGNAL \BLOWER SUPPLY\ : std_logic;
SIGNAL N117035 : std_logic;
SIGNAL N111294 : std_logic;
SIGNAL N111157 : std_logic;
SIGNAL N111027 : std_logic;
SIGNAL N110895 : std_logic;
SIGNAL N110766 : std_logic;
SIGNAL N110644 : std_logic;
SIGNAL N121909 : std_logic;
SIGNAL \BLOWER SIGNAL\ : std_logic;
SIGNAL N110522 : std_logic;
SIGNAL N110400 : std_logic;
SIGNAL N110281 : std_logic;
SIGNAL \RS232 RX\ : std_logic;
SIGNAL N110165 : std_logic;
SIGNAL N110052 : std_logic;
SIGNAL N109942 : std_logic;
SIGNAL N130382 : std_logic;
SIGNAL \FUEL SIGNAL\ : std_logic;
SIGNAL N116698 : std_logic;
SIGNAL N116534 : std_logic;
SIGNAL \LAN CABLE\ : std_logic;
SIGNAL \GENERATOR - RED1\ : std_logic;
SIGNAL \GENERATOR - RED2\ : std_logic;

-- INSTANCE ATTRIBUTES



-- GATE INSTANCES

BEGIN
U45 : injector_honda	PORT MAP(
	\12V\ => \12V\, 
	GND => N41611
);
U46 : ECU_fartstrup	PORT MAP(
	\Ignition #3\ => N46816, 
	\GND Ignition\ => GND, 
	\Fuel Pump Relay\ => \FUEL SIGNAL\, 
	\Idle/EGAS\ => \BLOWER SIGNAL\, 
	\Ignition #2\ => N46573, 
	\RPM meter\ => OPEN, 
	\Airflow +\ => 'Z', 
	\Intake cam sensor\ => N49338, 
	\CAN-L 500kbaud\ => OPEN, 
	\Lambda-\ => N50878, 
	\Knock sensor +\ => 'Z', 
	\+5V reference\ => N49600, 
	\RS232 RXD\ => \RS232 RX\, 
	\GND Injection\ => GND, 
	\Error/Shift light\ => OPEN, 
	\Injector #2\ => N42301, 
	\Injector #3\ => N42682, 
	KL30 => 'Z', 
	\GND Shield\ => GND, 
	\Ignition #4\ => N44456, 
	\Ignition #1\ => N44670, 
	\Idle/EGAS #2\ => OPEN, 
	Heater => N51628, 
	\GND ECU\ => GND, 
	\IP or TI\ => N51322, 
	\Airflow -\ => 'Z', 
	\KL15 (ign)\ => \ECU IGNITON\, 
	\Lambda+\ => N50306, 
	\Speed driveline\ => N63948, 
	\GND Knock sensor\ => GND, 
	\GND Cam sensor\ => GND, 
	AUX => N60215, 
	\Injector #1\ => N41944, 
	\Exhaust vanos\ => OPEN, 
	\Injector #4\ => N41611, 
	\Main power relay\ => OPEN, 
	KL87 => \12V\, 
	\12V=ALS on\ => 'Z', 
	\Left Speed input\ => 'Z', 
	\Laptimer input\ => 'Z', 
	\Wastegate or Intake vanos\ => OPEN, 
	\0V=Traction\ => 'Z', 
	\EGAS trottle,AFR/KnockOut\ => OPEN, 
	\Air temp\ => N54758, 
	\Water temp\ => N93145, 
	\MAP or Height sensor\ => N56104, 
	\Crank sensor +\ => N61472, 
	\GND Crank sensor\ => GND, 
	\CAN-H 500kbaud\ => OPEN, 
	\Exh CAM Sensor/Powershift\ => N88847, 
	\Right Speed input\ => 'Z', 
	Potmeter => N107761, 
	\Rs meas\ => OPEN, 
	\RS232 TXD\ => \RS232 TX\
);
U47 : lambda_probe_bosch	PORT MAP(
	Vs => N50306, 
	\Cal R\ => OPEN, 
	\H+\ => \12V\, 
	\H-\ => N51628, 
	\VS/IP\ => N50878, 
	IP => N51322
);
U48 : oil_pressure_bosch	PORT MAP(
	GND => GND, 
	Output => N60215, 
	\5V\ => N55419
);
U49 : MAPIAT_bosch	PORT MAP(
	GND => GND, 
	\NTC Thermistor\ => N54758, 
	\5V\ => N55419, 
	\Output signal\ => N56104
);
\11W effekt\ : RESISTOR	PORT MAP(
	\1\ => N130382, 
	\2\ => GND
);
U50 : network_node_unicorn	PORT MAP(
	\TX+ green/white\ => N79560, 
	\TX- green\ => N79802, 
	\RX+ orange/white\ => N80051, 
	\N/A blue\ => N80303, 
	\n/a blue/white\ => N80558, 
	\RX- orange\ => N80816, 
	\n/a brown/white\ => N81077, 
	\n/a brown\ => N81341, 
	\12V\ => \12V\, 
	GND => GND, 
	adc1 => 'Z', 
	adc2 => 'Z', 
	adc3 => 'Z', 
	adc4 => 'Z', 
	mosi => OPEN, 
	miso => OPEN, 
	reset => OPEN, 
	scp => OPEN, 
	clk => OPEN, 
	\5V ref\ => OPEN, 
	digi1 => N74804, 
	digi2 => N88847, 
	digi3 => OPEN, 
	digi4 => OPEN
);
U51 : watertemp_honda	PORT MAP(
	pin1 => \12V\, 
	pin2 => N93145, 
	pin3 => OPEN
);
U52 : throttle_honda	PORT MAP(
	\5V\ => N55419, 
	GND => GND, 
	Output => N107761
);
U53 : router_asus_unicorn	PORT MAP(
	\WAN TX+ green/white\ => OPEN, 
	\WAN TX- green\ => OPEN, 
	\WAN RX+ orange/white\ => 'Z', 
	\WAN N/A blue\ => OPEN, 
	\WAN n/a blue/white\ => OPEN, 
	\WAN RX- orange\ => OPEN, 
	\WAN n/a brown/white\ => OPEN, 
	\WAN n/a brown\ => OPEN, 
	\LAN1 TX+ green/white\ => OPEN, 
	\LAN1 TX- green\ => OPEN, 
	\LAN1 RX+ orange/white\ => 'Z', 
	\LAN1 N/A blue\ => OPEN, 
	\LAN1 n/a blue/white\ => OPEN, 
	\LAN1 RX- orange\ => OPEN, 
	\LAN1 n/a brown/white\ => OPEN, 
	\LAN1 n/a brown\ => OPEN, 
	\LAN2 TX+ green/white\ => OPEN, 
	\LAN2 TX- green\ => OPEN, 
	\LAN2 RX+ orange/white\ => 'Z', 
	\LAN2 N/A blue\ => OPEN, 
	\LAN2 n/a blue/white\ => OPEN, 
	\LAN2 RX- orange\ => OPEN, 
	\LAN2 n/a brown/white\ => OPEN, 
	\LAN2 n/a brown\ => OPEN, 
	\LAN3 TX+ green/white\ => N117563, 
	\LAN3 TX- green\ => N117384, 
	\LAN3 RX+ orange/white\ => N117208, 
	\LAN3 N/A blue\ => N117035, 
	\LAN3 n/a blue/white\ => N116865, 
	\LAN3 RX- orange\ => N116698, 
	\LAN3 n/a brown/white\ => N116534, 
	\LAN3 n/a brown\ => N116377, 
	\LAN4 TX+ green/white\ => N111577, 
	\LAN4 TX- green\ => N111434, 
	\LAN4 RX+ orange/white\ => N111294, 
	\LAN4 N/A blue\ => N111157, 
	\LAN4 n/a blue/white\ => N111027, 
	\LAN4 RX- orange\ => N110895, 
	\LAN4 n/a brown/white\ => N110766, 
	\LAN4 n/a brown\ => N110644, 
	\5V\ => \12V\, 
	GND => GND, 
	\rs232 tx\ => \RS232 TX\, 
	\rs232 rx\ => \RS232 RX\
);
U54 : network_node_unicorn	PORT MAP(
	\TX+ green/white\ => N109735, 
	\TX- green\ => N109835, 
	\RX+ orange/white\ => N109942, 
	\N/A blue\ => N110052, 
	\n/a blue/white\ => N110165, 
	\RX- orange\ => N110281, 
	\n/a brown/white\ => N110400, 
	\n/a brown\ => N110522, 
	\12V\ => \12V\, 
	GND => GND, 
	adc1 => 'Z', 
	adc2 => 'Z', 
	adc3 => 'Z', 
	adc4 => 'Z', 
	mosi => OPEN, 
	miso => OPEN, 
	reset => OPEN, 
	scp => OPEN, 
	clk => OPEN, 
	\5V ref\ => OPEN, 
	digi1 => OPEN, 
	digi2 => OPEN, 
	digi3 => OPEN, 
	digi4 => OPEN
);
U55 : switch_cockpit	PORT MAP(
	pin1 => \ECU IGNITON\, 
	pin2 => N120692
);
U56 : brake_over_threshold_switch	PORT MAP(
	pin1 => N120692, 
	pin2 => N127592
);
U57 : starter_relay_honda	PORT MAP(
	pin1 => N124070, 
	pin2 => GND, 
	pin3 => \STARTER SUPPLY\, 
	pin4 => \12V\
);
U58 : master_switch	PORT MAP(
	Battery => N121909, 
	pin1 => N130382, 
	pin2 => OPEN, 
	Supply => \12V\, 
	pin3 => OPEN, 
	pin4 => N127592
);
U59 : starter_switch	PORT MAP(
	pin1 => \ECU IGNITON\, 
	pin2 => N124070
);
\12V\ : BATTERY	PORT MAP(
	\+\ => GND, 
	\-\ => N121909
);
U60 : Fuel_pump	PORT MAP(
	\12V\ => N134102, 
	GND => GND
);
U61 : \Fuel relay\	PORT MAP(
	\Control signal - 85\ => \FUEL SIGNAL\, 
	\GND - 86\ => GND, 
	\Consumers - 87\ => N134102, 
	\12V - 30\ => \12V\
);
U62 : \Blower relay\	PORT MAP(
	\Control signal - 85\ => \BLOWER SIGNAL\, 
	\GND - 86\ => GND, 
	\Consumers - 87\ => \BLOWER SUPPLY\, 
	\12V - 30\ => \12V\
);
U63 : starter_honda	PORT MAP(
	\12V\ => \STARTER SUPPLY\
);
U31 : speed_honda	PORT MAP(
	\5V\ => N55419, 
	HALL => N63948, 
	GND => GND
);
U32 : gear_neutral_honda	PORT MAP(
	pin => N74804
);
U64 : water_blower_honda	PORT MAP(
	\12V\ => \BLOWER SUPPLY\, 
	GND => GND
);
U34 : generator_honda	PORT MAP(
	pin1 => N39566, 
	pin2 => N39607, 
	pin3 => N39693
);
U35 : generator_relay_honda	PORT MAP(
	Red1 => \GENERATOR - RED1\, 
	Red2 => \GENERATOR - RED2\, 
	Green1 => \0\, 
	Green2 => \0\, 
	Yellow1 => N39566, 
	Yellow2 => N39607, 
	Yellow3 => N39693
);
U36 : cranksensor_honda	PORT MAP(
	\VR+\ => N61472, 
	\VR-\ => GND
);
U37 : cam_sensor_honda	PORT MAP(
	\5V\ => N49600, 
	GND => GND, 
	Output => N49338
);
U38 : ignition_coil_honda	PORT MAP(
	\12V\ => \12V\, 
	GND => N44670
);
U39 : ignition_coil_honda	PORT MAP(
	\12V\ => \12V\, 
	GND => N46272
);
U40 : ignition_coil_honda	PORT MAP(
	\12V\ => \12V\, 
	GND => N46816
);
U41 : ignition_coil_honda	PORT MAP(
	\12V\ => \12V\, 
	GND => N44456
);
U42 : injector_honda	PORT MAP(
	\12V\ => \12V\, 
	GND => N41944
);
U43 : injector_honda	PORT MAP(
	\12V\ => \12V\, 
	GND => N42301
);
U44 : injector_honda	PORT MAP(
	\12V\ => \12V\, 
	GND => N42682
);
END STRUCTURE;

