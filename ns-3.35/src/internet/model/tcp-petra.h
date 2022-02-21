#ifndef TCP_PETRA_H
#define TCP_PETRA_H

// #include "tcp-congestion-ops.h"
#include "tcp-westwood.h"
#include "ns3/tcp-recovery-ops.h"
#include "ns3/sequence-number.h"
#include "ns3/traced-value.h"
#include "ns3/event-id.h"

namespace ns3 {

class TcpPetra : public TcpWestwood
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  TcpPetra (void);
  /**
   * \brief Copy constructor
   * \param sock the object to copy
   */
  TcpPetra (const TcpPetra& sock);
  virtual ~TcpPetra (void);
  virtual void PktsAcked (Ptr<TcpSocketState> tcb, uint32_t packetsAcked,
                          const Time& rtt);
  virtual Ptr<TcpCongestionOps> Fork ();
  // virtual void IncreaseWindow (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked);
  // virtual uint32_t GetSsThresh (Ptr<const TcpSocketState> tcb,
  //                               uint32_t bytesInFlight);

protected:
  
 virtual uint32_t SlowStart (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked);

private:
 void EstimateBW (const Time &rtt, Ptr<TcpSocketState> tcb);
//  void UpdateAckedSegments (int acked);
  
};

} // namespace ns3

#endif /* TCP_PETRA_H */
