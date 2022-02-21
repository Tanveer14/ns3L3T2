//Welcome to Technosilent.... Like & subscribe


//Network Animation using NetAnim in NS3
	
	1.PyViz // $ ./waf --run scratch/second --vis
	2. NetAnim // $./NetAnim

//Configuration
	Ubuntu 16.04 LTS
	ns-3.29
	netanim-3.108

// Step 1: Installation 
	//Prerequisites
	$ sudo apt-get install qt5-default
	//Build steps

	$ cd ns-allinone-3.29/netanim-3.108
	$ make clean
	$ qmake NetAnim.pro
	$ make

//Step 2: Add header file
	#include "ns3/netanim-module.h"

//Step 3: Add line above Simulator::Run();
	AnimationInterface anim ("filename.xml");

//Step 4: open NetAnim
	$ cd ns-allinone-3.29/netanim-3.108
	$ ./NetAnim

//Step 5: Load xml file and view animation

Kindly subscribe //Technosilent


















