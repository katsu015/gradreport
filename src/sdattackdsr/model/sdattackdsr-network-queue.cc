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

#include "sdattackdsr-network-queue.h"
#include "ns3/test.h"
#include <map>
#include <algorithm>
#include <functional>
#include "ns3/log.h"
#include "ns3/ipv4-route.h"
#include "ns3/socket.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("SdattackdsrNetworkQueue");

namespace sdattackdsr {

NS_OBJECT_ENSURE_REGISTERED (SdattackdsrNetworkQueue);

TypeId
SdattackdsrNetworkQueue::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::sdattackdsr::SdattackdsrNetworkQueue")
    .SetParent<Object> ()
    .SetGroupName ("Sdattackdsr")
    .AddConstructor<SdattackdsrNetworkQueue>  ()
  ;
  return tid;
}

SdattackdsrNetworkQueue::SdattackdsrNetworkQueue (uint32_t maxLen, Time maxDelay)
  : m_size (0),
    m_maxSize (maxLen),
    m_maxDelay (maxDelay)
{
  NS_LOG_FUNCTION (this);
}

SdattackdsrNetworkQueue::SdattackdsrNetworkQueue () : m_size (0)
{
  NS_LOG_FUNCTION (this);
}

SdattackdsrNetworkQueue::~SdattackdsrNetworkQueue ()
{
  NS_LOG_FUNCTION (this);
  Flush ();
}

void
SdattackdsrNetworkQueue::SetMaxNetworkSize (uint32_t maxSize)
{
  m_maxSize = maxSize;
}

void
SdattackdsrNetworkQueue::SetMaxNetworkDelay (Time delay)
{
  m_maxDelay = delay;
}

uint32_t
SdattackdsrNetworkQueue::GetMaxNetworkSize (void) const
{
  return m_maxSize;
}

Time
SdattackdsrNetworkQueue::GetMaxNetworkDelay (void) const
{
  return m_maxDelay;
}

bool
SdattackdsrNetworkQueue::FindPacketWithNexthop (Ipv4Address nextHop, SdattackdsrNetworkQueueEntry & entry)
{
  Cleanup ();
  for (std::vector<SdattackdsrNetworkQueueEntry>::iterator i = m_sdattackdsrNetworkQueue.begin (); i != m_sdattackdsrNetworkQueue.end (); ++i)
    {
      if (i->GetNextHopAddress () == nextHop)
        {
          entry = *i;
          i = m_sdattackdsrNetworkQueue.erase (i);
          return true;
        }
    }
  return false;
}

bool
SdattackdsrNetworkQueue::Find (Ipv4Address nextHop)
{
  Cleanup ();
  for (std::vector<SdattackdsrNetworkQueueEntry>::iterator i = m_sdattackdsrNetworkQueue.begin (); i != m_sdattackdsrNetworkQueue.end (); ++i)
    {
      if (i->GetNextHopAddress () == nextHop)
        {
          return true;
        }
    }
  return false;
}

bool
SdattackdsrNetworkQueue::Enqueue (SdattackdsrNetworkQueueEntry & entry)
{
  NS_LOG_FUNCTION (this << m_size << m_maxSize);
  if (m_size >= m_maxSize)
    {
      return false;
    }
  Time now = Simulator::Now ();
  entry.SetInsertedTimeStamp (now);
  m_sdattackdsrNetworkQueue.push_back (entry);
  m_size++;
  NS_LOG_LOGIC ("The network queue size is " << m_size);
  return true;
}

bool
SdattackdsrNetworkQueue::Dequeue (SdattackdsrNetworkQueueEntry & entry)
{
  NS_LOG_FUNCTION (this);
  Cleanup ();
  std::vector<SdattackdsrNetworkQueueEntry>::iterator i = m_sdattackdsrNetworkQueue.begin ();
  if (i == m_sdattackdsrNetworkQueue.end ())
    {
      // no elements in array
      NS_LOG_LOGIC ("No queued packet in the network queue");
      return false;
    }
  entry = *i;
  m_sdattackdsrNetworkQueue.erase (i);
  m_size--;
  return true;
}

void
SdattackdsrNetworkQueue::Cleanup (void)
{
  NS_LOG_FUNCTION (this);
  if (m_sdattackdsrNetworkQueue.empty ())
    {
      return;
    }

  Time now = Simulator::Now ();
  uint32_t n = 0;
  for (std::vector<SdattackdsrNetworkQueueEntry>::iterator i = m_sdattackdsrNetworkQueue.begin (); i != m_sdattackdsrNetworkQueue.end (); )
    {
      if (i->GetInsertedTimeStamp () + m_maxDelay > now)
        {
          i++;
        }
      else
        {
          NS_LOG_LOGIC ("Outdated packet");
          i = m_sdattackdsrNetworkQueue.erase (i);
          n++;
        }
    }
  m_size -= n;
}

uint32_t
SdattackdsrNetworkQueue::GetSize ()
{
  NS_LOG_FUNCTION (this);
  return m_size;
}

void
SdattackdsrNetworkQueue::Flush (void)
{
  NS_LOG_FUNCTION (this);
  m_sdattackdsrNetworkQueue.erase (m_sdattackdsrNetworkQueue.begin (), m_sdattackdsrNetworkQueue.end ());
  m_size = 0;
}

}  // namespace sdattackdsr
}  // namespace ns3
