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
//       10.1.1.0        10.1.2.0
// n0 -------------- n1-------------n2
//    point-to-point    point-to-point
//
 
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;// a helper object to handle Node objects internally
  nodes.Create (3);//creates two node objects and those are stored internally

  NodeContainer n01 = NodeContainer( nodes.Get(0), nodes.Get(1) );
  NodeContainer n12 = NodeContainer( nodes.Get(1), nodes.Get(2) );


  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices01;
  devices01 = pointToPoint.Install (n01);//what does this refer to ?-> pointToPoint is a topology helper that helps in making config for net devices and establishes the point to point connection

  NetDeviceContainer devices12;
  devices12 = pointToPoint.Install (n12);

  InternetStackHelper stack;
  stack.Install (nodes); //installing the internet stack on each of the nodes in nodeContainer "nodes"
  
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");//setting the ip address network address and submask
 

  
  Ipv4InterfaceContainer interfaces01 = address.Assign (devices01);//each of the ip address, device pair gives an interface. These all are returned by the Assign method

  address.SetBase ("10.1.2.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces12 = address.Assign (devices12);//each of the ip address, device pair gives an interface. These all are returned by the Assign method
  
  UdpEchoServerHelper echoServer (9);//takes port number of the server to create a server

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));//takes the node to be declared as server , here 1, now the node is a server
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));



  UdpEchoClientHelper echoClient01 (interfaces01.GetAddress (1), 9);//takes the server interface(ip) address and the server port
  echoClient01.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient01.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient01.SetAttribute ("PacketSize", UintegerValue (1024));
  

  ApplicationContainer clientApps = echoClient01.Install (nodes.Get (0));//takes the node to be declared as client, here 0, now the node is a client
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient12 (interfaces12.GetAddress (0), 9);//takes the server interface(ip) address and the server port
  echoClient12.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient12.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient12.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps2 = echoClient12.Install (nodes.Get (2));//takes the node to be declared as client, here 0, now the node is a client
  clientApps2.Start (Seconds (2.0));
  clientApps2.Stop (Seconds (10.0));


  AsciiTraceHelper ascii;
  pointToPoint.EnableAsciiAll(ascii.CreateFileStream("myfirst.tr"));
  pointToPoint.EnablePcapAll("myfirst");

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
