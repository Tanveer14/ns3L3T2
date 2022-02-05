/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//
 
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);

  //enables log level info for echo client and echo server application
  //This will result in the application printing out messages as packets are sent and received during the simulation.
//  there are a number of levels of
// logging verbosity/detail that you can enable on each component. These two lines of code enable debug logging at the
// INFO level for echo clients and servers.
// error,ward,debug,program process info these will be printed in console
// log deals with console printing and other printing mechanisms
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  InternetStackHelper stack;
  stack.Install (nodes);
//   When it is executed, it will
// install an Internet Stack (TCP, UDP, IP, etc.) on each of the nodes in the node container.

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

// The first line of code in the above snippet declares the UdpEchoServerHelper . As usual, this isn’t the application
// itself, it is an object used to help us create the actual applications. One of our conventions is to place required
// Attributes in the helper constructor. In this case, the helper can’t do anything useful unless it is provided with a
// port number that the client also knows about. Rather than just picking one and hoping it all works out, we require the
// port number as a parameter to the constructor. The constructor, in turn, simply does a SetAttribute with the passed
// value. If you want, you can set the “Port” Attribute to another value later using SetAttribute .
// Similar to many other helper objects, the UdpEchoServerHelper object has an Install method. It is the execution
// of this method that actually causes the underlying echo server application to be instantiated and attached to a node.
// Interestingly, the Install method takes a NodeContainter as a parameter just as the other Install methods

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  // previously in the Start and Stop part, we only scheduled when the things will happen
  // now we are actually running the simulation

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
