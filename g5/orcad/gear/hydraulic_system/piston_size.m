close all
clear all

p_min=10;
p_max=400;

m_gear=10;
m_clutch=40.5;
g=9.81;

r_rod_gear=0.005/2;

p=(p_min:p_max);

length(p)

for index=1:length(p)
    r_gear_push(index) = 1000*sqrt((m_gear*g)/((p(index)*100000)*pi));
    r_gear_pull(index) = 1000*sqrt(((m_gear*g)/((p(index)*100000)*pi))+r_rod_gear^2);
    r_clutch(index) = 1000*sqrt((m_clutch*g)/((p(index)*100000)*pi));
end



subplot(2,1,1)
plot(p,r_gear_push,'c',p,r_gear_pull,'g')
axis([p_min p_max+10 0 max([max(r_gear_push) max(r_gear_pull)])])
ylabel('Gear piston size [mm]')
xlabel('Hydraulic pressure')
grid on;
legend('Push','Pull');

subplot(2,1,2)
plot(p,r_clutch)
ylabel('Clutch piston size [mm]')
xlabel('Hydraulic pressure')
axis([p_min p_max+10 0 max(r_clutch)])
grid on