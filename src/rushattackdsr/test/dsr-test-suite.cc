/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Yufei Cheng
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
 * Author: Yufei Cheng   <yfcheng@ittc.ku.edu>
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  http://wiki.ittc.ku.edu/resilinets
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture),
 * NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI),
 * US Department of Defense (DoD), and ITTC at The University of Kansas.
 */

#include <vector>
#include "ns3/ptr.h"
#include "ns3/boolean.h"
#include "ns3/test.h"
#include "ns3/ipv4-route.h"
#include "ns3/mesh-helper.h"
#include "ns3/simulator.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "ns3/ipv4-address-helper.h"

#include "ns3/rushattackdsr-fs-header.h"
#include "ns3/rushattackdsr-option-header.h"
#include "ns3/rushattackdsr-rreq-table.h"
#include "ns3/rushattackdsr-rcache.h"
#include "ns3/rushattackdsr-rsendbuff.h"
#include "ns3/rushattackdsr-main-helper.h"
#include "ns3/rushattackdsr-helper.h"

using namespace ns3;
using namespace rushattackdsr;

// -----------------------------------------------------------------------------
/**
 * \ingroup rushattackdsr
 * \defgroup rushattackdsr-test DSR routing module tests
 */


/**
 * \ingroup rushattackdsr-test
 * \ingroup tests
 *
 * \class RushattackdsrFsHeaderTest
 * \brief Unit test for DSR Fixed Size Header
 */
class RushattackdsrFsHeaderTest : public TestCase
{
public:
  RushattackdsrFsHeaderTest ();
  ~RushattackdsrFsHeaderTest ();
  virtual void
  DoRun (void);
};
RushattackdsrFsHeaderTest::RushattackdsrFsHeaderTest ()
  : TestCase ("DSR Fixed size Header")
{
}
RushattackdsrFsHeaderTest::~RushattackdsrFsHeaderTest ()
{
}
void
RushattackdsrFsHeaderTest::DoRun ()
{
  rushattackdsr::RushattackdsrRoutingHeader header;
  rushattackdsr::RushattackdsrOptionRreqHeader rreqHeader;
  header.AddRushattackdsrOption (rreqHeader); // has an alignment of 4n+0

  NS_TEST_EXPECT_MSG_EQ (header.GetSerializedSize () % 2, 0, "length of routing header is not a multiple of 4");
  Buffer buf;
  buf.AddAtStart (header.GetSerializedSize ());
  header.Serialize (buf.Begin ());

  const uint8_t* data = buf.PeekData ();
  NS_TEST_EXPECT_MSG_EQ (*(data + 8), rreqHeader.GetType (), "expect the rreqHeader after fixed size header");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup rushattackdsr-test
 * \ingroup tests
 *
 * \class RushattackdsrRreqHeaderTest
 * \brief Unit test for RREQ
 */
class RushattackdsrRreqHeaderTest : public TestCase
{
public:
  RushattackdsrRreqHeaderTest ();
  ~RushattackdsrRreqHeaderTest ();
  virtual void
  DoRun (void);
};
RushattackdsrRreqHeaderTest::RushattackdsrRreqHeaderTest ()
  : TestCase ("DSR RREQ")
{
}
RushattackdsrRreqHeaderTest::~RushattackdsrRreqHeaderTest ()
{
}
void
RushattackdsrRreqHeaderTest::DoRun ()
{
  rushattackdsr::RushattackdsrOptionRreqHeader h;
  std::vector<Ipv4Address> nodeList;
  nodeList.push_back (Ipv4Address ("1.1.1.0"));
  nodeList.push_back (Ipv4Address ("1.1.1.1"));
  nodeList.push_back (Ipv4Address ("1.1.1.2"));

  h.SetTarget (Ipv4Address ("1.1.1.3"));
  NS_TEST_EXPECT_MSG_EQ (h.GetTarget (), Ipv4Address ("1.1.1.3"), "trivial");
  h.SetNodesAddress (nodeList);
  NS_TEST_EXPECT_MSG_EQ (h.GetNodeAddress (0), Ipv4Address ("1.1.1.0"), "trivial");
  NS_TEST_EXPECT_MSG_EQ (h.GetNodeAddress (1), Ipv4Address ("1.1.1.1"), "trivial");
  NS_TEST_EXPECT_MSG_EQ (h.GetNodeAddress (2), Ipv4Address ("1.1.1.2"), "trivial");
  h.SetId (1);
  NS_TEST_EXPECT_MSG_EQ (h.GetId (), 1, "trivial");

  Ptr<Packet> p = Create<Packet> ();
  rushattackdsr::RushattackdsrRoutingHeader header;
  header.AddRushattackdsrOption (h);
  p->AddHeader (header);
  p->RemoveAtStart (8);
  rushattackdsr::RushattackdsrOptionRreqHeader h2;
  h2.SetNumberAddress (3);
  uint32_t bytes = p->RemoveHeader (h2);
  NS_TEST_EXPECT_MSG_EQ (bytes, 20, "Total RREP is 20 bytes long");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup rushattackdsr-test
 * \ingroup tests
 *
 * \class RushattackdsrRrepHeaderTest
 * \brief Unit test for RREP
 */
class RushattackdsrRrepHeaderTest : public TestCase
{
public:
  RushattackdsrRrepHeaderTest ();
  ~RushattackdsrRrepHeaderTest ();
  virtual void
  DoRun (void);
};
RushattackdsrRrepHeaderTest::RushattackdsrRrepHeaderTest ()
  : TestCase ("DSR RREP")
{
}
RushattackdsrRrepHeaderTest::~RushattackdsrRrepHeaderTest ()
{
}
void
RushattackdsrRrepHeaderTest::DoRun ()
{
  rushattackdsr::RushattackdsrOptionRrepHeader h;

  std::vector<Ipv4Address> nodeList;
  nodeList.push_back (Ipv4Address ("1.1.1.0"));
  nodeList.push_back (Ipv4Address ("1.1.1.1"));
  nodeList.push_back (Ipv4Address ("1.1.1.2"));
  h.SetNodesAddress (nodeList);
  NS_TEST_EXPECT_MSG_EQ (h.GetNodeAddress (0), Ipv4Address ("1.1.1.0"), "trivial");
  NS_TEST_EXPECT_MSG_EQ (h.GetNodeAddress (1), Ipv4Address ("1.1.1.1"), "trivial");
  NS_TEST_EXPECT_MSG_EQ (h.GetNodeAddress (2), Ipv4Address ("1.1.1.2"), "trivial");

  Ptr<Packet> p = Create<Packet> ();
  rushattackdsr::RushattackdsrRoutingHeader header;
  header.AddRushattackdsrOption (h);
  p->AddHeader (header);
  p->RemoveAtStart (8);
  rushattackdsr::RushattackdsrOptionRrepHeader h2;
  h2.SetNumberAddress (3);
  uint32_t bytes = p->RemoveHeader (h2);
  NS_TEST_EXPECT_MSG_EQ (bytes, 16, "Total RREP is 16 bytes long");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup rushattackdsr-test
 * \ingroup tests
 *
 * \class RushattackdsrSRHeaderTest
 * \brief Unit test for Source Route
 */
class RushattackdsrSRHeaderTest : public TestCase
{
public:
  RushattackdsrSRHeaderTest ();
  ~RushattackdsrSRHeaderTest ();
  virtual void
  DoRun (void);
};
RushattackdsrSRHeaderTest::RushattackdsrSRHeaderTest ()
  : TestCase ("DSR Source Route")
{
}
RushattackdsrSRHeaderTest::~RushattackdsrSRHeaderTest ()
{
}
void
RushattackdsrSRHeaderTest::DoRun ()
{
  rushattackdsr::RushattackdsrOptionSRHeader h;
  std::vector<Ipv4Address> nodeList;
  nodeList.push_back (Ipv4Address ("1.1.1.0"));
  nodeList.push_back (Ipv4Address ("1.1.1.1"));
  nodeList.push_back (Ipv4Address ("1.1.1.2"));
  h.SetNodesAddress (nodeList);
  NS_TEST_EXPECT_MSG_EQ (h.GetNodeAddress (0), Ipv4Address ("1.1.1.0"), "trivial");
  NS_TEST_EXPECT_MSG_EQ (h.GetNodeAddress (1), Ipv4Address ("1.1.1.1"), "trivial");
  NS_TEST_EXPECT_MSG_EQ (h.GetNodeAddress (2), Ipv4Address ("1.1.1.2"), "trivial");

  h.SetSalvage (1);
  NS_TEST_EXPECT_MSG_EQ (h.GetSalvage (), 1, "trivial");
  h.SetSegmentsLeft (2);
  NS_TEST_EXPECT_MSG_EQ (h.GetSegmentsLeft (), 2, "trivial");

  Ptr<Packet> p = Create<Packet> ();
  rushattackdsr::RushattackdsrRoutingHeader header;
  header.AddRushattackdsrOption (h);
  p->AddHeader (header);
  p->RemoveAtStart (8);
  rushattackdsr::RushattackdsrOptionSRHeader h2;
  h2.SetNumberAddress (3);
  uint32_t bytes = p->RemoveHeader (h2);
  NS_TEST_EXPECT_MSG_EQ (bytes, 16, "Total RREP is 16 bytes long");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup rushattackdsr-test
 * \ingroup tests
 *
 * \class RushattackdsrRerrHeaderTest
 * \brief Unit test for RERR
 */
class RushattackdsrRerrHeaderTest : public TestCase
{
public:
  RushattackdsrRerrHeaderTest ();
  ~RushattackdsrRerrHeaderTest ();
  virtual void
  DoRun (void);
};
RushattackdsrRerrHeaderTest::RushattackdsrRerrHeaderTest ()
  : TestCase ("DSR RERR")
{
}
RushattackdsrRerrHeaderTest::~RushattackdsrRerrHeaderTest ()
{
}
void
RushattackdsrRerrHeaderTest::DoRun ()
{
  rushattackdsr::RushattackdsrOptionRerrUnreachHeader h;
  h.SetErrorSrc (Ipv4Address ("1.1.1.0"));
  NS_TEST_EXPECT_MSG_EQ (h.GetErrorSrc (), Ipv4Address ("1.1.1.0"), "trivial");
  h.SetErrorDst (Ipv4Address ("1.1.1.1"));
  NS_TEST_EXPECT_MSG_EQ (h.GetErrorDst (), Ipv4Address ("1.1.1.1"), "trivial");
  h.SetSalvage (1);
  NS_TEST_EXPECT_MSG_EQ (h.GetSalvage (), 1, "trivial");
  h.SetUnreachNode (Ipv4Address ("1.1.1.2"));
  NS_TEST_EXPECT_MSG_EQ (h.GetUnreachNode (), Ipv4Address ("1.1.1.2"), "trivial");

  Ptr<Packet> p = Create<Packet> ();
  rushattackdsr::RushattackdsrRoutingHeader header;
  header.AddRushattackdsrOption (h);
  p->AddHeader (header);
  p->RemoveAtStart (8);
  rushattackdsr::RushattackdsrOptionRerrUnreachHeader h2;
  uint32_t bytes = p->RemoveHeader (h2);
  NS_TEST_EXPECT_MSG_EQ (bytes, 20, "Total RREP is 20 bytes long");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup rushattackdsr-test
 * \ingroup tests
 *
 * \class RushattackdsrAckReqHeaderTest
 * \brief Unit test for ACK-REQ
 */
class RushattackdsrAckReqHeaderTest : public TestCase
{
public:
  RushattackdsrAckReqHeaderTest ();
  ~RushattackdsrAckReqHeaderTest ();
  virtual void
  DoRun (void);
};
RushattackdsrAckReqHeaderTest::RushattackdsrAckReqHeaderTest ()
  : TestCase ("DSR Ack Req")
{
}
RushattackdsrAckReqHeaderTest::~RushattackdsrAckReqHeaderTest ()
{
}
void
RushattackdsrAckReqHeaderTest::DoRun ()
{
  rushattackdsr::RushattackdsrOptionAckReqHeader h;

  h.SetAckId (1);
  NS_TEST_EXPECT_MSG_EQ (h.GetAckId (), 1, "trivial");

  Ptr<Packet> p = Create<Packet> ();
  rushattackdsr::RushattackdsrRoutingHeader header;
  header.AddRushattackdsrOption (h);
  p->AddHeader (header);
  p->RemoveAtStart (8);
  p->AddHeader (header);
  rushattackdsr::RushattackdsrOptionAckReqHeader h2;
  p->RemoveAtStart (8);
  uint32_t bytes = p->RemoveHeader (h2);
  NS_TEST_EXPECT_MSG_EQ (bytes, 4, "Total RREP is 4 bytes long");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup rushattackdsr-test
 * \ingroup tests
 *
 * \class RushattackdsrAckHeaderTest
 * \brief Unit test for ACK
 */
class RushattackdsrAckHeaderTest : public TestCase
{
public:
  RushattackdsrAckHeaderTest ();
  ~RushattackdsrAckHeaderTest ();
  virtual void
  DoRun (void);
};
RushattackdsrAckHeaderTest::RushattackdsrAckHeaderTest ()
  : TestCase ("DSR ACK")
{
}
RushattackdsrAckHeaderTest::~RushattackdsrAckHeaderTest ()
{
}
void
RushattackdsrAckHeaderTest::DoRun ()
{
  rushattackdsr::RushattackdsrOptionAckHeader h;

  h.SetRealSrc (Ipv4Address ("1.1.1.0"));
  NS_TEST_EXPECT_MSG_EQ (h.GetRealSrc (), Ipv4Address ("1.1.1.0"), "trivial");
  h.SetRealDst (Ipv4Address ("1.1.1.1"));
  NS_TEST_EXPECT_MSG_EQ (h.GetRealDst (), Ipv4Address ("1.1.1.1"), "trivial");
  h.SetAckId (1);
  NS_TEST_EXPECT_MSG_EQ (h.GetAckId (), 1, "trivial");

  Ptr<Packet> p = Create<Packet> ();
  rushattackdsr::RushattackdsrRoutingHeader header;
  header.AddRushattackdsrOption (h);
  p->AddHeader (header);
  p->RemoveAtStart (8);
  p->AddHeader (header);
  rushattackdsr::RushattackdsrOptionAckHeader h2;
  p->RemoveAtStart (8);
  uint32_t bytes = p->RemoveHeader (h2);
  NS_TEST_EXPECT_MSG_EQ (bytes, 12, "Total RREP is 12 bytes long");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup rushattackdsr-test
 * \ingroup tests
 *
 * \class RushattackdsrCacheEntryTest
 * \brief Unit test for DSR route cache entry
 */
class RushattackdsrCacheEntryTest : public TestCase
{
public:
  RushattackdsrCacheEntryTest ();
  ~RushattackdsrCacheEntryTest ();
  virtual void
  DoRun (void);
};
RushattackdsrCacheEntryTest::RushattackdsrCacheEntryTest ()
  : TestCase ("DSR ACK")
{
}
RushattackdsrCacheEntryTest::~RushattackdsrCacheEntryTest ()
{
}
void
RushattackdsrCacheEntryTest::DoRun ()
{
  Ptr<rushattackdsr::RushattackdsrRouteCache> rcache = CreateObject<rushattackdsr::RushattackdsrRouteCache> ();
  std::vector<Ipv4Address> ip;
  ip.push_back (Ipv4Address ("0.0.0.0"));
  ip.push_back (Ipv4Address ("0.0.0.1"));
  Ipv4Address dst = Ipv4Address ("0.0.0.1");
  rushattackdsr::RushattackdsrRouteCacheEntry entry (ip, dst, Seconds (1));
  NS_TEST_EXPECT_MSG_EQ (entry.GetVector ().size (), 2, "trivial");
  NS_TEST_EXPECT_MSG_EQ (entry.GetDestination (), Ipv4Address ("0.0.0.1"), "trivial");
  NS_TEST_EXPECT_MSG_EQ (entry.GetExpireTime (), Seconds (1), "trivial");

  entry.SetExpireTime (Seconds (3));
  NS_TEST_EXPECT_MSG_EQ (entry.GetExpireTime (), Seconds (3), "trivial");
  entry.SetDestination (Ipv4Address ("1.1.1.1"));
  NS_TEST_EXPECT_MSG_EQ (entry.GetDestination (), Ipv4Address ("1.1.1.1"), "trivial");
  ip.push_back (Ipv4Address ("0.0.0.2"));
  entry.SetVector (ip);
  NS_TEST_EXPECT_MSG_EQ (entry.GetVector ().size (), 3, "trivial");

  NS_TEST_EXPECT_MSG_EQ (rcache->AddRoute (entry), true, "trivial");

  std::vector<Ipv4Address> ip2;
  ip2.push_back (Ipv4Address ("1.1.1.0"));
  ip2.push_back (Ipv4Address ("1.1.1.1"));
  Ipv4Address dst2 = Ipv4Address ("1.1.1.1");
  rushattackdsr::RushattackdsrRouteCacheEntry entry2 (ip2, dst2, Seconds (2));
  rushattackdsr::RushattackdsrRouteCacheEntry newEntry;
  NS_TEST_EXPECT_MSG_EQ (rcache->AddRoute (entry2), true, "trivial");
  NS_TEST_EXPECT_MSG_EQ (rcache->LookupRoute (dst2, newEntry), true, "trivial");
  NS_TEST_EXPECT_MSG_EQ (rcache->DeleteRoute (Ipv4Address ("2.2.2.2")), false, "trivial");

  NS_TEST_EXPECT_MSG_EQ (rcache->DeleteRoute (Ipv4Address ("1.1.1.1")), true, "trivial");
  NS_TEST_EXPECT_MSG_EQ (rcache->DeleteRoute (Ipv4Address ("1.1.1.1")), false, "trivial");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup rushattackdsr-test
 * \ingroup tests
 *
 * \class RushattackdsrSendBuffTest
 * \brief Unit test for Send Buffer
 */
class RushattackdsrSendBuffTest : public TestCase
{
public:
  RushattackdsrSendBuffTest ();
  ~RushattackdsrSendBuffTest ();
  virtual void
  DoRun (void);
  /// Check size limit function
  void CheckSizeLimit ();
  /// Check timeout function
  void CheckTimeout ();

  rushattackdsr::RushattackdsrSendBuffer q; ///< send buffer
};
RushattackdsrSendBuffTest::RushattackdsrSendBuffTest ()
  : TestCase ("DSR SendBuff"),
    q ()
{
}
RushattackdsrSendBuffTest::~RushattackdsrSendBuffTest ()
{
}
void
RushattackdsrSendBuffTest::DoRun ()
{
  q.SetMaxQueueLen (32);
  NS_TEST_EXPECT_MSG_EQ (q.GetMaxQueueLen (), 32, "trivial");
  q.SetSendBufferTimeout (Seconds (10));
  NS_TEST_EXPECT_MSG_EQ (q.GetSendBufferTimeout (), Seconds (10), "trivial");

  Ptr<const Packet> packet = Create<Packet> ();
  Ipv4Address dst1 = Ipv4Address ("0.0.0.1");
  rushattackdsr::RushattackdsrSendBuffEntry e1 (packet, dst1, Seconds (1));
  q.Enqueue (e1);
  q.Enqueue (e1);
  q.Enqueue (e1);
  NS_TEST_EXPECT_MSG_EQ (q.Find (Ipv4Address ("0.0.0.1")), true, "trivial");
  NS_TEST_EXPECT_MSG_EQ (q.Find (Ipv4Address ("1.1.1.1")), false, "trivial");
  NS_TEST_EXPECT_MSG_EQ (q.GetSize (), 1, "trivial");
  q.DropPacketWithDst (Ipv4Address ("0.0.0.1"));
  NS_TEST_EXPECT_MSG_EQ (q.Find (Ipv4Address ("0.0.0.1")), false, "trivial");
  NS_TEST_EXPECT_MSG_EQ (q.GetSize (), 0, "trivial");

  Ipv4Address dst2 = Ipv4Address ("0.0.0.2");
  rushattackdsr::RushattackdsrSendBuffEntry e2 (packet, dst2, Seconds (1));
  q.Enqueue (e1);
  q.Enqueue (e2);
  Ptr<Packet> packet2 = Create<Packet> ();
  rushattackdsr::RushattackdsrSendBuffEntry e3 (packet2, dst2, Seconds (1));
  NS_TEST_EXPECT_MSG_EQ (q.Dequeue (Ipv4Address ("0.0.0.3"), e3), false, "trivial");
  NS_TEST_EXPECT_MSG_EQ (q.Dequeue (Ipv4Address ("0.0.0.2"), e3), true, "trivial");
  NS_TEST_EXPECT_MSG_EQ (q.Find (Ipv4Address ("0.0.0.2")), false, "trivial");
  q.Enqueue (e2);
  q.Enqueue (e3);
  NS_TEST_EXPECT_MSG_EQ (q.GetSize (), 2, "trivial");
  Ptr<Packet> packet4 = Create<Packet> ();
  Ipv4Address dst4 = Ipv4Address ("0.0.0.4");
  rushattackdsr::RushattackdsrSendBuffEntry e4 (packet4, dst4, Seconds (20));
  q.Enqueue (e4);
  NS_TEST_EXPECT_MSG_EQ (q.GetSize (), 3, "trivial");
  q.DropPacketWithDst (Ipv4Address ("0.0.0.4"));
  NS_TEST_EXPECT_MSG_EQ (q.GetSize (), 2, "trivial");

  CheckSizeLimit ();

  Simulator::Schedule (q.GetSendBufferTimeout () + Seconds (1), &RushattackdsrSendBuffTest::CheckTimeout, this);

  Simulator::Run ();
  Simulator::Destroy ();
}
void
RushattackdsrSendBuffTest::CheckSizeLimit ()
{
  Ptr<Packet> packet = Create<Packet> ();
  Ipv4Address dst;
  rushattackdsr::RushattackdsrSendBuffEntry e1 (packet, dst, Seconds (1));

  for (uint32_t i = 0; i < q.GetMaxQueueLen (); ++i)
    {
      q.Enqueue (e1);
    }
  NS_TEST_EXPECT_MSG_EQ (q.GetSize (), 3, "trivial");

  for (uint32_t i = 0; i < q.GetMaxQueueLen (); ++i)
    {
      q.Enqueue (e1);
    }
  NS_TEST_EXPECT_MSG_EQ (q.GetSize (), 3, "trivial");
}
void
RushattackdsrSendBuffTest::CheckTimeout ()
{
  NS_TEST_EXPECT_MSG_EQ (q.GetSize (), 0, "Must be empty now");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup rushattackdsr-test
 * \ingroup tests
 *
 * \class RushattackdsrRreqTableTest
 * \brief Unit test for DSR routing table entry
 */
class RushattackdsrRreqTableTest : public TestCase
{
public:
  RushattackdsrRreqTableTest ();
  ~RushattackdsrRreqTableTest ();
  virtual void
  DoRun (void);
};
RushattackdsrRreqTableTest::RushattackdsrRreqTableTest ()
  : TestCase ("DSR RreqTable")
{
}
RushattackdsrRreqTableTest::~RushattackdsrRreqTableTest ()
{
}
void
RushattackdsrRreqTableTest::DoRun ()
{
  rushattackdsr::RreqTableEntry rt;

  rt.m_reqNo = 2;
  NS_TEST_EXPECT_MSG_EQ (rt.m_reqNo, 2, "trivial");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup rushattackdsr-test
 * \ingroup tests
 *
 * \class RushattackdsrTestSuite
 * \brief DSR test suite
 */
class RushattackdsrTestSuite : public TestSuite
{
public:
  RushattackdsrTestSuite () : TestSuite ("routing-rushattackdsr", UNIT)
  {
    AddTestCase (new RushattackdsrFsHeaderTest, TestCase::QUICK);
    AddTestCase (new RushattackdsrRreqHeaderTest, TestCase::QUICK);
    AddTestCase (new RushattackdsrRrepHeaderTest, TestCase::QUICK);
    AddTestCase (new RushattackdsrSRHeaderTest, TestCase::QUICK);
    AddTestCase (new RushattackdsrRerrHeaderTest, TestCase::QUICK);
    AddTestCase (new RushattackdsrAckReqHeaderTest, TestCase::QUICK);
    AddTestCase (new RushattackdsrAckHeaderTest, TestCase::QUICK);
    AddTestCase (new RushattackdsrCacheEntryTest, TestCase::QUICK);
    AddTestCase (new RushattackdsrSendBuffTest, TestCase::QUICK);
  }
} g_rushattackdsrTestSuite;