%clc,clear


%% Wheel speed maks speed

% Max antal pulser pr sek = 2000

% Pulser pr omgang
wheel_puls = 56;

% Diameter
wheel_d = 0.508; % [m]

% Omkreds
wheel_o = wheel_d*pi;

% Maks speed
maks_speed = 180/3.6 % [m/s], 180 km/t

% Maks rpm
wheel_rmp_max = maks_speed / wheel_o

% Maks pulser
maks_puls = wheel_rmp_max * wheel_puls

%% Wheel speed min speed

min_speed = 2.16/3.6 % [m/s], 2.16 km/t 
wheel_rmp_min = min_speed / wheel_o
min_puls = wheel_rmp_min * wheel_puls

%% mcu counter calc

min_freq = 1/min_puls %% tælleren skal tælle til 65.000 på denne tid
counter_pr_sek = (2^16)/min_freq

cpu_freq = 11059200;

prescaler = 11059200 / counter_pr_sek

test = (((11059200 / 8)) / x) * (wheel_o/50) * 3.6