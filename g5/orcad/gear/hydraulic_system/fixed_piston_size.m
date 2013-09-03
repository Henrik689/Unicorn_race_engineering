close all
clear all

r_rod=0.0025;
r_piston=0.004;

p_min=10;
p_max=400;

p=(p_min:p_max);


for index=1:length(p)
   f_gear_push(index) = p(index)*100000*r_piston^2*pi;
   f_gear_pull(index) = (p(index)*100000*(r_piston^2-r_rod^2)*pi);
   f_clutch(index) = p(index)*100000*r_piston^2*pi;
end


subplot(2,1,1)
plot(p,f_gear_push,'c',p,f_gear_pull,'g')
axis([p_min p_max+10 0 max([max(f_gear_push) max(f_gear_pull)])])
ylabel('Force [N]')
xlabel('Hydraulic pressure')
grid on;
legend('Push','Pull');

subplot(2,1,2)
plot(p,f_clutch)
ylabel('Force [N]')
xlabel('Hydraulic pressure')
axis([p_min p_max+10 0 max(f_clutch)])
grid on