#include <fstream>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/mobility-module.h"
#include "ns3/spectrum-module.h"
#include "ns3/propagation-module.h"
#include "ns3/sixlowpan-module.h"
#include "ns3/lr-wpan-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv6-flow-classifier.h"
#include "ns3/flow-monitor-helper.h"
#include <ns3/lr-wpan-error-model.h>
 
using namespace ns3;

class MyApp : public Application
{
public:
  MyApp ();
  virtual ~MyApp ();

  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);
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

MyApp::~MyApp ()
{
  m_socket = 0;
}

/* static */
TypeId MyApp::GetTypeId (void)
{
  static TypeId tid = TypeId ("MyApp")
    .SetParent<Application> ()
    .SetGroupName ("Tutorial")
    .AddConstructor<MyApp> ()
    ;
  return tid;
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
  if (InetSocketAddress::IsMatchingType (m_peer))
    {
      m_socket->Bind ();
    }
  else
    {
      m_socket->Bind6 ();
    }
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
 
int main (int argc, char** argv)
{
  bool verbose = false;
  uint32_t no_of_TCP_flows = 1;
  uint16_t sinkPort = 9;
  double start_time = 0;
  double simulationTime = 10;
  uint32_t nWsnNodes = 2;
  int packetsize = 1024;
  int totalpacket = 1000;
  std::string dataRate;
  uint64_t packetpersecond = 128;
 
  Packet::EnablePrinting ();
 
  CommandLine cmd (__FILE__);
  cmd.AddValue ("verbose", "turn on log components", verbose);
  cmd.Parse (argc, argv);
 
  if (verbose)
    {
      LogComponentEnable ("Ping6Application", LOG_LEVEL_ALL);
      LogComponentEnable ("LrWpanMac", LOG_LEVEL_ALL);
      LogComponentEnable ("LrWpanPhy", LOG_LEVEL_ALL);
      LogComponentEnable ("LrWpanNetDevice", LOG_LEVEL_ALL);
      LogComponentEnable ("SixLowPanNetDevice", LOG_LEVEL_ALL);
    }

  dataRate = std::to_string(packetpersecond*packetsize*8/(std::pow(2,20))) + "Mbps";
  std::cout << "datarate: " << dataRate << "\n";
 
  
  NodeContainer wsnNodes;
  wsnNodes.Create (nWsnNodes);
 
  NodeContainer wiredNodes;
  wiredNodes.Create (1);
  wiredNodes.Add (wsnNodes.Get (0));
 
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (80),
                                 "DeltaY", DoubleValue (80),
                                 "GridWidth", UintegerValue (10),
                                 "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wsnNodes);
 
  LrWpanHelper lrWpanHelper;

  NetDeviceContainer lrwpanDevices = lrWpanHelper.Install (wsnNodes);
 
  lrWpanHelper.AssociateToPan (lrwpanDevices, 0);
 
  InternetStackHelper internetv6;
  internetv6.Install (wsnNodes);
  internetv6.Install (wiredNodes.Get (0));
 
  SixLowPanHelper sixLowPanHelper;
  NetDeviceContainer sixLowPanDevices = sixLowPanHelper.Install (lrwpanDevices);
 
  CsmaHelper csmaHelper;
  NetDeviceContainer csmaDevices = csmaHelper.Install (wiredNodes);
 
  Ipv6AddressHelper ipv6address;
  ipv6address.SetBase (Ipv6Address ("2001:cafe::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer wiredDeviceInterfaces;
  wiredDeviceInterfaces = ipv6address.Assign (csmaDevices);
  wiredDeviceInterfaces.SetForwarding (1, true);
  wiredDeviceInterfaces.SetDefaultRouteInAllNodes (1);
 
  ipv6address.SetBase (Ipv6Address ("2001:f00d::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer wsnDeviceInterfaces;
  wsnDeviceInterfaces = ipv6address.Assign (sixLowPanDevices);
  wsnDeviceInterfaces.SetForwarding (0, true);
  wsnDeviceInterfaces.SetDefaultRouteInAllNodes (0);

 
  for (uint32_t i = 0; i < sixLowPanDevices.GetN (); i++)
    {
      Ptr<NetDevice> dev = sixLowPanDevices.Get (i);
      dev->SetAttribute ("UseMeshUnder", BooleanValue (true));
      dev->SetAttribute ("MeshUnderRadius", UintegerValue (10));
    }

// flow
  for(uint64_t i = 0; i < no_of_TCP_flows; i++) {
    Address sinkAddress(Inet6SocketAddress(wiredDeviceInterfaces.GetAddress(0,1), sinkPort));
    PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", Inet6SocketAddress (Ipv6Address::GetAny (), sinkPort));
    ApplicationContainer sinkApps = sinkHelper.Install (wiredNodes.Get(i));

    sinkApps.Start (Seconds (start_time));
    sinkApps.Stop (Seconds (simulationTime));

    Ptr<Socket> ns3TcpSocket = Socket::CreateSocket (wsnNodes.Get(i), TcpSocketFactory::GetTypeId ());

    Ptr<MyApp> app = CreateObject<MyApp> ();
    app->Setup (ns3TcpSocket, sinkAddress, packetsize, totalpacket, DataRate (dataRate));
    wsnNodes.Get(i)->AddApplication (app);
    app->SetStartTime (Seconds (start_time + 1));
    app->SetStopTime (Seconds (simulationTime));
   
    Simulator::Stop (Seconds (simulationTime));
    
    //-------- trace ------ //

    // AsciiTraceHelper asciiTraceHelper;
    // Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream ("customnet_" + std::to_string(i) + ".cwnd");
    // ns3TcpSocket->TraceConnectWithoutContext ("CongestionWindow", MakeBoundCallback (&CwndChange, stream));

  }
  std::cout << "after loop\n";


 
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();

  std::string dir = "Output_Files/";
  std::string dirToSave = "mkdir -p " + dir;
    if (system (dirToSave.c_str ()) == -1)
    {
      exit (1);
    }
  Simulator::Run ();

  flowmon.SerializeToXmlFile(dir+"taskA-wireless-flow.xml", false, false);

  // Ptr<Ipv6FlowClassifier> classifier = DynamicCast<Ipv6FlowClassifier> (flowmon.GetClassifier() );
  // FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();


  // for(auto iter = stats.begin(); iter != stats.end(); ++iter){
	//   Ipv6FlowClassifier::FiveTuple t = classifier->FindFlow (iter->first);
	//   NS_LOG_UNCOND("====Flow ID: " << iter->first << "====");
	//   NS_LOG_UNCOND("src addr: " << t.sourceAddress << "-- dest addr: " << t.destinationAddress);
	//   NS_LOG_UNCOND("Sent Packets =" <<iter->second. txPackets);
	//   NS_LOG_UNCOND("Received Patkets =" <<iter->second.rxPackets);
  //   NS_LOG_UNCOND("Lost Packets =" <<iter->second.lostPackets);
  //   NS_LOG_UNCOND("Dropped Packet = " << iter->second.txPackets - iter->second.rxPackets - iter->second.lostPackets);
  //   NS_LOG_UNCOND("Metrics Calculated:");
  //   NS_LOG_UNCOND("\tNetwork Throughput =" <<iter->second.rxBytes * 8.0/(iter->second.timeLastRxPacket.GetSeconds()-iter->second.timeFirstTxPacket.GetSeconds()) << "bps");
  //   if(iter->second.rxBytes != 0)
  //     NS_LOG_UNCOND("\tEnd to End Delay = " << iter->second.delaySum/iter->second.rxPackets);
  //   else
  //     NS_LOG_UNCOND("\tEnd to End Delay = NA");
  //   NS_LOG_UNCOND("\tPacket delivery ratio = " <<iter->second.rxPackets*100.0/iter->second.txPackets << "%");
	//   NS_LOG_UNCOND("\tPacket loss ratio =" << (iter->second.txPackets-iter->second.rxPackets)*100.0/iter->second.txPackets << "%");
  // }

  Simulator::Destroy ();
 
}