#!/bin/bash

#Default Value
#PacketsPerSecond = 300
#nNodes = 22
#numHalfFlows = 10
#MaxCoverageRange = 30 meter
#packetSize=1024

#PacketsPerSecond vary
# for i in {100..500..100}
# do
# ./waf --run "scratch/topology_for_task_a_low2 --packetsPerSecond=$i"
# done

#nNodes vary

# for i in {20..100..20}
# do
# ./waf --run "scratch/topology_for_task_a_low2 --nNodes=$i"
# done

# MaxCoverageRange vary

# for i in {1..5..1}
# do
# ./waf --run "scratch/topology_for_task_a_low2 --MaxCoverageRange=$i"
# done

# numHalfFlows vary

for i in {5..25..5}
do
./waf --run "scratch/topology_for_task_a_low2 --numHalfFlows=$i"
done

