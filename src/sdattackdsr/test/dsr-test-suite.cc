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

#include "ns3/sdattackdsr-fs-header.h"
#include "ns3/sdattackdsr-option-header.h"
#include "ns3/sdattackdsr-rreq-table.h"
#include "ns3/sdattackdsr-rcache.h"
#include "ns3/sdattackdsr-rsendbuff.h"
#include "ns3/sdattackdsr-main-helper.h"
#include "ns3/sdattackdsr-helper.h"

using namespace ns3;
using namespace sdattackdsr;

// -----------------------------------------------------------------------------
/**
 * \ingroup sdattackdsr
 * \defgroup sdattackdsr-test DSR routing module tests
 */


/**
 * \ingroup sdattackdsr-test
 * \ingroup tests
 *
 * \class SdattackdsrFsHeaderTest
 * \brief Unit test for DSR Fixed Size Header
 */
class SdattackdsrFsHeaderTest : public TestCase
{
public:
  SdattackdsrFsHeaderTest ();
  ~SdattackdsrFsHeaderTest ();
  virtual void
  DoRun (void);
};
SdattackdsrFsHeaderTest::SdattackdsrFsHeaderTest ()
  : TestCase ("DSR Fixed size Header")
{
}
SdattackdsrFsHeaderTest::~SdattackdsrFsHeaderTest ()
{
}
void
SdattackdsrFsHeaderTest::DoRun ()
{
  sdattackdsr::SdattackdsrRoutingHeader header;
  sdattackdsr::SdattackdsrOptionRreqHeader rreqHeader;
  header.AddSdattackdsrOption (rreqHeader); // has an alignment of 4n+0

  NS_TEST_EXPECT_MSG_EQ (header.GetSerializedSize () % 2, 0, "length of routing header is not a multiple of 4");
  Buffer buf;
  buf.AddAtStart (header.GetSerializedSize ());
  header.Serialize (buf.Begin ());

  const uint8_t* data = buf.PeekData ();
  NS_TEST_EXPECT_MSG_EQ (*(data + 8), rreqHeader.GetType (), "expect the rreqHeader after fixed size header");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup sdattackdsr-test
 * \ingroup tests
 *
 * \class SdattackdsrRreqHeaderTest
 * \brief Unit test for RREQ
 */
class SdattackdsrRreqHeaderTest : public TestCase
{
public:
  SdattackdsrRreqHeaderTest ();
  ~SdattackdsrRreqHeaderTest ();
  virtual void
  DoRun (void);
};
SdattackdsrRreqHeaderTest::SdattackdsrRreqHeaderTest ()
  : TestCase ("DSR RREQ")
{
}
SdattackdsrRreqHeaderTest::~SdattackdsrRreqHeaderTest ()
{
}
void
SdattackdsrRreqHeaderTest::DoRun ()
{
  sdattackdsr::SdattackdsrOptionRreqHeader h;
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
  sdattackdsr::SdattackdsrRoutingHeader header;
  header.AddSdattackdsrOption (h);
  p->AddHeader (header);
  p->RemoveAtStart (8);
  sdattackdsr::SdattackdsrOptionRreqHeader h2;
  h2.SetNumberAddress (3);
  uint32_t bytes = p->RemoveHeader (h2);
  NS_TEST_EXPECT_MSG_EQ (bytes, 20, "Total RREP is 20 bytes long");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup sdattackdsr-test
 * \ingroup tests
 *
 * \class SdattackdsrRrepHeaderTest
 * \brief Unit test for RREP
 */
class SdattackdsrRrepHeaderTest : public TestCase
{
public:
  SdattackdsrRrepHeaderTest ();
  ~SdattackdsrRrepHeaderTest ();
  virtual void
  DoRun (void);
};
SdattackdsrRrepHeaderTest::SdattackdsrRrepHeaderTest ()
  : TestCase ("DSR RREP")
{
}
SdattackdsrRrepHeaderTest::~SdattackdsrRrepHeaderTest ()
{
}
void
SdattackdsrRrepHeaderTest::DoRun ()
{
  sdattackdsr::SdattackdsrOptionRrepHeader h;

  std::vector<Ipv4Address> nodeList;
  nodeList.push_back (Ipv4Address ("1.1.1.0"));
  nodeList.push_back (Ipv4Address ("1.1.1.1"));
  nodeList.push_back (Ipv4Address ("1.1.1.2"));
  h.SetNodesAddress (nodeList);
  NS_TEST_EXPECT_MSG_EQ (h.GetNodeAddress (0), Ipv4Address ("1.1.1.0"), "trivial");
  NS_TEST_EXPECT_MSG_EQ (h.GetNodeAddress (1), Ipv4Address ("1.1.1.1"), "trivial");
  NS_TEST_EXPECT_MSG_EQ (h.GetNodeAddress (2), Ipv4Address ("1.1.1.2"), "trivial");

  Ptr<Packet> p = Create<Packet> ();
  sdattackdsr::SdattackdsrRoutingHeader header;
  header.AddSdattackdsrOption (h);
  p->AddHeader (header);
  p->RemoveAtStart (8);
  sdattackdsr::SdattackdsrOptionRrepHeader h2;
  h2.SetNumberAddress (3);
  uint32_t bytes = p->RemoveHeader (h2);
  NS_TEST_EXPECT_MSG_EQ (bytes, 16, "Total RREP is 16 bytes long");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup sdattackdsr-test
 * \ingroup tests
 *
 * \class SdattackdsrSRHeaderTest
 * \brief Unit test for Source Route
 */
class SdattackdsrSRHeaderTest : public TestCase
{
public:
  SdattackdsrSRHeaderTest ();
  ~SdattackdsrSRHeaderTest ();
  virtual void
  DoRun (void);
};
SdattackdsrSRHeaderTest::SdattackdsrSRHeaderTest ()
  : TestCase ("DSR Source Route")
{
}
SdattackdsrSRHeaderTest::~SdattackdsrSRHeaderTest ()
{
}
void
SdattackdsrSRHeaderTest::DoRun ()
{
  sdattackdsr::SdattackdsrOptionSRHeader h;
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
  sdattackdsr::SdattackdsrRoutingHeader header;
  header.AddSdattackdsrOption (h);
  p->AddHeader (header);
  p->RemoveAtStart (8);
  sdattackdsr::SdattackdsrOptionSRHeader h2;
  h2.SetNumberAddress (3);
  uint32_t bytes = p->RemoveHeader (h2);
  NS_TEST_EXPECT_MSG_EQ (bytes, 16, "Total RREP is 16 bytes long");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup sdattackdsr-test
 * \ingroup tests
 *
 * \class SdattackdsrRerrHeaderTest
 * \brief Unit test for RERR
 */
class SdattackdsrRerrHeaderTest : public TestCase
{
public:
  SdattackdsrRerrHeaderTest ();
  ~SdattackdsrRerrHeaderTest ();
  virtual void
  DoRun (void);
};
SdattackdsrRerrHeaderTest::SdattackdsrRerrHeaderTest ()
  : TestCase ("DSR RERR")
{
}
SdattackdsrRerrHeaderTest::~SdattackdsrRerrHeaderTest ()
{
}
void
SdattackdsrRerrHeaderTest::DoRun ()
{
  sdattackdsr::SdattackdsrOptionRerrUnreachHeader h;
  h.SetErrorSrc (Ipv4Address ("1.1.1.0"));
  NS_TEST_EXPECT_MSG_EQ (h.GetErrorSrc (), Ipv4Address ("1.1.1.0"), "trivial");
  h.SetErrorDst (Ipv4Address ("1.1.1.1"));
  NS_TEST_EXPECT_MSG_EQ (h.GetErrorDst (), Ipv4Address ("1.1.1.1"), "trivial");
  h.SetSalvage (1);
  NS_TEST_EXPECT_MSG_EQ (h.GetSalvage (), 1, "trivial");
  h.SetUnreachNode (Ipv4Address ("1.1.1.2"));
  NS_TEST_EXPECT_MSG_EQ (h.GetUnreachNode (), Ipv4Address ("1.1.1.2"), "trivial");

  Ptr<Packet> p = Create<Packet> ();
  sdattackdsr::SdattackdsrRoutingHeader header;
  header.AddSdattackdsrOption (h);
  p->AddHeader (header);
  p->RemoveAtStart (8);
  sdattackdsr::SdattackdsrOptionRerrUnreachHeader h2;
  uint32_t bytes = p->RemoveHeader (h2);
  NS_TEST_EXPECT_MSG_EQ (bytes, 20, "Total RREP is 20 bytes long");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup sdattackdsr-test
 * \ingroup tests
 *
 * \class SdattackdsrAckReqHeaderTest
 * \brief Unit test for ACK-REQ
 */
class SdattackdsrAckReqHeaderTest : public TestCase
{
public:
  SdattackdsrAckReqHeaderTest ();
  ~SdattackdsrAckReqHeaderTest ();
  virtual void
  DoRun (void);
};
SdattackdsrAckReqHeaderTest::SdattackdsrAckReqHeaderTest ()
  : TestCase ("DSR Ack Req")
{
}
SdattackdsrAckReqHeaderTest::~SdattackdsrAckReqHeaderTest ()
{
}
void
SdattackdsrAckReqHeaderTest::DoRun ()
{
  sdattackdsr::SdattackdsrOptionAckReqHeader h;

  h.SetAckId (1);
  NS_TEST_EXPECT_MSG_EQ (h.GetAckId (), 1, "trivial");

  Ptr<Packet> p = Create<Packet> ();
  sdattackdsr::SdattackdsrRoutingHeader header;
  header.AddSdattackdsrOption (h);
  p->AddHeader (header);
  p->RemoveAtStart (8);
  p->AddHeader (header);
  sdattackdsr::SdattackdsrOptionAckReqHeader h2;
  p->RemoveAtStart (8);
  uint32_t bytes = p->RemoveHeader (h2);
  NS_TEST_EXPECT_MSG_EQ (bytes, 4, "Total RREP is 4 bytes long");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup sdattackdsr-test
 * \ingroup tests
 *
 * \class SdattackdsrAckHeaderTest
 * \brief Unit test for ACK
 */
class SdattackdsrAckHeaderTest : public TestCase
{
public:
  SdattackdsrAckHeaderTest ();
  ~SdattackdsrAckHeaderTest ();
  virtual void
  DoRun (void);
};
SdattackdsrAckHeaderTest::SdattackdsrAckHeaderTest ()
  : TestCase ("DSR ACK")
{
}
SdattackdsrAckHeaderTest::~SdattackdsrAckHeaderTest ()
{
}
void
SdattackdsrAckHeaderTest::DoRun ()
{
  sdattackdsr::SdattackdsrOptionAckHeader h;

  h.SetRealSrc (Ipv4Address ("1.1.1.0"));
  NS_TEST_EXPECT_MSG_EQ (h.GetRealSrc (), Ipv4Address ("1.1.1.0"), "trivial");
  h.SetRealDst (Ipv4Address ("1.1.1.1"));
  NS_TEST_EXPECT_MSG_EQ (h.GetRealDst (), Ipv4Address ("1.1.1.1"), "trivial");
  h.SetAckId (1);
  NS_TEST_EXPECT_MSG_EQ (h.GetAckId (), 1, "trivial");

  Ptr<Packet> p = Create<Packet> ();
  sdattackdsr::SdattackdsrRoutingHeader header;
  header.AddSdattackdsrOption (h);
  p->AddHeader (header);
  p->RemoveAtStart (8);
  p->AddHeader (header);
  sdattackdsr::SdattackdsrOptionAckHeader h2;
  p->RemoveAtStart (8);
  uint32_t bytes = p->RemoveHeader (h2);
  NS_TEST_EXPECT_MSG_EQ (bytes, 12, "Total RREP is 12 bytes long");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup sdattackdsr-test
 * \ingroup tests
 *
 * \class SdattackdsrCacheEntryTest
 * \brief Unit test for DSR route cache entry
 */
class SdattackdsrCacheEntryTest : public TestCase
{
public:
  SdattackdsrCacheEntryTest ();
  ~SdattackdsrCacheEntryTest ();
  virtual void
  DoRun (void);
};
SdattackdsrCacheEntryTest::SdattackdsrCacheEntryTest ()
  : TestCase ("DSR ACK")
{
}
SdattackdsrCacheEntryTest::~SdattackdsrCacheEntryTest ()
{
}
void
SdattackdsrCacheEntryTest::DoRun ()
{
  Ptr<sdattackdsr::SdattackdsrRouteCache> rcache = CreateObject<sdattackdsr::SdattackdsrRouteCache> ();
  std::vector<Ipv4Address> ip;
  ip.push_back (Ipv4Address ("0.0.0.0"));
  ip.push_back (Ipv4Address ("0.0.0.1"));
  Ipv4Address dst = Ipv4Address ("0.0.0.1");
  sdattackdsr::SdattackdsrRouteCacheEntry entry (ip, dst, Seconds (1));
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
  sdattackdsr::SdattackdsrRouteCacheEntry entry2 (ip2, dst2, Seconds (2));
  sdattackdsr::SdattackdsrRouteCacheEntry newEntry;
  NS_TEST_EXPECT_MSG_EQ (rcache->AddRoute (entry2), true, "trivial");
  NS_TEST_EXPECT_MSG_EQ (rcache->LookupRoute (dst2, newEntry), true, "trivial");
  NS_TEST_EXPECT_MSG_EQ (rcache->DeleteRoute (Ipv4Address ("2.2.2.2")), false, "trivial");

  NS_TEST_EXPECT_MSG_EQ (rcache->DeleteRoute (Ipv4Address ("1.1.1.1")), true, "trivial");
  NS_TEST_EXPECT_MSG_EQ (rcache->DeleteRoute (Ipv4Address ("1.1.1.1")), false, "trivial");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup sdattackdsr-test
 * \ingroup tests
 *
 * \class SdattackdsrSendBuffTest
 * \brief Unit test for Send Buffer
 */
class SdattackdsrSendBuffTest : public TestCase
{
public:
  SdattackdsrSendBuffTest ();
  ~SdattackdsrSendBuffTest ();
  virtual void
  DoRun (void);
  /// Check size limit function
  void CheckSizeLimit ();
  /// Check timeout function
  void CheckTimeout ();

  sdattackdsr::SdattackdsrSendBuffer q; ///< send buffer
};
SdattackdsrSendBuffTest::SdattackdsrSendBuffTest ()
  : TestCase ("DSR SendBuff"),
    q ()
{
}
SdattackdsrSendBuffTest::~SdattackdsrSendBuffTest ()
{
}
void
SdattackdsrSendBuffTest::DoRun ()
{
  q.SetMaxQueueLen (32);
  NS_TEST_EXPECT_MSG_EQ (q.GetMaxQueueLen (), 32, "trivial");
  q.SetSendBufferTimeout (Seconds (10));
  NS_TEST_EXPECT_MSG_EQ (q.GetSendBufferTimeout (), Seconds (10), "trivial");

  Ptr<const Packet> packet = Create<Packet> ();
  Ipv4Address dst1 = Ipv4Address ("0.0.0.1");
  sdattackdsr::SdattackdsrSendBuffEntry e1 (packet, dst1, Seconds (1));
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
  sdattackdsr::SdattackdsrSendBuffEntry e2 (packet, dst2, Seconds (1));
  q.Enqueue (e1);
  q.Enqueue (e2);
  Ptr<Packet> packet2 = Create<Packet> ();
  sdattackdsr::SdattackdsrSendBuffEntry e3 (packet2, dst2, Seconds (1));
  NS_TEST_EXPECT_MSG_EQ (q.Dequeue (Ipv4Address ("0.0.0.3"), e3), false, "trivial");
  NS_TEST_EXPECT_MSG_EQ (q.Dequeue (Ipv4Address ("0.0.0.2"), e3), true, "trivial");
  NS_TEST_EXPECT_MSG_EQ (q.Find (Ipv4Address ("0.0.0.2")), false, "trivial");
  q.Enqueue (e2);
  q.Enqueue (e3);
  NS_TEST_EXPECT_MSG_EQ (q.GetSize (), 2, "trivial");
  Ptr<Packet> packet4 = Create<Packet> ();
  Ipv4Address dst4 = Ipv4Address ("0.0.0.4");
  sdattackdsr::SdattackdsrSendBuffEntry e4 (packet4, dst4, Seconds (20));
  q.Enqueue (e4);
  NS_TEST_EXPECT_MSG_EQ (q.GetSize (), 3, "trivial");
  q.DropPacketWithDst (Ipv4Address ("0.0.0.4"));
  NS_TEST_EXPECT_MSG_EQ (q.GetSize (), 2, "trivial");

  CheckSizeLimit ();

  Simulator::Schedule (q.GetSendBufferTimeout () + Seconds (1), &SdattackdsrSendBuffTest::CheckTimeout, this);

  Simulator::Run ();
  Simulator::Destroy ();
}
void
SdattackdsrSendBuffTest::CheckSizeLimit ()
{
  Ptr<Packet> packet = Create<Packet> ();
  Ipv4Address dst;
  sdattackdsr::SdattackdsrSendBuffEntry e1 (packet, dst, Seconds (1));

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
SdattackdsrSendBuffTest::CheckTimeout ()
{
  NS_TEST_EXPECT_MSG_EQ (q.GetSize (), 0, "Must be empty now");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup sdattackdsr-test
 * \ingroup tests
 *
 * \class SdattackdsrRreqTableTest
 * \brief Unit test for DSR routing table entry
 */
class SdattackdsrRreqTableTest : public TestCase
{
public:
  SdattackdsrRreqTableTest ();
  ~SdattackdsrRreqTableTest ();
  virtual void
  DoRun (void);
};
SdattackdsrRreqTableTest::SdattackdsrRreqTableTest ()
  : TestCase ("DSR RreqTable")
{
}
SdattackdsrRreqTableTest::~SdattackdsrRreqTableTest ()
{
}
void
SdattackdsrRreqTableTest::DoRun ()
{
  sdattackdsr::RreqTableEntry rt;

  rt.m_reqNo = 2;
  NS_TEST_EXPECT_MSG_EQ (rt.m_reqNo, 2, "trivial");
}
// -----------------------------------------------------------------------------
/**
 * \ingroup sdattackdsr-test
 * \ingroup tests
 *
 * \class SdattackdsrTestSuite
 * \brief DSR test suite
 */
class SdattackdsrTestSuite : public TestSuite
{
public:
  SdattackdsrTestSuite () : TestSuite ("routing-sdattackdsr", UNIT)
  {
    AddTestCase (new SdattackdsrFsHeaderTest, TestCase::QUICK);
    AddTestCase (new SdattackdsrRreqHeaderTest, TestCase::QUICK);
    AddTestCase (new SdattackdsrRrepHeaderTest, TestCase::QUICK);
    AddTestCase (new SdattackdsrSRHeaderTest, TestCase::QUICK);
    AddTestCase (new SdattackdsrRerrHeaderTest, TestCase::QUICK);
    AddTestCase (new SdattackdsrAckReqHeaderTest, TestCase::QUICK);
    AddTestCase (new SdattackdsrAckHeaderTest, TestCase::QUICK);
    AddTestCase (new SdattackdsrCacheEntryTest, TestCase::QUICK);
    AddTestCase (new SdattackdsrSendBuffTest, TestCase::QUICK);
  }
} g_sdattackdsrTestSuite;
