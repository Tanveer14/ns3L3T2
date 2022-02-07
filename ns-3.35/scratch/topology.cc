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
// n5   n6   n7   n0 -------------- n1   n2   n3   n4   
//                   point-to-point  |    |    |    |    
//                                   *    *    *    *   
//                                   AP
//                                      Wifi 10.1.2.0 (P)

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");



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

  if (++m_packetsSent < m_nPackets)
    {
      ScheduleTx ();
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

static void
CwndChange (Ptr<OutputStreamWrapper> stream,uint32_t oldCwnd, uint32_t newCwnd)
{
  // NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << "\t" << newCwnd);
  *stream->GetStream()<<Simulator::Now ().GetSeconds () << "\t" << newCwnd<<std::endl;
}

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
  double error_p = 0.1;

  uint32_t nWifi = 4;
  uint32_t nWifiP = 4;
  bool tracing = false;
  double simulationTime=5.0;
  std::string tcpVariant = "TcpNewReno"; 

  CommandLine cmd (__FILE__);

  cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

  cmd.Parse (argc,argv);

  // The underlying restriction of 18 is due to the grid position
  // allocator's configuration; the grid layout will exceed the
  // bounding box if more than 18 nodes are provided.
  if (nWifi > 18)
    {
      std::cout << "nWifi should be 18 or less; otherwise grid layout exceeds the bounding box" << std::endl;
      return 1;
    }
  
  tcpVariant = std::string ("ns3::") + tcpVariant;
  TypeId tcpTid;
  NS_ABORT_MSG_UNLESS (TypeId::LookupByNameFailSafe (tcpVariant, &tcpTid), "TypeId " << tcpVariant << " not found");
  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TypeId::LookupByName (tcpVariant)));
  NodeContainer p2pNodes;
  p2pNodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));


  //add error model

  Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
  uv->SetStream (50);
  RateErrorModel error_model;
  error_model.SetRandomVariable (uv);
  error_model.SetUnit (RateErrorModel::ERROR_UNIT_PACKET);
  error_model.SetRate (error_p);
  pointToPoint.SetDeviceAttribute ("ReceiveErrorModel", PointerValue (&error_model));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi);

  NodeContainer wifiStaNodesP;
  wifiStaNodesP.Create (nWifiP);

  NodeContainer wifiApNode = p2pNodes.Get (0);
  NodeContainer wifiApNodeP = p2pNodes.Get (1);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
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
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
  mobility.Install (wifiStaNodes);
  mobility.Install (wifiStaNodesP);

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);
  mobility.Install (wifiApNodeP);

// install stacks to the nodes
  InternetStackHelper stack;

  stack.Install(p2pNodes);

  Ipv4StaticRoutingHelper staticRoutingHelper;
  stack.SetRoutingHelper (staticRoutingHelper);

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

int num_half_flows = nWifi;
  for(int i = 0; i < num_half_flows; i++) {


    uint16_t sinkPort = 8000+i;
    Address sinkAddress (InetSocketAddress (wifiStaInterfacesP.GetAddress (0), sinkPort));
    PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
    ApplicationContainer sinkApps = packetSinkHelper.Install (wifiStaNodesP.Get (i));
    sinkApps.Start (Seconds (0.));
    sinkApps.Stop (Seconds (simulationTime));

    Ptr<Socket> ns3TcpSocket = Socket::CreateSocket (wifiStaNodes.Get (i), TcpSocketFactory::GetTypeId ());
    

    Ptr<MyApp> app = CreateObject<MyApp> ();
    app->Setup (ns3TcpSocket, sinkAddress, 1472, 20000, DataRate ("100Mbps"));
    wifiStaNodes.Get (i)->AddApplication (app);
    app->SetStartTime (Seconds (1.));
    app->SetStopTime (Seconds (simulationTime));

    AsciiTraceHelper ascii ;
    Ptr<OutputStreamWrapper> osw=ascii.CreateFileStream("data/cwnd"+tcpVariant+std::to_string(i));
    
    ns3TcpSocket->TraceConnectWithoutContext ("CongestionWindow", MakeBoundCallback (&CwndChange,osw));
  }
  

 

  Simulator::Stop (Seconds (simulationTime));
   

  Simulator::Run ();
  Simulator::Destroy ();


  Ptr<Ipv4FlowClassifier> classifier=DynamicCast<Ipv4FlowClassifier>(flowmonitorhelper.GetClassifier ());
  FlowMonitor::FlowStatsContainer stats= flowmonitor->GetFlowStats();

  for (auto iter = stats.begin(); iter!=stats.end(); ++iter)
  {
    Ipv4FlowClassifier::FiveTuple flowid=classifier->FindFlow(iter->first);
    NS_LOG_UNCOND("SRC: IP > "<<flowid.sourceAddress<<" PORT > "<<flowid.sourcePort<<"---------- DST: IP> "<<flowid.destinationAddress<<" PORT > "<<flowid.destinationPort);
  }


  
  return 0;
}
