#!/bin/bash

#Default Value
#Nodes = 10
#Flow = 10
#TxArea = 10 meter
#Pps = 500

#Keeping Nodes variable and others as default

# for i in {5..25..5}
# do
# ./waf --run "scratch/topology_for_task_a --nWifi=$i"
# input="node_throughput.txt"
# while IFS= read -r line
# do
# echo "$line">>"nodeChangeThroughput.txt"
# done < "$input"
# done

#Keeping TxArea Variable and others As default

# for i in {1..5}
# do
# ./waf --run "scratch/topology_for_task_a --txArea=$i"
# input="area_throughput.txt"
# while IFS= read -r line
# do
# echo "$line">>"AreaChangeThroughput.txt"
# done < "$input"
# done

#Keeping PPS Variable and others As default

for i in {100..500..100}
do
./waf --run "scratch/topology_for_task_a --packetsPerSecond=$i"
done

#Keeping FLow Variable and others As default

# for i in {10..50..10}
# do
# ./waf --run "scratch/topology_for_task_a --nWifi=15 --num_half_flows=$i" 