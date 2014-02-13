function [ name ] = get_mode_name( index )

vals = {'rand', 'LRU', 'FIFO', 'fixed', 'round robin'};
name = vals(index + 1);
name = name{1};

end

