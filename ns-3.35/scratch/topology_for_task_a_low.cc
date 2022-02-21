/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2019 Universita' di Firenze, Italy
 *
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
 *
 * Author: Tommaso Pecorella <tommaso.pecorella@unifi.it>
 */
 
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/stats-module.h"
#include "ns3/mobility-module.h"

#include "ns3/internet-apps-module.h"
#include "ns3/mobility-module.h"
#include "ns3/spectrum-module.h"
#include "ns3/propagation-module.h"
#include "ns3/sixlowpan-module.h"
#include "ns3/lr-wpan-module.h"
#include "ns3/csma-module.h"

using namespace ns3;





// class MyApp : public Application
// {
// public:
//     MyApp();
//     virtual ~MyApp();

//     /**
//      * Register this type.
//      * \return The TypeId.
//      */
//     static TypeId GetTypeId(void);
//     void Setup(Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate);

// private:
//     virtual void StartApplication(void);
//     virtual void StopApplication(void);

//     void ScheduleTx(void);
//     void SendPacket(void);

//     Ptr<Socket> m_socket;
//     Address m_peer;
//     uint32_t m_packetSize;
//     uint32_t m_nPackets;
//     DataRate m_dataRate;
//     EventId m_sendEvent;
//     bool m_running;
//     uint32_t m_packetsSent;
// };

// MyApp::MyApp()
//     : m_socket(0),
//       m_peer(),
//       m_packetSize(0),
//       m_nPackets(0),
//       m_dataRate(0),
//       m_sendEvent(),
//       m_running(false),
//       m_packetsSent(0)
// {
// }

// MyApp::~MyApp()
// {
//     m_socket = 0;
// }

// /* static */
// TypeId MyApp::GetTypeId(void)
// {
//     static TypeId tid = TypeId("MyApp")
//                             .SetParent<Application>()
//                             .SetGroupName("Tutorial")
//                             .AddConstructor<MyApp>();
//     return tid;
// }

// void MyApp::Setup(Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate)
// {
//     m_socket = socket;
//     m_peer = address;
//     m_packetSize = packetSize;
//     m_nPackets = nPackets;
//     m_dataRate = dataRate;
// }

// void MyApp::StartApplication(void)
// {
//     m_running = true;
//     m_packetsSent = 0;
//     if (InetSocketAddress::IsMatchingType(m_peer))
//     {
//         m_socket->Bind();
//     }
//     else
//     {
//         m_socket->Bind6();
//     }
//     m_socket->Connect(m_peer);
//     SendPacket();
// }

// void MyApp::StopApplication(void)
// {
//     m_running = false;

//     if (m_sendEvent.IsRunning())
//     {
//         Simulator::Cancel(m_sendEvent);
//     }

//     if (m_socket)
//     {
//         m_socket->Close();
//     }
// }

// void MyApp::SendPacket(void)
// {
//     Ptr<Packet> packet = Create<Packet>(m_packetSize);
//     m_socket->Send(packet);

//     if (++m_packetsSent < m_nPackets)
//     {
//         ScheduleTx();
//     }
// }

// void MyApp::ScheduleTx(void)
// {
//     if (m_running)
//     {
//         Time tNext(Seconds(m_packetSize * 8 / static_cast<double>(m_dataRate.GetBitRate())));
//         m_sendEvent = Simulator::Schedule(tNext, &MyApp::SendPacket, this);
//     }
// }

// static void
// CwndChange(Ptr<OutputStreamWrapper> stream, uint32_t oldCwnd, uint32_t newCwnd)
// {
//     NS_LOG_UNCOND(Simulator::Now().GetSeconds() << "\t" << newCwnd);
//     *stream->GetStream() << Simulator::Now().GetSeconds() << "\t" << oldCwnd << "\t" << newCwnd << std::endl;
// }

// static void
// RxDrop(Ptr<PcapFileWrapper> file, Ptr<const Packet> p)
// {
//     NS_LOG_UNCOND("RxDrop at " << Simulator::Now().GetSeconds());
//     file->Write(Simulator::Now(), p);
// }

 
using namespace ns3;
 
int main (int argc, char** argv)
{
  
 
  uint32_t nLeftNodes=5;
  uint32_t nRightNodes=5;
  // uint32_t nCsmaNodes=2;
 
  NodeContainer LeftNodes;
  LeftNodes.Create (nLeftNodes);

  NodeContainer RightNodes;
  RightNodes.Create (nRightNodes);

  NodeContainer csmaNodes;
  csmaNodes.Add(LeftNodes.Get(0));
  csmaNodes.Add(RightNodes.Get(0));

  MobilityHelper LeftMobility;
  LeftMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  LeftMobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue(5.0),
                                  "MinY", DoubleValue(0.0),
                                  "DeltaX", DoubleValue(.5),
                                  "DeltaY", DoubleValue(1.2),
                                  "GridWidth", UintegerValue(2),
                                  "LayoutType", StringValue("RowFirst"));
  LeftMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  LeftMobility.Install(LeftNodes);

  MobilityHelper RightMobility;
  RightMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  RightMobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue(5.0),
                                  "MinY", DoubleValue(0.0),
                                  "DeltaX", DoubleValue(.5),
                                  "DeltaY", DoubleValue(1.2),
                                  "GridWidth", UintegerValue(2),
                                  "LayoutType", StringValue("RowFirst"));
  RightMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  RightMobility.Install(RightNodes);

  
  

  LrWpanHelper LeftLrWpanHelper;
  // Add and install the LrWpanNetDevice for each node
  NetDeviceContainer LeftLrwpanDevices = LeftLrWpanHelper.Install (LeftNodes);
 
  // Fake PAN association and short address assignment.
  // This is needed because the lr-wpan module does not provide (yet)
  // a full PAN association procedure.
  LeftLrWpanHelper.AssociateToPan (LeftLrwpanDevices, 0);

  LrWpanHelper RightLrWpanHelper;
  // Add and install the LrWpanNetDevice for each node
  NetDeviceContainer RightLrwpanDevices = RightLrWpanHelper.Install (RightNodes);
 
  // Fake PAN association and short address assignment.
  // This is needed because the lr-wpan module does not provide (yet)
  // a full PAN association procedure.
  RightLrWpanHelper.AssociateToPan (RightLrwpanDevices, 0);



  
 
  InternetStackHelper stack;
  stack.Install (LeftNodes);
  stack.Install (RightNodes);
 
 
  SixLowPanHelper LeftSixLowPanHelper;
  NetDeviceContainer LeftSixLowPanDevices = LeftSixLowPanHelper.Install (LeftLrwpanDevices);

  SixLowPanHelper RightSixLowPanHelper;
  NetDeviceContainer RightSixLowPanDevices = RightSixLowPanHelper.Install (RightLrwpanDevices);
 
  CsmaHelper csmaHelper;
  NetDeviceContainer csmaDevices = csmaHelper.Install (csmaNodes);
 
  Ipv6AddressHelper ipv6;

  ipv6.SetBase (Ipv6Address ("2001:cafe::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer LeftInterfaces;
  LeftInterfaces = ipv6.Assign (LeftSixLowPanDevices);
  LeftInterfaces.SetForwarding (0, true);
  LeftInterfaces.SetDefaultRouteInAllNodes(0);

  
  ipv6.SetBase (Ipv6Address ("2001:c001::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer RightInterfaces;
  RightInterfaces = ipv6.Assign (RightSixLowPanDevices);
  RightInterfaces.SetForwarding (0, true);
  RightInterfaces.SetDefaultRouteInAllNodes(0);

 
  ipv6.SetBase (Ipv6Address ("2001:f00d::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer csmaInterfaces;
  csmaInterfaces = ipv6.Assign (csmaDevices);
  csmaInterfaces.SetForwarding (1, true);
  csmaInterfaces.SetDefaultRouteInAllNodes (0);
 
  for (uint32_t i = 0; i < LeftSixLowPanDevices.GetN (); i++)
    {
      Ptr<NetDevice> dev = LeftSixLowPanDevices.Get (i);
      dev->SetAttribute ("UseMeshUnder", BooleanValue (true));
      dev->SetAttribute ("MeshUnderRadius", UintegerValue (10));
    }

  for (uint32_t i = 0; i < RightSixLowPanDevices.GetN (); i++)
    {
      Ptr<NetDevice> dev = RightSixLowPanDevices.Get (i);
      dev->SetAttribute ("UseMeshUnder", BooleanValue (true));
      dev->SetAttribute ("MeshUnderRadius", UintegerValue (10));
    }
 
  // uint32_t packetSize = 10;
  // uint32_t maxPacketCount = 5;
  // Time interPacketInterval = Seconds (1.);

 
  Simulator::Stop (Seconds (10));
 
  Simulator::Run ();
  Simulator::Destroy ();
 
}
 