//Welcome to Technosilent.... Like & subscribe


//Analysis of Network using Flow Monitor in NS3


//Configuration
	Ubuntu 16.04 LTS
	ns-3.29

//Step 1:Add header file in our program

	#include "ns3/flow-monitor.h"
	#include "ns3/flow-monitor-helper.h"
	#include "ns3/flow-monitor-module.h"

//Step 2:Typical use in program
	
	Ptr<FlowMonitor> flowMonitor;
	FlowMonitorHelper flowHelper;
	flowMonitor = flowHelper.InstallAll();
	
	//Simulator::Stop (Seconds(stop_time));
	//Simulator::Run();
	
	flowMonitor->SerializeToXmlFile("TopologyForTaskA.xml", true, true);

//Step 3: Run the program
	./waf --run scratch/third

	Get thirdflow.xml


//Step 4: Analysis the parameters 

//first method using flowmon-parse-results.py

	find flowmon-parse-results.py // which is in src/flow-monitor/examples

	$ python flowmon-parse-results.py filename.xml

	Get the results

//Second method using NetAnim
	$ ./NetAnim



Kindly subscribe //Technosilent
 
Visit blog for free online course with certificate// https://freeonlinecoursesforu.blogspot.com/


