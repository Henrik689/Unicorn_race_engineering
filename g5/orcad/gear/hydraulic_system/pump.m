close all
clear all

r_rod=0.0025;
r_piston=0.004;

l_clutch=20/1000;

l_gear=30/1000;

p_min=10;
p_max=400;

p=(p_min:p_max);


pump_vol_min=0.05;
pump_vol_max=1;

pump_min_rpm=1000;
pump_max_rpm=3000;

pump_vol=pump_vol_min:0.005:pump_vol_max;


for index=1:length(pump_vol)
   t_gear_push(index)=(l_gear/2*r_piston^2*pi)/(pump_max_rpm*pump_vol(index)*10^-6/(60*1000));
   t_gear_pull(index)=(l_gear/2*(r_piston^2*pi-r_rod^2*pi))/(pump_max_rpm*pump_vol(index)*10^-6/(60*1000));
   t_clutch(index)=(l_gear/2*r_piston^2*pi)/(pump_max_rpm*pump_vol(index)*10^-6/(60*1000));
end




subplot(2,1,1)
plot(pump_vol,t_gear_push,'c',pump_vol,t_gear_pull,'g')
axis([pump_vol_min pump_vol_max*1.1 0 max([max(t_gear_push) max(t_gear_pull)])])
ylabel('Time [ms]')
xlabel('Pump displacement')
grid on;
legend('Push','Pull');

subplot(2,1,2)
plot(pump_vol,t_clutch)
ylabel('Time [ms]')
xlabel('Pump displacement')
axis([pump_vol_min pump_vol_max*1.1 0 max(t_clutch)])
grid on