/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Universita' di Firenze
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

// Network topology
//
//       n0    n1
//       |     |
//       =================
//        WSN (802.15.4)
//
// - ICMPv6 echo request flows from n0 to n1 and back with ICMPv6 echo reply
// - DropTail queues 
// - Tracing of queues and packet receptions to file "wsn-ping6.tr"
//
// This example is based on the "ping6.cc" example.

#include <fstream>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/sixlowpan-module.h"
#include "ns3/lr-wpan-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/mobility-module.h"
#include "ns3/on-off-helper.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/mobility-model.h"
#include "ns3/packet-sink.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/single-model-spectrum-channel.h"

#include "ns3/internet-module.h"
#include "ns3/sixlowpan-module.h"
#include "ns3/lr-wpan-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/mobility-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/netanim-module.h"
#include "ns3/single-model-spectrum-channel.h"



using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Ping6WsnExample");

int main (int argc, char **argv)
{
  bool verbose = false;
  uint32_t coverage=1;
  uint32_t nNodes=100;

  CommandLine cmd (__FILE__);
  cmd.AddValue ("verbose", "turn on log components", verbose);
  cmd.Parse (argc, argv);

  if (verbose)
    {
      LogComponentEnable ("Ping6WsnExample", LOG_LEVEL_INFO);
      LogComponentEnable ("Ipv6EndPointDemux", LOG_LEVEL_ALL);
      LogComponentEnable ("Ipv6L3Protocol", LOG_LEVEL_ALL);
      LogComponentEnable ("Ipv6StaticRouting", LOG_LEVEL_ALL);
      LogComponentEnable ("Ipv6ListRouting", LOG_LEVEL_ALL);
      LogComponentEnable ("Ipv6Interface", LOG_LEVEL_ALL);
      LogComponentEnable ("Icmpv6L4Protocol", LOG_LEVEL_ALL);
      LogComponentEnable ("Ping6Application", LOG_LEVEL_ALL);
      LogComponentEnable ("NdiscCache", LOG_LEVEL_ALL);
      LogComponentEnable ("SixLowPanNetDevice", LOG_LEVEL_ALL);
    }

  NS_LOG_INFO ("Create nodes.");
  NodeContainer nodes;
  nodes.Create (nNodes);

  // Set seed for random numbers
  SeedManager::SetSeed (167);

  // Install mobility
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");


  mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue(5.0),
                                  "MinY", DoubleValue(0.0),
                                  // "DeltaX", DoubleValue(.5),
                                  "DeltaX", DoubleValue(.5),

                                  // "DeltaY", DoubleValue(1.2),
                                  "DeltaY", DoubleValue(1.2),

                                  "GridWidth", UintegerValue(2),
                                  "LayoutType", StringValue("RowFirst"));
  mobility.Install(nodes);

  LrWpanHelper lrWpanHelper;
  // Add and install the LrWpanNetDevice for each node
  // lrWpanHelper.EnableLogComponents();
   Ptr<SingleModelSpectrumChannel> channel = CreateObject<SingleModelSpectrumChannel>();
  Ptr<RangePropagationLossModel> propModel = CreateObject<RangePropagationLossModel>();
  Ptr<ConstantSpeedPropagationDelayModel> delayModel = CreateObject<ConstantSpeedPropagationDelayModel>();
  propModel->SetAttribute("MaxRange", DoubleValue(coverage * 50));
  channel->AddPropagationLossModel(propModel);
  channel->SetPropagationDelayModel(delayModel);

  lrWpanHelper.SetChannel(channel);
  NetDeviceContainer devContainer = lrWpanHelper.Install(nodes);
  lrWpanHelper.AssociateToPan (devContainer, 10);

  std::cout << "Created " << devContainer.GetN() << " devices" << std::endl;
  std::cout << "There are " << nodes.GetN() << " nodes" << std::endl;

  /* Install IPv4/IPv6 stack */
  NS_LOG_INFO ("Install Internet stack.");
  InternetStackHelper internetv6;
  internetv6.SetIpv4StackInstall (false);
  internetv6.Install (nodes);

  // Install 6LowPan layer
  NS_LOG_INFO ("Install 6LoWPAN.");
  SixLowPanHelper sixlowpan;
  NetDeviceContainer six1 = sixlowpan.Install (devContainer);

  NS_LOG_INFO ("Assign addresses.");
  Ipv6AddressHelper ipv6;
  ipv6.SetBase (Ipv6Address ("2001:1::"), Ipv6Prefix (64));
  Ipv6InterfaceContainer interfaces = ipv6.Assign (six1);



  /* Create a Ping6 application to send ICMPv6 echo request from node zero to
   * all-nodes (ff02::1).
   */
  uint32_t packetSize = 10;
  Time interPacketInterval = Seconds (1.);
  int32_t simulationTime=10;


    uint32_t nHalf=nNodes/2;



    NS_LOG_INFO ("Create Applications.");
    uint16_t sinkPort = 9;
    ApplicationContainer sinkApps;

    for(uint32_t i=0;i<nHalf;i++)
    {
    uint32_t j=nHalf+i;
    NS_LOG_INFO ("Create Application"<<i);
        /* Install TCP Receiver on the access point */
    PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", Inet6SocketAddress (Ipv6Address::GetAny (), sinkPort));
    ApplicationContainer sinkApp = sinkHelper.Install (nodes.Get(j));
      /* Install TCP/UDP Transmitter on the station */
    OnOffHelper server ("ns3::TcpSocketFactory", (Inet6SocketAddress (interfaces.GetAddress (j,1), sinkPort)));
    server.SetAttribute ("PacketSize", UintegerValue (packetSize));
    server.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
    server.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
    ApplicationContainer serverApp = server.Install (nodes.Get(i));


    sinkApp.Start (Seconds (0.0));
    sinkApp.Stop (Seconds (simulationTime));
    serverApp.Start (Seconds (1.0));

    
    serverApp.Stop (Seconds (simulationTime+10));

}

    NS_LOG_INFO (" Applications Created.");


   FlowMonitorHelper flowmonitorhelper;
  Ptr<FlowMonitor> flowmonitor = flowmonitorhelper.InstallAll();

  Simulator::Stop (Seconds (simulationTime+10));

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  flowmonitor->SerializeToXmlFile("TopologyForTaskALowRate.xml", true, true);

  int j = 0;
  float AvgThroughput = 0;
  int SentPackets=0,ReceivedPackets=0,LostPackets=0;
  Time Jitter;
  Time Delay;

  Ptr<Ipv6FlowClassifier> classifier = DynamicCast<Ipv6FlowClassifier>(flowmonitorhelper.GetClassifier6());
  std::map<FlowId, FlowMonitor::FlowStats> stats = flowmonitor->GetFlowStats();

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin(); iter != stats.end(); ++iter)
  {
      Ipv6FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);

      // NS_LOG_UNCOND("----Flow ID:" << iter->first);
      // NS_LOG_UNCOND("Source Address (" << t.sourceAddress <<","<<t.sourcePort<< ") Destination Address (" << t.destinationAddress<<","<<t.destinationPort<< ")");
      // NS_LOG_UNCOND("Sent Packets=" << iter->second.txPackets);
      // NS_LOG_UNCOND("Received Packets =" << iter->second.rxPackets);
      // NS_LOG_UNCOND("Lost Packets =" << iter->second.lostPackets);
      // NS_LOG_UNCOND("Packet delivery ratio =" << iter->second.rxPackets * 100 / iter->second.txPackets << "%");
      // NS_LOG_UNCOND("Packet loss ratio =" << (iter->second.lostPackets) * 100 / iter->second.txPackets << "%");
      // NS_LOG_UNCOND("Delay =" << iter->second.delaySum);
      // NS_LOG_UNCOND("Jitter =" << iter->second.jitterSum);
      // NS_LOG_UNCOND("Throughput =" << iter->second.rxBytes * 8.0 / (iter->second.timeLastRxPacket.GetSeconds() - iter->second.timeFirstTxPacket.GetSeconds()) / 1024 << "Kbps");
      // std::cout<<"\n";
      SentPackets = SentPackets + (iter->second.txPackets);
      ReceivedPackets = ReceivedPackets + (iter->second.rxPackets);
      LostPackets = LostPackets + (iter->second.lostPackets);
      AvgThroughput = AvgThroughput + (iter->second.rxBytes * 8.0 / (iter->second.timeLastRxPacket.GetSeconds() - iter->second.timeFirstTxPacket.GetSeconds()) )/ 1024;
      Delay = Delay + (iter->second.delaySum);
      Jitter = Jitter + (iter->second.jitterSum);

      j = j + 1;
      
  }


  
  float DeliveryRatio=(100.0*ReceivedPackets)/SentPackets;
  float DropRatio=(100.0*LostPackets)/SentPackets;
  
  uint32_t totalFlows=nNodes;
  AvgThroughput = AvgThroughput /totalFlows;
  Time  avgDelay=Delay/ReceivedPackets;
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
  std::cout<<"\n";


  // std::ofstream AvgThroughputFileVsNumNodes;
  // AvgThroughputFileVsNumNodes.open("dataTaskA2/AvgThroughputFileVsNumNodes",std::ios_base::app);
  // AvgThroughputFileVsNumNodes<<nNodes<<" "<<AvgThroughput<<std::endl;

  // std::ofstream DeliveryRatioFileVsNumNodes;
  // DeliveryRatioFileVsNumNodes.open("dataTaskA2/DeliveryRatioFileVsNumNodes",std::ios_base::app);
  // DeliveryRatioFileVsNumNodes<<nNodes<<" "<<DeliveryRatio<<std::endl;

  //   std::ofstream DropRatioFileVsNumNodes;
  // DropRatioFileVsNumNodes.open("dataTaskA2/DropRatioFileVsNumNodes",std::ios_base::app);
  // DropRatioFileVsNumNodes<<nNodes<<" "<<DropRatio<<std::endl;

  //   std::ofstream DelayFileVsNumNodes;
  // DelayFileVsNumNodes.open("dataTaskA2/DelayFileVsNumNodes",std::ios_base::app);
  // DelayFileVsNumNodes<<nNodes<<" "<<avgDelay<<std::endl;




  // std::ofstream AvgThroughputFileVsNumFlows;
  // AvgThroughputFileVsNumFlows.open("dataTaskA2/AvgThroughputFileVsNumFlows",std::ios_base::app);
  // AvgThroughputFileVsNumFlows<<totalFlows<<" "<<AvgThroughput<<std::endl;

  // std::ofstream DeliveryRatioFileVsNumFlows;
  // DeliveryRatioFileVsNumFlows.open("dataTaskA2/DeliveryRatioFileVsNumFlows",std::ios_base::app);
  // DeliveryRatioFileVsNumFlows<<totalFlows<<" "<<DeliveryRatio<<std::endl;

  //   std::ofstream DropRatioFileVsNumFlows;
  // DropRatioFileVsNumFlows.open("dataTaskA2/DropRatioFileVsNumFlows",std::ios_base::app);
  //  DropRatioFileVsNumFlows<<totalFlows<<" "<<DropRatio<<std::endl;

  //    std::ofstream DelayFileVsNumFlows;
  // DelayFileVsNumFlows.open("dataTaskA2/DelayFileVsNumFlows",std::ios_base::app);
  //  DelayFileVsNumFlows<<totalFlows<<" "<<avgDelay<<std::endl;



  // std::ofstream AvgThroughputFileVsNumPackets;
  // AvgThroughputFileVsNumPackets.open("dataTaskA2/AvgThroughputFileVsNumPackets",std::ios_base::app);
  // AvgThroughputFileVsNumPackets<<packetsPerSecond<<" "<<AvgThroughput<<std::endl;

  //   std::ofstream DeliveryRatioFileVsNumPackets;
  // DeliveryRatioFileVsNumPackets.open("dataTaskA2/DeliveryRatioFileVsNumPackets",std::ios_base::app);
  // DeliveryRatioFileVsNumPackets<<packetsPerSecond<<" "<<DeliveryRatio<<std::endl;

  // std::ofstream DropRatioFileVsNumPackets;
  // DropRatioFileVsNumPackets.open("dataTaskA2/DropRatioFileVsNumPackets",std::ios_base::app);
  // DropRatioFileVsNumPackets<<packetsPerSecond<<" "<<DropRatio<<std::endl;

  // std::ofstream DelayFileVsNumPackets;
  // DelayFileVsNumPackets.open("dataTaskA2/DelayFileVsNumPackets",std::ios_base::app);
  // DelayFileVsNumPackets<<packetsPerSecond<<" "<<avgDelay<<std::endl;



  // std::ofstream AvgThroughputFileVsCoverage;
  // AvgThroughputFileVsCoverage.open("dataTaskA2/AvgThroughputFileVsCoverage",std::ios_base::app);
  // AvgThroughputFileVsCoverage<<MaxCoverageRange<<" "<<AvgThroughput<<std::endl;

  // std::ofstream DeliveryRatioFileVsCoverage;
  // DeliveryRatioFileVsCoverage.open("dataTaskA2/DeliveryRatioFileVsCoverage",std::ios_base::app);
  // DeliveryRatioFileVsCoverage<<MaxCoverageRange<<" "<<DeliveryRatio<<std::endl;

  // std::ofstream DropRatioFileVsCoverage;
  // DropRatioFileVsCoverage.open("dataTaskA2/DropRatioFileVsCoverage",std::ios_base::app);
  // DropRatioFileVsCoverage<<MaxCoverageRange<<" "<<DropRatio<<std::endl;

  //   std::ofstream DelayFileVsCoverage;
  // DelayFileVsCoverage.open("dataTaskA2/DelayFileVsCoverage",std::ios_base::app);
  // DelayFileVsCoverage<<MaxCoverageRange<<" "<<avgDelay<<std::endl;



  // std::ofstream AvgThroughputFileVspacketSize;
  // AvgThroughputFileVspacketSize.open("dataTaskA2/AvgThroughputFileVspacketSize",std::ios_base::app);
  // AvgThroughputFileVspacketSize<<packetSize<<" "<<AvgThroughput<<std::endl;

  // std::ofstream DeliveryRatioFileVspacketSize;
  // DeliveryRatioFileVspacketSize.open("dataTaskA2/DeliveryRatioFileVspacketSize",std::ios_base::app);
  // DeliveryRatioFileVspacketSize<<packetSize<<" "<<DeliveryRatio<<std::endl;

  // std::ofstream DropRatioFileVspacketSize;
  // DropRatioFileVspacketSize.open("dataTaskA2/DropRatioFileVspacketSize",std::ios_base::app);
  // DropRatioFileVspacketSize<<packetSize<<" "<<DropRatio<<std::endl;

  //   std::ofstream DelayFileVspacketSize;
  // DelayFileVspacketSize.open("dataTaskA2/DelayFileVspacketSize",std::ios_base::app);
  // DelayFileVspacketSize<<packetSize<<" "<<avgDelay<<std::endl;




  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}

