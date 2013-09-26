To start server:

node main.js

(in linux, you might need root)

To change / setup new sensor data:
edit sensor_config.js

Requires:
node.js
dependencies:
socket.io (must be installed in the bonescript dir)
serialport
sqlite3

To run on raspberry pi the virtual com port drivers at http://www.ftdichip.com/Drivers/D2XX.htm must be installed.

when installed do:
modprobe ftdi_sio
echo ftdi_sio >> /etc/modules
