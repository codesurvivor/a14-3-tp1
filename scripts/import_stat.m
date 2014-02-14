function [ stat ] = import_stat( ...
    nb_input, nb_output, fifo_depth, arbiter_mode, ...
    exec_time, stream_period)

stat = csvread( ...
    strcat('../data/noc_', int2str(nb_input), '_', ...
    int2str(nb_output), '_', int2str(fifo_depth), '_', ...
    int2str(arbiter_mode), '_', int2str(exec_time), '_', ...
    int2str(stream_period), '_simple_stat.data'));

end
