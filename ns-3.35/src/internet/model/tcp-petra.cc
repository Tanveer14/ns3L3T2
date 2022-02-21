
#include "tcp-petra.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "rtt-estimator.h"
#include "tcp-socket-base.h"



namespace ns3 {
NS_LOG_COMPONENT_DEFINE ("TcpPetra");
NS_OBJECT_ENSURE_REGISTERED (TcpPetra);

TypeId
TcpPetra::GetTypeId (void)
{
  static TypeId tid = TypeId("ns3::TcpPetra")
    .SetParent<TcpWestwood>()
    .SetGroupName ("Internet")
    .AddConstructor<TcpPetra>()
  ;
  return tid;
}

TcpPetra::TcpPetra (void) :
  TcpWestwood ()
{
  NS_LOG_FUNCTION (this);
  
}

TcpPetra::TcpPetra (const TcpPetra& sock) :
  TcpWestwood (sock)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC ("Invoked the copy constructor");
}

TcpPetra::~TcpPetra (void)
{
}

void
TcpPetra::PktsAcked (Ptr<TcpSocketState> tcb, uint32_t packetsAcked,
                        const Time& rtt)
{
  NS_LOG_FUNCTION (this << tcb << packetsAcked << rtt);

  if (rtt.IsZero ())
    {
      NS_LOG_WARN ("RTT measured is zero!");
      return;
    }

  m_ackedSegments += packetsAcked;
  EstimateBW (rtt, tcb);
   
}


uint32_t
TcpPetra::SlowStart (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{
  NS_LOG_FUNCTION (this << tcb << segmentsAcked);

std::cout<<"ss called from petra...............\n";
  if (segmentsAcked >= 1)
    {
      if(tcb->original_lastRtt<=tcb->m_lastRtt && tcb->m_cWnd<tcb->m_ssThresh/2 )  tcb->m_cWnd += tcb->m_segmentSize*tcb->m_ssThresh/tcb->m_cWnd; 
      else tcb->m_cWnd += tcb->m_segmentSize;
      NS_LOG_INFO ("In SlowStart, updated to cwnd " << tcb->m_cWnd << " ssthresh " << tcb->m_ssThresh);
      return segmentsAcked - 1;
    }

  return 0;
}



// void
// TcpPetra::IncreaseWindow (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
// {
//   NS_LOG_FUNCTION (this << tcb << segmentsAcked);

//   if (tcb->m_cWnd < tcb->m_ssThresh)
//     {
//       segmentsAcked = SlowStart (tcb, segmentsAcked);
//     }

//   if (tcb->m_cWnd >= tcb->m_ssThresh)
//     {
//       CongestionAvoidance (tcb, segmentsAcked);
//     }

//   /* At this point, we could have segmentsAcked != 0. This because RFC says
//    * that in slow start, we should increase cWnd by min (N, SMSS); if in
//    * slow start we receive a cumulative ACK, it counts only for 1 SMSS of
//    * increase, wasting the others.
//    *
//    * // Incorrect assert, I am sorry
//    * NS_ASSERT (segmentsAcked == 0);
//    */
// }





void
TcpPetra::EstimateBW (const Time &rtt, Ptr<TcpSocketState> tcb)
{
  // std::cout<<"ebw called from petra...............\n";
  NS_LOG_FUNCTION (this);
  // std::cout<<"here14....................................................\n";
  NS_ASSERT (!rtt.IsZero ());
  double alpha = 0.9;
  double sample_bwe;

  Time currentAck = Simulator::Now ();
  m_currentBW = m_ackedSegments * tcb->m_segmentSize / (currentAck - m_lastAck).GetSeconds ();
  m_ackedSegments = 0;
  sample_bwe = m_currentBW;
  m_currentBW = (alpha * m_lastBW) + ((1 - alpha) * sample_bwe) ;
  m_lastAck = currentAck;
  m_lastSampleBW = sample_bwe;
  m_lastBW = m_currentBW;
  

  
  NS_LOG_LOGIC ("Estimated BW : " << m_currentBW);
}


Ptr<TcpCongestionOps>
TcpPetra::Fork ()
{
  return CreateObject<TcpPetra> (*this);
}


// uint32_t
// TcpPetra::GetSsThresh (Ptr<const TcpSocketState> tcb,
//                           uint32_t bytesInFlight)
// {
//   NS_UNUSED (bytesInFlight);
//   NS_LOG_LOGIC ("CurrentBW: " << m_currentBW << " minRtt: " <<
//                 tcb->m_minRtt << " ssthresh: " <<
//                 m_currentBW * static_cast<double> (tcb->m_minRtt.GetSeconds ()));

//   return std::max (2*tcb->m_segmentSize,
//                    uint32_t (m_currentBW * static_cast<double> (tcb->m_minRtt.GetSeconds ())));
// }



}