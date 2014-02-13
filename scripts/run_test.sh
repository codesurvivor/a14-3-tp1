#! /bin/sh

# ./bin/test/noc_test <nb_input> <nb_output> <arbiter_mode> <execution_time_ns> <stream_period>

mkdir -p data
cd data

for ((k=2; k<33; k=2*k));
do
  for ((j=2; j<65; j=2*j));
  do
    for ((i=0; i<5;i++));
    do
      echo "fifo depth:    $k";
      echo "arbiter mode:  $i";
      echo "stream period: $j";
      ../bin/test/noc_test 4 4 $k $i $1 $j;
    done
  done
done
