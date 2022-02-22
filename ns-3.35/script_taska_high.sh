#!/bin/bash

#Default Value
#PacketsPerSecond = 300
#totalNodes = 22
#numHalfFlows = 10
#MaxCoverageRange = 30 meter
#packetSize=1024

#PacketsPerSecond vary--done

# for i in {100..500..100}
# do
# ./waf --run "scratch/topology_for_task_a_high --packetsPerSecond=$i"
# done

#totalNodes vary

# for i in {20..100..20}
# do
# ./waf --run "scratch/topology_for_task_a_high --totalNodes=$i"
# done

# MaxCoverageRange vary

for i in {1..5..1}
do
./waf --run "scratch/topology_for_task_a_high --MaxCoverageRange=$i"
done

# numHalfFlows vary--done

# for i in {5..25..5}
# do
# ./waf --run "scratch/topology_for_task_a_high --numHalfFlows=$i"
# done

