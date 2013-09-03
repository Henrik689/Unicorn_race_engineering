pump_volume=0.24;
pump_rpm=1500;
pump_pressure=0.2;

r_rod=2.5;
r_piston=4;

stroke_clutch=20;

stroke_gear=30;

pv=pump_volume*10^-6/(60*1000);
pp=pump_pressure*100000;
pr=pump_rpm;

rr=2.5/1000;
rp=4/1000;

sc=20/1000;
sg=30/(2*1000);




force_gear_push = pp*rp^2*pi
force_gear_pull = (pp*(rp^2-rr^2)*pi)
force_clutch = pp*rp^2*pi

time_gear_push=(sg/2*rp^2*pi)/(pr*pv)
time_gear_pull=(sg/2*(rp^2*pi-rr^2*pi))/(pr*pv)
time_clutch=(sc/2*rp^2*pi)/(pr*pv)