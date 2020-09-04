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

#include "sdattackdsr-main-helper.h"
#include "ns3/sdattackdsr-helper.h"
#include "ns3/sdattackdsr-routing.h"
#include "ns3/sdattackdsr-rcache.h"
#include "ns3/sdattackdsr-rreq-table.h"
#include "ns3/node-list.h"
#include "ns3/names.h"
#include "ns3/log.h"
#include "ns3/ptr.h"
#include "ns3/node.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("SdattackdsrMainHelper");

SdattackdsrMainHelper::SdattackdsrMainHelper ()
  : m_sdattackdsrHelper (0)
{
  NS_LOG_FUNCTION (this);
}

SdattackdsrMainHelper::SdattackdsrMainHelper (const SdattackdsrMainHelper &o)
{
  NS_LOG_FUNCTION (this);
  m_sdattackdsrHelper = o.m_sdattackdsrHelper->Copy ();
}

SdattackdsrMainHelper::~SdattackdsrMainHelper ()
{
  NS_LOG_FUNCTION (this);
  delete m_sdattackdsrHelper;
}

SdattackdsrMainHelper &
SdattackdsrMainHelper::operator = (const SdattackdsrMainHelper &o)
{
  if (this == &o)
    {
      return *this;
    }
  m_sdattackdsrHelper = o.m_sdattackdsrHelper->Copy ();
  return *this;
}

void
SdattackdsrMainHelper::Install (SdattackdsrHelper &sdattackdsrHelper, NodeContainer nodes)
{
  NS_LOG_DEBUG ("Passed node container");
  delete m_sdattackdsrHelper;
  m_sdattackdsrHelper = sdattackdsrHelper.Copy ();
  for (NodeContainer::Iterator i = nodes.Begin (); i != nodes.End (); ++i)
    {
      Install (*i);
    }
}

void
SdattackdsrMainHelper::Install (Ptr<Node> node)
{
  NS_LOG_FUNCTION (node);
  Ptr<ns3::sdattackdsr::SdattackdsrRouting> sdattackdsr = m_sdattackdsrHelper->Create (node);
//  Ptr<ns3::sdattackdsr::RouteCache> routeCache = CreateObject<ns3::sdattackdsr::RouteCache> ();
//  Ptr<ns3::sdattackdsr::RreqTable> rreqTable = CreateObject<ns3::sdattackdsr::RreqTable> ();
//  sdattackdsr->SetRouteCache (routeCache);
//  sdattackdsr->SetRequestTable (rreqTable);
  sdattackdsr->SetNode (node);
//  node->AggregateObject (routeCache);
//  node->AggregateObject (rreqTable);
}

void
SdattackdsrMainHelper::SetSdattackdsrHelper (SdattackdsrHelper &sdattackdsrHelper)
{
  NS_LOG_FUNCTION (this);
  delete m_sdattackdsrHelper;
  m_sdattackdsrHelper = sdattackdsrHelper.Copy ();
}

} // namespace ns3
