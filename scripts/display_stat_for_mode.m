function [ ] = display_stat_for_mode( ...
    nb_input, nb_output, arbiter_mode, exec_time)

%% Init.

x = zeros(5*6);
y = zeros(5*6);

life_time_max_z = zeros(5*6);
life_time_mean_z = zeros(5*6);
transit_time_max_z = zeros(5*6);
transit_time_mean_z = zeros(5*6);

current = 1;

%% Compute values.

% fifo_depth is 2^1 -> 2^5.
for fifo_depth_log=1:5
    % stream_period is 2^1 -> 2^6.
    for stream_period_log=1:6
        stat = import_stat( ...
            nb_input, nb_output, 2^fifo_depth_log, ...
            arbiter_mode, exec_time, 2^stream_period_log);
        
        x(current) = 2^fifo_depth_log;
        y(current) = 2^stream_period_log;
        
        life_time_max_z(current) = stat(1);
        life_time_mean_z(current) = stat(2);
        transit_time_max_z(current) = stat(3);
        transit_time_mean_z(current) = stat(4);
       
        current = current + 1;
    end
end

%% Prepare display.

mode_name = get_mode_name(arbiter_mode);

figure
s(1) = subplot(2,2,1);
s(2) = subplot(2,2,2);
s(3) = subplot(2,2,3);
s(4) = subplot(2,2,4);

%% Display life time.

stem3(s(1), x(:), y(:), life_time_max_z(:));

set(s(1),'xscal','log');
set(s(1),'yscal','log');
xlabel(s(1), 'fifo depth');
ylabel(s(1), 'stream period');
title(s(1), sprintf('packet maximal life time for %s mode', mode_name));

stem3(s(2), x(:), y(:), life_time_mean_z(:));

set(s(2),'xscal','log');
set(s(2),'yscal','log');
xlabel(s(2), 'fifo depth');
ylabel(s(2), 'stream period');
title(s(2), sprintf('packet mean life time for %s mode', mode_name));

%% Display transit time.

stem3(s(3), x(:), y(:), transit_time_max_z(:));

set(s(3),'xscal','log');
set(s(3),'yscal','log');
xlabel(s(3), 'fifo depth');
ylabel(s(3), 'stream period');
title(s(3), sprintf('packet maximal transit time for %s mode', mode_name));

stem3(s(4), x(:), y(:), transit_time_mean_z(:));

set(s(4),'xscal','log');
set(s(4),'yscal','log');
xlabel(s(4), 'fifo depth');
ylabel(s(4), 'stream period');
title(s(4), sprintf('packet mean transit time for %s mode', mode_name));


end
