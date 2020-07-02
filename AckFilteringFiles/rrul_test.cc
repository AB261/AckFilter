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

// Network topology
//
//       s --- r1 --- r2 --- c
//           
//
//

#include <string>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/traffic-control-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpBulkSendExample");

int
main (int argc, char *argv[])
{

  bool tracing = true;
  uint32_t maxBytes = 181000;
  NS_LOG_INFO ("Create nodes.");
  NodeContainer nodes;
  nodes.Create (4);

  NodeContainer s_r1_NC;
  s_r1_NC = NodeContainer (nodes.Get (0), nodes.Get (1));
  NodeContainer r1_r2_NC;
  r1_r2_NC = NodeContainer (nodes.Get (1), nodes.Get (2));
  NodeContainer r2_c_NC;
  r2_c_NC = NodeContainer (nodes.Get (2), nodes.Get (3));

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("30Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("0ms"));

  


  //B/T Server and 1st router
  NetDeviceContainer s_r1_dev = p2p.Install (s_r1_NC);
  Ptr<PointToPointNetDevice> s_r1 = DynamicCast<PointToPointNetDevice> (s_r1_dev.Get (0));
  s_r1->SetAttribute ("DataRate", StringValue ("50Mbps"));
  Ptr<PointToPointNetDevice> r1_s = DynamicCast<PointToPointNetDevice> (s_r1_dev.Get (1));
  r1_s->SetAttribute ("DataRate", StringValue ("50Mbps"));

  //B/T 1st router and 2nd router
  NetDeviceContainer r1_r2_dev = p2p.Install (r1_r2_NC);
  Ptr<PointToPointNetDevice> r1_r2 = DynamicCast<PointToPointNetDevice> (r1_r2_dev.Get (0));
  r1_r2->SetAttribute ("DataRate", StringValue ("30Mbps"));
  Ptr<PointToPointNetDevice> r2_r1 = DynamicCast<PointToPointNetDevice> (r1_r2_dev.Get (1));
  r2_r1->SetAttribute ("DataRate", StringValue ("1Mbps"));

  //B/T 2nd router and client
  NetDeviceContainer r2_c1_dev = p2p.Install (r2_c_NC);
  Ptr<PointToPointNetDevice> r2_c = DynamicCast<PointToPointNetDevice> (r2_c1_dev.Get (0));
  r2_c->SetAttribute ("DataRate", StringValue ("50Mbps"));
  Ptr<PointToPointNetDevice> c_r2 = DynamicCast<PointToPointNetDevice> (r2_c1_dev.Get (1));
  c_r2->SetAttribute ("DataRate", StringValue ("50Mbps"));

  InternetStackHelper stack;
  stack.InstallAll ();


  TrafficControlHelper tchRed;
  tchRed.SetRootQueueDisc ("ns3::CobaltQueueDisc","UseAckFilter",BooleanValue (true));
  QueueDiscContainer queueDiscs;
  queueDiscs = tchRed.Install (r1_r2_dev);

  queueDiscs = tchRed.Install (s_r1);

  queueDiscs = tchRed.Install (c_r2);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer s_r1_interface = ipv4.Assign (s_r1_dev);
  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer r1_r2_interface = ipv4.Assign (r1_r2_dev);
  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer r2_c_interface = ipv4.Assign (r2_c1_dev);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();


  uint16_t port = 60000;  // well-known echo port number
  // uint16_t port2 = 50001;



  BulkSendHelper source1 ("ns3::TcpSocketFactory",
                         InetSocketAddress (r2_c_interface.GetAddress (1), port));
  // Set the amount of data to send in bytes.  Zero is unlimited.
  source1.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  ApplicationContainer sourceApps1 = source1.Install (nodes.Get (0));
  sourceApps1.Start (Seconds (0.0));
  sourceApps1.Stop (Seconds (3.0));

  PacketSinkHelper sink1 ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), port));
  ApplicationContainer sinkApps1 = sink1.Install (nodes.Get (3));
  sinkApps1.Start (Seconds (0.0));
  sinkApps1.Stop (Seconds (3.0));

  port = 60001;
  // port2 = 50002;

    BulkSendHelper source2 ("ns3::TcpSocketFactory",
                         InetSocketAddress (r2_c_interface.GetAddress (1), port));
  // Set the amount of data to send in bytes.  Zero is unlimited.
  source2.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  ApplicationContainer sourceApps2 = source2.Install (nodes.Get (0));
  sourceApps2.Start (Seconds (0.0));
  sourceApps2.Stop (Seconds (3.0));

    PacketSinkHelper sink2 ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), port));
  ApplicationContainer sinkApps2 = sink2.Install (nodes.Get (3));
  sinkApps2.Start (Seconds (0.0));
  sinkApps2.Stop (Seconds (3.0));

  port = 60002;
  // port2 = 50003;

    BulkSendHelper source3 ("ns3::TcpSocketFactory",
                         InetSocketAddress (r2_c_interface.GetAddress (1), port));
  // Set the amount of data to send in bytes.  Zero is unlimited.
  source3.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  ApplicationContainer sourceApps3 = source3.Install (nodes.Get (0));
  sourceApps3.Start (Seconds (0.0));
  sourceApps3.Stop (Seconds (3.0));

    PacketSinkHelper sink3 ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), port));
  ApplicationContainer sinkApps3 = sink3.Install (nodes.Get (3));
  sinkApps3.Start (Seconds (0.0));
  sinkApps3.Stop (Seconds (3.0));

  port = 60003;
  // port2 = 50004;

    BulkSendHelper source4 ("ns3::TcpSocketFactory",
                         InetSocketAddress (r2_c_interface.GetAddress (1), port));
  // Set the amount of data to send in bytes.  Zero is unlimited.
  source4.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  ApplicationContainer sourceApps4 = source4.Install (nodes.Get (0));
  sourceApps4.Start (Seconds (0.0));
  sourceApps4.Stop (Seconds (3.0));

  PacketSinkHelper sink4 ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), port));
  ApplicationContainer sinkApps4 = sink4.Install (nodes.Get (3));
  sinkApps4.Start (Seconds (0.0));
  sinkApps4.Stop (Seconds (3.0));

  //Upload connections
  port = 60004;
      BulkSendHelper source5 ("ns3::TcpSocketFactory",
                         InetSocketAddress (s_r1_interface.GetAddress (0), port));
  // Set the amount of data to send in bytes.  Zero is unlimited.
  source5.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  ApplicationContainer sourceApps5 = source5.Install (nodes.Get (3));
  sourceApps5.Start (Seconds (0.0));
  sourceApps5.Stop (Seconds (3.0));

  PacketSinkHelper sink5 ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), port));
  ApplicationContainer sinkApps5 = sink5.Install (nodes.Get (0));
  sinkApps5.Start (Seconds (0.0));
  sinkApps5.Stop (Seconds (3.0));

    port = 60005;
      BulkSendHelper source6 ("ns3::TcpSocketFactory",
                         InetSocketAddress (s_r1_interface.GetAddress (0), port));
  // Set the amount of data to send in bytes.  Zero is unlimited.
  source5.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  ApplicationContainer sourceApps6 = source6.Install (nodes.Get (3));
  sourceApps6.Start (Seconds (0.0));
  sourceApps6.Stop (Seconds (3.0));

  PacketSinkHelper sink6 ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), port));
  ApplicationContainer sinkApps6 = sink6.Install (nodes.Get (0));
  sinkApps6.Start (Seconds (0.0));
  sinkApps6.Stop (Seconds (3.0));

    port = 60006;
      BulkSendHelper source7 ("ns3::TcpSocketFactory",
                         InetSocketAddress (s_r1_interface.GetAddress (0), port));
  // Set the amount of data to send in bytes.  Zero is unlimited.
  source7.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  ApplicationContainer sourceApps7 = source7.Install (nodes.Get (3));
  sourceApps7.Start (Seconds (0.0));
  sourceApps7.Stop (Seconds (3.0));

  PacketSinkHelper sink7 ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), port));
  ApplicationContainer sinkApps7 = sink7.Install (nodes.Get (0));
  sinkApps7.Start (Seconds (0.0));
  sinkApps7.Stop (Seconds (3.0));


    port = 60007;
      BulkSendHelper source8 ("ns3::TcpSocketFactory",
                         InetSocketAddress (s_r1_interface.GetAddress (0), port));
  // Set the amount of data to send in bytes.  Zero is unlimited.
  source8.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  ApplicationContainer sourceApps8 = source8.Install (nodes.Get (3));
  sourceApps8.Start (Seconds (0.0));
  sourceApps8.Stop (Seconds (3.0));

  PacketSinkHelper sink8 ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), port));
  ApplicationContainer sinkApps8 = sink8.Install (nodes.Get (0));
  sinkApps8.Start (Seconds (0.0));
  sinkApps8.Stop (Seconds (3.0));


  if (tracing)
    {
      AsciiTraceHelper ascii;
      p2p.EnableAsciiAll (ascii.CreateFileStream ("tcp-bulk-send_with_ack.tr"));
      p2p.EnablePcapAll ("tcp-bulk-send_with", false);
    }
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Seconds (5));
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");

  Ptr<PacketSink> sink9 = DynamicCast<PacketSink> (sinkApps8.Get (0));
  std::cout << "Total Bytes downloaded: " << sink9->GetTotalRx () << std::endl;

    Ptr<PacketSink> sink10 = DynamicCast<PacketSink> (sinkApps1.Get (0));
  std::cout << "Total Bytes uploaded: " << sink10->GetTotalRx () << std::endl;
}