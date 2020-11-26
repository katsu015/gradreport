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

#include "rushattackdsr-main-helper.h"
#include "ns3/rushattackdsr-helper.h"
#include "ns3/rushattackdsr-routing.h"
#include "ns3/rushattackdsr-rcache.h"
#include "ns3/rushattackdsr-rreq-table.h"
#include "ns3/node-list.h"
#include "ns3/names.h"
#include "ns3/log.h"
#include "ns3/ptr.h"
#include "ns3/node.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RushattackdsrMainHelper");

RushattackdsrMainHelper::RushattackdsrMainHelper ()
  : m_rushattackdsrHelper (0)
{
  NS_LOG_FUNCTION (this);
}

RushattackdsrMainHelper::RushattackdsrMainHelper (const RushattackdsrMainHelper &o)
{
  NS_LOG_FUNCTION (this);
  m_rushattackdsrHelper = o.m_rushattackdsrHelper->Copy ();
}

RushattackdsrMainHelper::~RushattackdsrMainHelper ()
{
  NS_LOG_FUNCTION (this);
  delete m_rushattackdsrHelper;
}

RushattackdsrMainHelper &
RushattackdsrMainHelper::operator = (const RushattackdsrMainHelper &o)
{
  if (this == &o)
    {
      return *this;
    }
  m_rushattackdsrHelper = o.m_rushattackdsrHelper->Copy ();
  return *this;
}

void
RushattackdsrMainHelper::Install (RushattackdsrHelper &rushattackdsrHelper, NodeContainer nodes)
{
  NS_LOG_DEBUG ("Passed node container");
  delete m_rushattackdsrHelper;
  m_rushattackdsrHelper = rushattackdsrHelper.Copy ();
  for (NodeContainer::Iterator i = nodes.Begin (); i != nodes.End (); ++i)
    {
      Install (*i);
    }
}

void
RushattackdsrMainHelper::Install (Ptr<Node> node)
{
  NS_LOG_FUNCTION (node);
  Ptr<ns3::rushattackdsr::RushattackdsrRouting> rushattackdsr = m_rushattackdsrHelper->Create (node);
//  Ptr<ns3::rushattackdsr::RouteCache> routeCache = CreateObject<ns3::rushattackdsr::RouteCache> ();
//  Ptr<ns3::rushattackdsr::RreqTable> rreqTable = CreateObject<ns3::rushattackdsr::RreqTable> ();
//  rushattackdsr->SetRouteCache (routeCache);
//  rushattackdsr->SetRequestTable (rreqTable);
  rushattackdsr->SetNode (node);
//  node->AggregateObject (routeCache);
//  node->AggregateObject (rreqTable);
}

void
RushattackdsrMainHelper::SetRushattackdsrHelper (RushattackdsrHelper &rushattackdsrHelper)
{
  NS_LOG_FUNCTION (this);
  delete m_rushattackdsrHelper;
  m_rushattackdsrHelper = rushattackdsrHelper.Copy ();
}

} // namespace ns3
