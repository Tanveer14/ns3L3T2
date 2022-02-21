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

// n1----------n2-------------n3

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
// RxDrop (Ptr<const Packet> p)
// {
//   NS_LOG_UNCOND ("RxDrop at " << Simulator::Now ().GetSeconds ());
// }

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  double error_p = 0.01;
  double simulationTime=5.0;

  bool tracing = false;
  std::string tcpVariant = "TcpWestwood"; 

  CommandLine cmd (__FILE__);


  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

  cmd.Parse (argc,argv);

  // The underlying restriction of 18 is due to the grid position
  // allocator's configuration; the grid layout will exceed the
  // bounding box if more than 18 nodes are provided.

  
  tcpVariant = std::string ("ns3::") + tcpVariant;
  TypeId tcpTid;
  NS_ABORT_MSG_UNLESS (TypeId::LookupByNameFailSafe (tcpVariant, &tcpTid), "TypeId " << tcpVariant << " not found");
  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TypeId::LookupByName (tcpVariant)));

  NodeContainer p2pNodes;
  p2pNodes.Create (3);

  PointToPointHelper pointToPoint1;
  pointToPoint1.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  pointToPoint1.SetChannelAttribute ("Delay", StringValue ("45ms"));

    PointToPointHelper pointToPoint2;
  pointToPoint2.SetDeviceAttribute ("DataRate", StringValue ("2Mbps"));
  pointToPoint2.SetChannelAttribute ("Delay", StringValue ("0.01ms"));


  //add error model

  Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
  uv->SetStream (50);
  RateErrorModel error_model;
  error_model.SetRandomVariable (uv);
  error_model.SetUnit (RateErrorModel::ERROR_UNIT_PACKET);
  error_model.SetRate (error_p);
  pointToPoint1.SetDeviceAttribute ("ReceiveErrorModel", PointerValue (&error_model));

  NetDeviceContainer p2pDevices1;
  p2pDevices1 = pointToPoint1.Install (p2pNodes.Get(0),p2pNodes.Get(1));

pointToPoint2.SetDeviceAttribute ("ReceiveErrorModel", PointerValue (&error_model));

  NetDeviceContainer p2pDevices2;
  p2pDevices2 = pointToPoint2.Install (p2pNodes.Get(1),p2pNodes.Get(2));

  InternetStackHelper stack;

  stack.Install(p2pNodes);


// assign ip addresses to the nodes
  Ipv4AddressHelper address;

  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces1;
  p2pInterfaces1 = address.Assign (p2pDevices1);

  address.SetBase ("10.1.2.0", "255.255.255.0");
   Ipv4InterfaceContainer p2pInterfaces2;
  p2pInterfaces2 = address.Assign (p2pDevices2);


  // introduce the default route ie static route to all the sta nodes. ie, all the sta nodes will have their corresponding ap node as gateway


 
  FlowMonitorHelper flowmonitorhelper;
  Ptr<FlowMonitor> flowmonitor = flowmonitorhelper.InstallAll();




    uint16_t sinkPort = 8000;
    Address sinkAddress (InetSocketAddress (p2pInterfaces2.GetAddress (1), sinkPort));
    PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
    ApplicationContainer sinkApps = packetSinkHelper.Install (p2pNodes.Get (2));
    sinkApps.Start (Seconds (0.));
    sinkApps.Stop (Seconds (simulationTime));

    Ptr<Socket> ns3TcpSocket = Socket::CreateSocket (p2pNodes.Get (0), TcpSocketFactory::GetTypeId ());
    

    Ptr<MyApp> app = CreateObject<MyApp> ();
    app->Setup (ns3TcpSocket, sinkAddress, 1040, 1000, DataRate ("1Mbps"));
    p2pNodes.Get (0)->AddApplication (app);
    app->SetStartTime (Seconds (1.));
    app->SetStopTime (Seconds (simulationTime));

    AsciiTraceHelper ascii ;
    Ptr<OutputStreamWrapper> osw=ascii.CreateFileStream("data/cwnd-p2p");
    ns3TcpSocket->TraceConnectWithoutContext ("CongestionWindow", MakeBoundCallback (&CwndChange,osw));

  

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
 

  Simulator::Stop (Seconds (5.0));
   

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
