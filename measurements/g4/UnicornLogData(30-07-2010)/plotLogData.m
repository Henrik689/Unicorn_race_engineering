clc
clear

newData1 = importdata('logdata1.txt');

% Create new variables in the base workspace from those fields.
vars = fieldnames(newData1);
for i = 1:length(vars)
    assignin('base', vars{i}, newData1.(vars{i}));
end

plots = 4;

figure(1)
subplot(plots,1,1), plot(data(:,1),'linewidth',2), title('RPM'), ylim([0 12000])
subplot(plots,1,2), plot(data(:,6),'linewidth',2), title('Pot'), ylim([0 100])
subplot(plots,1,3), plot(data(:,3),'linewidth',2), title('Water temp moter'), ylim([20 90])
subplot(plots,1,4), plot(data(:,11),'linewidth',2), title('Water temp radiator'), ylim([20 70])
%
figure(2)
subplot(3,1,1), plot(data(:,1),'linewidth',2), title('RPM'), ylim([0 12000])
subplot(3,1,2), plot(data(:,6),'linewidth',2), title('Pot'), ylim([0 100])
subplot(3,1,3), plot(data(:,12),'linewidth',2), title('Oil temp'), ylim([20 60])
    
figure(3)
subplot(4,1,1), plot(data(:,1),'linewidth',2), title('RPM'), ylim([0 12000])
subplot(4,1,2), plot(data(:,6),'linewidth',2), title('Pot'), ylim([0 100])
subplot(4,1,3), plot(data(:,4),'linewidth',2), title('Manifold temp'), ylim([20 60])
subplot(4,1,4), plot(data(:,7)-100,'linewidth',2), title('Manifold press'), ylim([400 1200])