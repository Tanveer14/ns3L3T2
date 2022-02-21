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
#include <iostream>
#include <string>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/flow-monitor-module.h"

// Default Network Topology
//
//   Wifi 10.1.3.0
//                 AP
//  *    *    *    *
//  |    |    |    |    10.1.1.0
// n2   n3   n4   n0 -------------- n1   n5   
//                   point-to-point  |    |      
//                                   *    *     
//                                   AP
//                                      Wifi 10.1.2.0 (P)

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TopologyForTaskA");


double simulationTime=25.0;


class MyApp : public Application 
{
public:

  MyApp ();
  virtual ~MyApp();

  void Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate);

private:
  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void ScheduleTx (void);
  void SendPacket (void);

  Ptr<Socket>     m_socket;
  Address         m_peer;
  uint32_t        m_packetSize;
  uint32_t        m_nPackets;
  DataRate        m_dataRate;
  EventId         m_sendEvent;
  bool            m_running;
  uint32_t        m_packetsSent;
};

MyApp::MyApp ()
  : m_socket (0), 
    m_peer (), 
    m_packetSize (0), 
    m_nPackets (0), 
    m_dataRate (0), 
    m_sendEvent (), 
    m_running (false), 
    m_packetsSent (0)
{
}

MyApp::~MyApp()
{
  m_socket = 0;
}

void
MyApp::Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t nPackets, DataRate dataRate)
{
  m_socket = socket;
  m_peer = address;
  m_packetSize = packetSize;
  m_nPackets = nPackets;
  m_dataRate = dataRate;
}

void
MyApp::StartApplication (void)
{
  m_running = true;
  m_packetsSent = 0;
  m_socket->Bind ();
  m_socket->Connect (m_peer);
  SendPacket ();
}

void 
MyApp::StopApplication (void)
{
  m_running = false;

  if (m_sendEvent.IsRunning ())
    {
      Simulator::Cancel (m_sendEvent);
    }

  if (m_socket)
    {
      m_socket->Close ();
    }
}

void 
MyApp::SendPacket (void)
{
  Ptr<Packet> packet = Create<Packet> (m_packetSize);
  m_socket->Send (packet);

  Time currTime=(Simulator :: Now());
  if( currTime.GetSeconds() < simulationTime){
      ScheduleTx();
  }  
}

void 
MyApp::ScheduleTx (void)
{
  if (m_running)
    {
      Time tNext (Seconds (m_packetSize * 8 / static_cast<double> (m_dataRate.GetBitRate ())));
      m_sendEvent = Simulator::Schedule (tNext, &MyApp::SendPacket, this);
    }
}

// static void
// CwndChange (Ptr<OutputStreamWrapper> stream,uint32_t oldCwnd, uint32_t newCwnd)
// {
//   // NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << "\t" << newCwnd);
//   *stream->GetStream()<<Simulator::Now ().GetSeconds () << "\t" << newCwnd<<std::endl;
// }

// static void
// TraceCwnd (std::string cwnd_tr_file_name)
// {
//   AsciiTraceHelper ascii;
//   static Ptr<OutputStreamWrapper> cWndStream = ascii.CreateFileStream (cwnd_tr_file_name.c_str ());
//   Config::ConnectWithoutContext ("/NodeList/1/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow", MakeCallback (&CwndChange));
// }

// static void
// RxDrop (Ptr<const Packet> p)
// {
//   NS_LOG_UNCOND ("RxDrop at " << Simulator::Now ().GetSeconds ());
// }

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  // double error_p = 0.1;


  bool tracing = false;
  
  double MaxCoverageRange=10.0;
  double packetsPerSecond=500;
  double packetSize=1024;
  uint32_t numHalfFlows = 20;
  uint32_t nWifi ;
  uint32_t nWifiP;
  uint32_t nPackets=2000;//20000
 
 Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (packetSize));

  // std::string tcpVariant = "TcpWestwood";

  CommandLine cmd (__FILE__);

  // cmd.AddValue ("nWifi", "Number of wifi STA devices left side", nWifi);
  // cmd.AddValue ("nWifiP", "Number of wifi STA devices rght side", nWifiP);
  cmd.AddValue ("numHalfFlows", "Number of flows", numHalfFlows);
  cmd.AddValue ("packetsPerSecond", "Number packets to be transmitted per second", packetsPerSecond);
  cmd.AddValue ("MaxCoverageRange", "Max coverage range of wifi", MaxCoverageRange);

  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

  cmd.Parse (argc,argv);



  // The underlying restriction of 18 is due to the grid position
  // allocator's configuration; the grid layout will exceed the
  // bounding box if more than 18 nodes are provided.
  // if (nWifi > 18)
  //   {
  //     std::cout << "nWifi should be 18 or less; otherwise grid layout exceeds the bounding box" << std::endl;
  //     return 1;
  //   }



  std::string dataRate = std::to_string(packetsPerSecond*packetSize*8/1024) + "Kbps";
 
  nWifi = numHalfFlows;
  nWifiP = numHalfFlows;
  

  
  // tcpVariant = std::string ("ns3::") + tcpVariant;
  // TypeId tcpTid;
  // NS_ABORT_MSG_UNLESS (TypeId::LookupByNameFailSafe (tcpVariant, &tcpTid), "TypeId " << tcpVariant << " not found");
  // Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TypeId::LookupByName (tcpVariant)));
  
  NodeContainer p2pNodes;
  p2pNodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));


  //add error model

  // Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
  // uv->SetStream (50);
  // RateErrorModel error_model;
  // error_model.SetRandomVariable (uv);
  // error_model.SetUnit (RateErrorModel::ERROR_UNIT_PACKET);
  // error_model.SetRate (error_p);
  // pointToPoint.SetDeviceAttribute ("ReceiveErrorModel", PointerValue (&error_model));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi);

  NodeContainer wifiStaNodesP;
  wifiStaNodesP.Create (nWifiP);

  NodeContainer wifiApNode = p2pNodes.Get (0);
  NodeContainer wifiApNodeP = p2pNodes.Get (1);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  channel.AddPropagationLoss("ns3::RangePropagationLossModel", "MaxRange", DoubleValue(MaxCoverageRange));
  YansWifiPhyHelper phy;
  phy.SetChannel (channel.Create ());

  WifiHelper wifi;
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  WifiMacHelper mac;
  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, wifiStaNodes);


  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevices;
  apDevices = wifi.Install (phy, mac, wifiApNode);


  YansWifiChannelHelper channelP = YansWifiChannelHelper::Default ();
  channelP.AddPropagationLoss("ns3::RangePropagationLossModel", "MaxRange", DoubleValue(MaxCoverageRange));
  YansWifiPhyHelper phyP;
  phyP.SetChannel (channelP.Create ());

  WifiHelper wifiP;
  wifiP.SetRemoteStationManager ("ns3::AarfWifiManager");

  WifiMacHelper macP;
  Ssid ssidP = Ssid ("ns-3-ssid");
  macP.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssidP),
               "ActiveProbing", BooleanValue (false));

  NetDeviceContainer staDevicesP;
  staDevicesP = wifiP.Install (phyP, macP, wifiStaNodesP);

  macP.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssidP));

  NetDeviceContainer apDevicesP;
  apDevicesP = wifiP.Install (phyP, macP, wifiApNodeP);

  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (0.5),
                                 "DeltaY", DoubleValue (0.5),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiStaNodes);
  mobility.Install (wifiStaNodesP);

  mobility.Install (wifiApNode);
  mobility.Install (wifiApNodeP);

// install stacks to the nodes
  InternetStackHelper stack;

  stack.Install(p2pNodes);

  // Ipv4StaticRoutingHelper staticRoutingHelper;
  // stack.SetRoutingHelper (staticRoutingHelper);

  stack.Install (wifiStaNodes);
  stack.Install (wifiStaNodesP);

// assign ip addresses to the nodes
  Ipv4AddressHelper address;

  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);

  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer wifiApInterfacesP,wifiStaInterfacesP;
  wifiStaInterfacesP=address.Assign (staDevicesP);
  wifiApInterfacesP=address.Assign (apDevicesP);

  address.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer wifiApInterfaces,wifiStaInterfaces;
  wifiStaInterfaces=address.Assign (staDevices);
  wifiApInterfaces=address.Assign (apDevices);

  // introduce the default route ie static route to all the sta nodes. ie, all the sta nodes will have their corresponding ap node as gateway

  
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  for (NodeContainer::Iterator iter = wifiStaNodes.Begin (); iter != wifiStaNodes.End (); iter++)
  {
    Ptr<Ipv4StaticRouting> staticRouting;
    staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> ((*iter)->GetObject<Ipv4> ()->GetRoutingProtocol ());
    staticRouting->SetDefaultRoute (wifiApInterfaces.GetAddress(0), 1 );
  }

  for (NodeContainer::Iterator iter = wifiStaNodesP.Begin (); iter != wifiStaNodesP.End (); iter++)
  {

    Ptr<Ipv4StaticRouting> staticRouting;
    staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> ((*iter)->GetObject<Ipv4> ()->GetRoutingProtocol ());
    staticRouting->SetDefaultRoute (wifiApInterfacesP.GetAddress(0), 1 );
  }

 
 
  FlowMonitorHelper flowmonitorhelper;
  Ptr<FlowMonitor> flowmonitor = flowmonitorhelper.InstallAll();


  for(uint32_t i = 0; i < numHalfFlows ; i++) {


    uint16_t sinkPort = 8000+i;
    Address sinkAddress (InetSocketAddress (wifiStaInterfacesP.GetAddress (i), sinkPort));
    PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
    ApplicationContainer sinkApps = packetSinkHelper.Install (wifiStaNodesP.Get (i));
    sinkApps.Start (Seconds (0.));
    sinkApps.Stop (Seconds (simulationTime));

 
    Ptr<Socket> ns3TcpSocket = Socket::CreateSocket (wifiStaNodes.Get (i), TcpSocketFactory::GetTypeId ());
    Ptr<MyApp> app = CreateObject<MyApp> ();
    app->Setup (ns3TcpSocket, sinkAddress, packetSize, nPackets, DataRate (dataRate));
    wifiStaNodes.Get (i)->AddApplication (app);
    app->SetStartTime (Seconds (1.));
    app->SetStopTime (Seconds (simulationTime));

      
  

    // AsciiTraceHelper ascii ;
    // Ptr<OutputStreamWrapper> osw=ascii.CreateFileStream("data/cwnd"+tcpVariant+std::to_string(i));
    
    // ns3TcpSocket->TraceConnectWithoutContext ("CongestionWindow", MakeBoundCallback (&CwndChange,osw));
  }
  

 

  Simulator::Stop (Seconds (simulationTime));
   

  Simulator::Run ();
  flowmonitor->SerializeToXmlFile("TopologyForTaskA.xml", true, true);

  int j = 0;
  float AvgThroughput = 0;
  int SentPackets=0,ReceivedPackets=0,LostPackets=0;
  Time Jitter;
  Time Delay;

  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmonitorhelper.GetClassifier());
  std::map<FlowId, FlowMonitor::FlowStats> stats = flowmonitor->GetFlowStats();

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin(); iter != stats.end(); ++iter)
  {
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);

      NS_LOG_UNCOND("----Flow ID:" << iter->first);
      NS_LOG_UNCOND("Source Address (" << t.sourceAddress <<","<<t.sourcePort<< ") Destination Address (" << t.destinationAddress<<","<<t.destinationPort<< ")");
      NS_LOG_UNCOND("Sent Packets=" << iter->second.txPackets);
      NS_LOG_UNCOND("Received Packets =" << iter->second.rxPackets);
      NS_LOG_UNCOND("Lost Packets =" << iter->second.txPackets - iter->second.rxPackets);
      NS_LOG_UNCOND("Packet delivery ratio =" << iter->second.rxPackets * 100 / iter->second.txPackets << "%");
      NS_LOG_UNCOND("Packet loss ratio =" << (iter->second.txPackets - iter->second.rxPackets) * 100 / iter->second.txPackets << "%");
      NS_LOG_UNCOND("Delay =" << iter->second.delaySum);
      NS_LOG_UNCOND("Jitter =" << iter->second.jitterSum);
      NS_LOG_UNCOND("Throughput =" << iter->second.rxBytes * 8.0 / (iter->second.timeLastRxPacket.GetSeconds() - iter->second.timeFirstTxPacket.GetSeconds()) / 1024 << "Kbps");

      SentPackets = SentPackets + (iter->second.txPackets);
      ReceivedPackets = ReceivedPackets + (iter->second.rxPackets);
      LostPackets = LostPackets + (iter->second.txPackets - iter->second.rxPackets);
      AvgThroughput = AvgThroughput + (iter->second.rxBytes * 8.0 / (iter->second.timeLastRxPacket.GetSeconds() - iter->second.timeFirstTxPacket.GetSeconds()) )/ 1024;
      Delay = Delay + (iter->second.delaySum);
      Jitter = Jitter + (iter->second.jitterSum);

      j = j + 1;
      std::cout<<"\n";
  }

  
  float DeliveryRatio=(100.0*ReceivedPackets)/SentPackets;
  float DropRatio=(100.0*LostPackets)/SentPackets;
  
  uint32_t totalFlows=2*numHalfFlows;
  AvgThroughput = AvgThroughput /totalFlows;
  Time  avgDelay=Delay/ReceivedPackets;


  // std::ofstream AvgThroughputFileVsNumNodes;
  // AvgThroughputFileVsNumNodes.open("dataTaskA/AvgThroughputFileVsNumNodes",std::ios_base::app);
  // AvgThroughputFileVsNumNodes<<totalNodes<<" "<<AvgThroughput<<std::endl;

  // std::ofstream DeliveryRatioFileVsNumNodes;
  // DeliveryRatioFileVsNumNodes.open("dataTaskA/DeliveryRatioFileVsNumNodes",std::ios_base::app);
  // DeliveryRatioFileVsNumNodes<<totalNodes<<" "<<DeliveryRatio<<std::endl;

  //   std::ofstream DropRatioFileVsNumNodes;
  // DropRatioFileVsNumNodes.open("dataTaskA/DropRatioFileVsNumNodes",std::ios_base::app);
  // DropRatioFileVsNumNodes<<totalNodes<<" "<<DropRatio<<std::endl;

  //   std::ofstream DelayFileVsNumNodes;
  // DelayFileVsNumNodes.open("dataTaskA/DelayFileVsNumNodes",std::ios_base::app);
  // DelayFileVsNumNodes<<totalNodes<<" "<<avgDelay<<std::endl;




  std::ofstream AvgThroughputFileVsNumFlows;
  AvgThroughputFileVsNumFlows.open("dataTaskA/AvgThroughputFileVsNumFlows",std::ios_base::app);
  AvgThroughputFileVsNumFlows<<totalFlows<<" "<<AvgThroughput<<std::endl;

  std::ofstream DeliveryRatioFileVsNumFlows;
  DeliveryRatioFileVsNumFlows.open("dataTaskA/DeliveryRatioFileVsNumFlows",std::ios_base::app);
  DeliveryRatioFileVsNumFlows<<totalFlows<<" "<<DeliveryRatio<<std::endl;

    std::ofstream DropRatioFileVsNumFlows;
  DropRatioFileVsNumFlows.open("dataTaskA/DropRatioFileVsNumFlows",std::ios_base::app);
   DropRatioFileVsNumFlows<<totalFlows<<" "<<DropRatio<<std::endl;

     std::ofstream DelayFileVsNumFlows;
  DelayFileVsNumFlows.open("dataTaskA/DelayFileVsNumFlows",std::ios_base::app);
   DelayFileVsNumFlows<<totalFlows<<" "<<avgDelay<<std::endl;



  // std::ofstream AvgThroughputFileVsNumPackets;
  // AvgThroughputFileVsNumPackets.open("dataTaskA/AvgThroughputFileVsNumPackets",std::ios_base::app);
  // AvgThroughputFileVsNumPackets<<packetsPerSecond<<" "<<AvgThroughput<<std::endl;

  //   std::ofstream DeliveryRatioFileVsNumPackets;
  // DeliveryRatioFileVsNumPackets.open("dataTaskA/DeliveryRatioFileVsNumPackets",std::ios_base::app);
  // DeliveryRatioFileVsNumPackets<<packetsPerSecond<<" "<<DeliveryRatio<<std::endl;

  // std::ofstream DropRatioFileVsNumPackets;
  // DropRatioFileVsNumPackets.open("dataTaskA/DropRatioFileVsNumPackets",std::ios_base::app);
  // DropRatioFileVsNumPackets<<packetsPerSecond<<" "<<DropRatio<<std::endl;

  // std::ofstream DelayFileVsNumPackets;
  // DelayFileVsNumPackets.open("dataTaskA/DelayFileVsNumPackets",std::ios_base::app);
  // DelayFileVsNumPackets<<packetsPerSecond<<" "<<avgDelay<<std::endl;



  // std::ofstream AvgThroughputFileVsCoverage;
  // AvgThroughputFileVsCoverage.open("dataTaskA/AvgThroughputFileVsCoverage",std::ios_base::app);
  // AvgThroughputFileVsCoverage<<MaxCoverageRange<<" "<<AvgThroughput<<std::endl;

  // std::ofstream DeliveryRatioFileVsCoverage;
  // DeliveryRatioFileVsCoverage.open("dataTaskA/DeliveryRatioFileVsCoverage",std::ios_base::app);
  // DeliveryRatioFileVsCoverage<<MaxCoverageRange<<" "<<DeliveryRatio<<std::endl;

  // std::ofstream DropRatioFileVsCoverage;
  // DropRatioFileVsCoverage.open("dataTaskA/DropRatioFileVsCoverage",std::ios_base::app);
  // DropRatioFileVsCoverage<<MaxCoverageRange<<" "<<DropRatio<<std::endl;

  //   std::ofstream DelayFileVsCoverage;
  // DelayFileVsCoverage.open("dataTaskA/DelayFileVsCoverage",std::ios_base::app);
  // DelayFileVsCoverage<<MaxCoverageRange<<" "<<avgDelay<<std::endl;



  // std::ofstream AvgThroughputFileVspacketSize;
  // AvgThroughputFileVspacketSize.open("dataTaskA/AvgThroughputFileVspacketSize",std::ios_base::app);
  // AvgThroughputFileVspacketSize<<packetSize<<" "<<AvgThroughput<<std::endl;

  // std::ofstream DeliveryRatioFileVspacketSize;
  // DeliveryRatioFileVspacketSize.open("dataTaskA/DeliveryRatioFileVspacketSize",std::ios_base::app);
  // DeliveryRatioFileVspacketSize<<packetSize<<" "<<DeliveryRatio<<std::endl;

  // std::ofstream DropRatioFileVspacketSize;
  // DropRatioFileVspacketSize.open("dataTaskA/DropRatioFileVspacketSize",std::ios_base::app);
  // DropRatioFileVspacketSize<<packetSize<<" "<<DropRatio<<std::endl;

  //   std::ofstream DelayFileVspacketSize;
  // DelayFileVspacketSize.open("dataTaskA/DelayFileVspacketSize",std::ios_base::app);
  // DelayFileVspacketSize<<packetSize<<" "<<avgDelay<<std::endl;












  NS_LOG_UNCOND("--------Total Results of the simulation----------" << std::endl);
  NS_LOG_UNCOND("Total sent packets  =" << SentPackets);
  NS_LOG_UNCOND("Total Received Packets =" << ReceivedPackets);
  NS_LOG_UNCOND("Total Lost Packets =" << LostPackets);
  NS_LOG_UNCOND("Packet Loss ratio =" <<  DropRatio << "%");
  NS_LOG_UNCOND("Packet delivery ratio =" << DeliveryRatio << "%");
  NS_LOG_UNCOND("Average Throughput =" << AvgThroughput << "Kbps");
  NS_LOG_UNCOND("End to End Delay =" << Delay);
  NS_LOG_UNCOND("End to End Average Delay =" << avgDelay);
  NS_LOG_UNCOND("End to End Jitter delay =" << Jitter);
  NS_LOG_UNCOND("Total Flow " << j);
  Simulator::Destroy ();

//   Ptr<Ipv4FlowClassifier> classifier=DynamicCast<Ipv4FlowClassifier>(flowmonitorhelper.GetClassifier ());
//   FlowMonitor::FlowStatsContainer stats= flowmonitor->GetFlowStats();

//   for (auto iter = stats.begin(); iter!=stats.end(); ++iter)
//   {
//     Ipv4FlowClassifier::FiveTuple flowid=classifier->FindFlow(iter->first);
    
    
//     NS_LOG_UNCOND("Protocol : "<<flowid.protocol<<" SRC: IP > "<<flowid.sourceAddress<<" PORT > "<<flowid.sourcePort<<"---------- DST: IP> "<<flowid.destinationAddress<<" PORT > "<<flowid.destinationPort);
    
//   }




  
  return 0;
}