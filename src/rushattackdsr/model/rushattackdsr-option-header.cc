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

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/header.h"
#include "rushattackdsr-option-header.h"
#include "ns3/ipv4-address.h"
#include "ns3/address-utils.h"
#include "ns3/packet.h"
#include "ns3/enum.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("RushattackdsrOptionHeader");

namespace rushattackdsr {

NS_OBJECT_ENSURE_REGISTERED (RushattackdsrOptionHeader);

TypeId RushattackdsrOptionHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rushattackdsr::RushattackdsrOptionHeader")
    .AddConstructor<RushattackdsrOptionHeader> ()
    .SetParent<Header> ()
    .SetGroupName ("Rushattackdsr")
  ;
  return tid;
}

TypeId RushattackdsrOptionHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

RushattackdsrOptionHeader::RushattackdsrOptionHeader ()
  : m_type (0),
    m_length (0)
{
}

RushattackdsrOptionHeader::~RushattackdsrOptionHeader ()
{
}

void RushattackdsrOptionHeader::SetType (uint8_t type)
{
  m_type = type;
}

uint8_t RushattackdsrOptionHeader::GetType () const
{
  return m_type;
}

void RushattackdsrOptionHeader::SetLength (uint8_t length)
{
  m_length = length;
}

uint8_t RushattackdsrOptionHeader::GetLength () const
{
  return m_length;
}

void RushattackdsrOptionHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)m_type << " length = " << (uint32_t)m_length << " )";
}

uint32_t RushattackdsrOptionHeader::GetSerializedSize () const
{
  return m_length + 2;
}

void RushattackdsrOptionHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (m_type);
  i.WriteU8 (m_length);
  i.Write (m_data.Begin (), m_data.End ());
}

uint32_t RushattackdsrOptionHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  m_type = i.ReadU8 ();
  m_length = i.ReadU8 ();

  m_data = Buffer ();
  m_data.AddAtEnd (m_length);
  Buffer::Iterator dataStart = i;
  i.Next (m_length);
  Buffer::Iterator dataEnd = i;
  m_data.Begin ().Write (dataStart, dataEnd);

  return GetSerializedSize ();
}

RushattackdsrOptionHeader::Alignment RushattackdsrOptionHeader::GetAlignment () const
{
  Alignment retVal = { 1, 0 };
  return retVal;
}

NS_OBJECT_ENSURE_REGISTERED (RushattackdsrOptionPad1Header);

TypeId RushattackdsrOptionPad1Header::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rushattackdsr::RushattackdsrOptionPad1Header")
    .AddConstructor<RushattackdsrOptionPad1Header> ()
    .SetParent<RushattackdsrOptionHeader> ()
    .SetGroupName ("Rushattackdsr")
  ;
  return tid;
}

TypeId RushattackdsrOptionPad1Header::GetInstanceTypeId () const
{
  return GetTypeId ();
}

RushattackdsrOptionPad1Header::RushattackdsrOptionPad1Header ()
{
  SetType (224);
}

RushattackdsrOptionPad1Header::~RushattackdsrOptionPad1Header ()
{
}

void RushattackdsrOptionPad1Header::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " )";
}

uint32_t RushattackdsrOptionPad1Header::GetSerializedSize () const
{
  return 1;
}

void RushattackdsrOptionPad1Header::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
}

uint32_t RushattackdsrOptionPad1Header::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());

  return GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (RushattackdsrOptionPadnHeader);

TypeId RushattackdsrOptionPadnHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rushattackdsr::RushattackdsrOptionPadnHeader")
    .AddConstructor<RushattackdsrOptionPadnHeader> ()
    .SetParent<RushattackdsrOptionHeader> ()
    .SetGroupName ("Rushattackdsr")
  ;
  return tid;
}

TypeId RushattackdsrOptionPadnHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

RushattackdsrOptionPadnHeader::RushattackdsrOptionPadnHeader (uint32_t pad)
{
  SetType (0);
  NS_ASSERT_MSG (pad >= 2, "PadN must be at least 2 bytes long");
  SetLength (pad - 2);
}

RushattackdsrOptionPadnHeader::~RushattackdsrOptionPadnHeader ()
{
}

void RushattackdsrOptionPadnHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength () << " )";
}

uint32_t RushattackdsrOptionPadnHeader::GetSerializedSize () const
{
  return GetLength () + 2;
}

void RushattackdsrOptionPadnHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetLength ());

  for (int padding = 0; padding < GetLength (); padding++)
    {
      i.WriteU8 (0);
    }
}

uint32_t RushattackdsrOptionPadnHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetLength (i.ReadU8 ());

  return GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (RushattackdsrOptionRreqHeader);

TypeId RushattackdsrOptionRreqHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rushattackdsr::RushattackdsrOptionRreqHeader")
    .AddConstructor<RushattackdsrOptionRreqHeader> ()
    .SetParent<RushattackdsrOptionHeader> ()
    .SetGroupName ("Rushattackdsr")
  ;
  return tid;
}

TypeId RushattackdsrOptionRreqHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

RushattackdsrOptionRreqHeader::RushattackdsrOptionRreqHeader ()
  : m_ipv4Address (0)
{
  SetType (1);
  SetLength (6 + m_ipv4Address.size () * 4);
}

RushattackdsrOptionRreqHeader::~RushattackdsrOptionRreqHeader ()
{
}

void RushattackdsrOptionRreqHeader::SetNumberAddress (uint8_t n)
{
  m_ipv4Address.clear ();
  m_ipv4Address.assign (n, Ipv4Address ());
}

Ipv4Address RushattackdsrOptionRreqHeader::GetTarget ()
{
  return m_target;
}

void RushattackdsrOptionRreqHeader::SetTarget (Ipv4Address target)
{
  m_target = target;
}

void RushattackdsrOptionRreqHeader::AddNodeAddress (Ipv4Address ipv4)
{
  m_ipv4Address.push_back (ipv4);
  SetLength (6 + m_ipv4Address.size () * 4);
}

void RushattackdsrOptionRreqHeader::SetNodesAddress (std::vector<Ipv4Address> ipv4Address)
{
  m_ipv4Address = ipv4Address;
  SetLength (6 + m_ipv4Address.size () * 4);
}

std::vector<Ipv4Address> RushattackdsrOptionRreqHeader::GetNodesAddresses () const
{
  return m_ipv4Address;
}

uint32_t RushattackdsrOptionRreqHeader::GetNodesNumber () const
{
  return m_ipv4Address.size ();
}

void RushattackdsrOptionRreqHeader::SetNodeAddress (uint8_t index, Ipv4Address addr)
{
  m_ipv4Address.at (index) = addr;
}

Ipv4Address RushattackdsrOptionRreqHeader::GetNodeAddress (uint8_t index) const
{
  return m_ipv4Address.at (index);
}

void RushattackdsrOptionRreqHeader::SetId (uint16_t identification)
{
  m_identification = identification;
}

uint16_t RushattackdsrOptionRreqHeader::GetId () const
{
  return m_identification;
}

void RushattackdsrOptionRreqHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength () << "";

  for (std::vector<Ipv4Address>::const_iterator it = m_ipv4Address.begin (); it != m_ipv4Address.end (); it++)
    {
      os << *it << " ";
    }

  os << ")";
}

uint32_t RushattackdsrOptionRreqHeader::GetSerializedSize () const
{
  return 8 + m_ipv4Address.size () * 4;
}

void RushattackdsrOptionRreqHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  uint8_t buff[4];

  i.WriteU8 (GetType ());
  i.WriteU8 (GetLength ());
  i.WriteHtonU16 (m_identification);
  WriteTo (i, m_target);

  for (VectorIpv4Address_t::const_iterator it = m_ipv4Address.begin (); it != m_ipv4Address.end (); it++)
    {
      it->Serialize (buff);
      i.Write (buff, 4);
    }
}

uint32_t RushattackdsrOptionRreqHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint8_t buff[4];

  SetType (i.ReadU8 ());
  SetLength (i.ReadU8 ());
  m_identification = i.ReadNtohU16 ();
  ReadFrom (i, m_target);

  uint8_t index = 0;
  for (std::vector<Ipv4Address>::iterator it = m_ipv4Address.begin (); it != m_ipv4Address.end (); it++)
    {
      i.Read (buff, 4);
      m_address = it->Deserialize (buff);
      SetNodeAddress (index, m_address);
      ++index;
    }

  return GetSerializedSize ();
}

RushattackdsrOptionHeader::Alignment RushattackdsrOptionRreqHeader::GetAlignment () const
{
  Alignment retVal = { 4, 0 };
  return retVal;
}

NS_OBJECT_ENSURE_REGISTERED (RushattackdsrOptionRrepHeader);

TypeId RushattackdsrOptionRrepHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rushattackdsr::RushattackdsrOptionRrepHeader")
    .AddConstructor<RushattackdsrOptionRrepHeader> ()
    .SetParent<RushattackdsrOptionHeader> ()
    .SetGroupName ("Rushattackdsr")
  ;
  return tid;
}

TypeId RushattackdsrOptionRrepHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

RushattackdsrOptionRrepHeader::RushattackdsrOptionRrepHeader ()
  : m_ipv4Address (0)
{
  SetType (2);
  SetLength (2 + m_ipv4Address.size () * 4);
}

RushattackdsrOptionRrepHeader::~RushattackdsrOptionRrepHeader ()
{
}

void RushattackdsrOptionRrepHeader::SetNumberAddress (uint8_t n)
{
  m_ipv4Address.clear ();
  m_ipv4Address.assign (n, Ipv4Address ());
}

void RushattackdsrOptionRrepHeader::SetNodesAddress (std::vector<Ipv4Address> ipv4Address)
{
  m_ipv4Address = ipv4Address;
  SetLength (2 + m_ipv4Address.size () * 4);
}

std::vector<Ipv4Address> RushattackdsrOptionRrepHeader::GetNodesAddress () const
{
  return m_ipv4Address;
}

void RushattackdsrOptionRrepHeader::SetNodeAddress (uint8_t index, Ipv4Address addr)
{
  m_ipv4Address.at (index) = addr;
}

Ipv4Address RushattackdsrOptionRrepHeader::GetNodeAddress (uint8_t index) const
{
  return m_ipv4Address.at (index);
}

Ipv4Address RushattackdsrOptionRrepHeader::GetTargetAddress (std::vector<Ipv4Address> ipv4Address) const
{
  return m_ipv4Address.at (ipv4Address.size () - 1);
}

void RushattackdsrOptionRrepHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength () << "";

  for (std::vector<Ipv4Address>::const_iterator it = m_ipv4Address.begin (); it != m_ipv4Address.end (); it++)
    {
      os << *it << " ";
    }

  os << ")";
}

uint32_t RushattackdsrOptionRrepHeader::GetSerializedSize () const
{
  return 4 + m_ipv4Address.size () * 4;
}

void RushattackdsrOptionRrepHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  uint8_t buff[4];

  i.WriteU8 (GetType ());
  i.WriteU8 (GetLength ());
  i.WriteU8 (0);
  i.WriteU8 (0);

  for (VectorIpv4Address_t::const_iterator it = m_ipv4Address.begin (); it != m_ipv4Address.end (); it++)
    {
      it->Serialize (buff);
      i.Write (buff, 4);
    }
}

uint32_t RushattackdsrOptionRrepHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint8_t buff[4];

  SetType (i.ReadU8 ());
  SetLength (i.ReadU8 ());
  i.ReadU8 ();
  i.ReadU8 ();

  uint8_t index = 0;
  for (std::vector<Ipv4Address>::iterator it = m_ipv4Address.begin (); it != m_ipv4Address.end (); it++)
    {
      i.Read (buff, 4);
      m_address = it->Deserialize (buff);
      SetNodeAddress (index, m_address);
      ++index;
    }

  return GetSerializedSize ();
}

RushattackdsrOptionHeader::Alignment RushattackdsrOptionRrepHeader::GetAlignment () const
{
  Alignment retVal = { 4, 0 };
  return retVal;
}

NS_OBJECT_ENSURE_REGISTERED (RushattackdsrOptionSRHeader);

TypeId RushattackdsrOptionSRHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rushattackdsr::RushattackdsrOptionSRHeader")
    .AddConstructor<RushattackdsrOptionSRHeader> ()
    .SetParent<RushattackdsrOptionHeader> ()
    .SetGroupName ("Rushattackdsr")
  ;
  return tid;
}

TypeId RushattackdsrOptionSRHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

RushattackdsrOptionSRHeader::RushattackdsrOptionSRHeader ()
  : m_segmentsLeft (0),
    m_ipv4Address (0)
{
  SetType (96);
  SetLength (2 + m_ipv4Address.size () * 4);
}

RushattackdsrOptionSRHeader::~RushattackdsrOptionSRHeader ()
{
}

void RushattackdsrOptionSRHeader::SetSegmentsLeft (uint8_t segmentsLeft)
{
  m_segmentsLeft = segmentsLeft;
}

uint8_t RushattackdsrOptionSRHeader::GetSegmentsLeft () const
{
  return m_segmentsLeft;
}

void RushattackdsrOptionSRHeader::SetSalvage (uint8_t salvage)
{
  m_salvage = salvage;
}

uint8_t RushattackdsrOptionSRHeader::GetSalvage () const
{
  return m_salvage;
}

void RushattackdsrOptionSRHeader::SetNumberAddress (uint8_t n)
{
  m_ipv4Address.clear ();
  m_ipv4Address.assign (n, Ipv4Address ());
}

void RushattackdsrOptionSRHeader::SetNodesAddress (std::vector<Ipv4Address> ipv4Address)
{
  m_ipv4Address = ipv4Address;
  SetLength (2 + m_ipv4Address.size () * 4);
}

std::vector<Ipv4Address> RushattackdsrOptionSRHeader::GetNodesAddress () const
{
  return m_ipv4Address;
}

void RushattackdsrOptionSRHeader::SetNodeAddress (uint8_t index, Ipv4Address addr)
{
  m_ipv4Address.at (index) = addr;
}

Ipv4Address RushattackdsrOptionSRHeader::GetNodeAddress (uint8_t index) const
{
  return m_ipv4Address.at (index);
}

uint8_t RushattackdsrOptionSRHeader::GetNodeListSize () const
{
  return m_ipv4Address.size ();
}

void RushattackdsrOptionSRHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength () << "";

  for (std::vector<Ipv4Address>::const_iterator it = m_ipv4Address.begin (); it != m_ipv4Address.end (); it++)
    {
      os << *it << " ";
    }

  os << ")";
}

uint32_t RushattackdsrOptionSRHeader::GetSerializedSize () const
{
  return 4 + m_ipv4Address.size () * 4;
}

void RushattackdsrOptionSRHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  uint8_t buff[4];

  i.WriteU8 (GetType ());
  i.WriteU8 (GetLength ());
  i.WriteU8 (m_salvage);
  i.WriteU8 (m_segmentsLeft);

  for (VectorIpv4Address_t::const_iterator it = m_ipv4Address.begin (); it != m_ipv4Address.end (); it++)
    {
      it->Serialize (buff);
      i.Write (buff, 4);
    }
}

uint32_t RushattackdsrOptionSRHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint8_t buff[4];

  SetType (i.ReadU8 ());
  SetLength (i.ReadU8 ());
  m_salvage = i.ReadU8 ();
  m_segmentsLeft = i.ReadU8 ();

  uint8_t index = 0;
  for (std::vector<Ipv4Address>::iterator it = m_ipv4Address.begin (); it != m_ipv4Address.end (); it++)
    {
      i.Read (buff, 4);
      m_address = it->Deserialize (buff);
      SetNodeAddress (index, m_address);
      ++index;
    }

  return GetSerializedSize ();
}

RushattackdsrOptionHeader::Alignment RushattackdsrOptionSRHeader::GetAlignment () const
{
  Alignment retVal = { 4, 0 };
  return retVal;
}

NS_OBJECT_ENSURE_REGISTERED (RushattackdsrOptionRerrHeader);

TypeId RushattackdsrOptionRerrHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rushattackdsr::RushattackdsrOptionRerrHeader")
    .AddConstructor<RushattackdsrOptionRerrHeader> ()
    .SetParent<RushattackdsrOptionHeader> ()
    .SetGroupName ("Rushattackdsr")
  ;
  return tid;
}

TypeId RushattackdsrOptionRerrHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

RushattackdsrOptionRerrHeader::RushattackdsrOptionRerrHeader ()
  : m_errorType (0),
    m_salvage (0),
    m_errorLength (4)
{
  SetType (3);
  SetLength (18);
}

RushattackdsrOptionRerrHeader::~RushattackdsrOptionRerrHeader ()
{
}

void RushattackdsrOptionRerrHeader::SetErrorType (uint8_t errorType)
{
  m_errorType = errorType;
}

uint8_t RushattackdsrOptionRerrHeader::GetErrorType () const
{
  return m_errorType;
}

void RushattackdsrOptionRerrHeader::SetSalvage (uint8_t salvage)
{
  m_salvage = salvage;
}

uint8_t RushattackdsrOptionRerrHeader::GetSalvage () const
{
  return m_salvage;
}

void RushattackdsrOptionRerrHeader::SetErrorSrc (Ipv4Address errorSrcAddress)
{
  m_errorSrcAddress = errorSrcAddress;
}

Ipv4Address RushattackdsrOptionRerrHeader::GetErrorSrc () const
{
  return m_errorSrcAddress;
}

void RushattackdsrOptionRerrHeader::SetErrorDst (Ipv4Address errorDstAddress)
{
  m_errorDstAddress = errorDstAddress;
}

Ipv4Address RushattackdsrOptionRerrHeader::GetErrorDst () const
{
  return m_errorDstAddress;
}

void RushattackdsrOptionRerrHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength ()
     << " errorType = " << (uint32_t)m_errorType << " salvage = " << (uint32_t)m_salvage
     << " error source = " << m_errorSrcAddress << " error dst = " << m_errorDstAddress << " )";

}

uint32_t RushattackdsrOptionRerrHeader::GetSerializedSize () const
{
  return 20;
}

void RushattackdsrOptionRerrHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetLength ());
  i.WriteU8 (m_errorType);
  i.WriteU8 (m_salvage);
  WriteTo (i, m_errorSrcAddress);
  WriteTo (i, m_errorDstAddress);
  i.Write (m_errorData.Begin (), m_errorData.End ());
}

uint32_t RushattackdsrOptionRerrHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetLength (i.ReadU8 ());
  m_errorType = i.ReadU8 ();
  m_salvage = i.ReadU8 ();
  ReadFrom (i, m_errorSrcAddress);
  ReadFrom (i, m_errorDstAddress);

  m_errorData = Buffer ();
  m_errorData.AddAtEnd (m_errorLength);
  Buffer::Iterator dataStart = i;
  i.Next (m_errorLength);
  Buffer::Iterator dataEnd = i;
  m_errorData.Begin ().Write (dataStart, dataEnd);

  return GetSerializedSize ();
}

RushattackdsrOptionHeader::Alignment RushattackdsrOptionRerrHeader::GetAlignment () const
{
  Alignment retVal = { 4, 0 };
  return retVal;
}

NS_OBJECT_ENSURE_REGISTERED (RushattackdsrOptionRerrUnreachHeader);

TypeId RushattackdsrOptionRerrUnreachHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rushattackdsr::RushattackdsrOptionRerrUnreachHeader")
    .AddConstructor<RushattackdsrOptionRerrUnreachHeader> ()
    .SetParent<RushattackdsrOptionRerrHeader> ()
    .SetGroupName ("Rushattackdsr")
  ;
  return tid;
}

TypeId RushattackdsrOptionRerrUnreachHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

RushattackdsrOptionRerrUnreachHeader::RushattackdsrOptionRerrUnreachHeader ()
  : m_salvage (0)
{
  SetType (3);
  SetLength (18);
  SetErrorType (1);
}

RushattackdsrOptionRerrUnreachHeader::~RushattackdsrOptionRerrUnreachHeader ()
{
}

void RushattackdsrOptionRerrUnreachHeader::SetSalvage (uint8_t salvage)
{
  m_salvage = salvage;
}

uint8_t RushattackdsrOptionRerrUnreachHeader::GetSalvage () const
{
  return m_salvage;
}

void RushattackdsrOptionRerrUnreachHeader::SetErrorSrc (Ipv4Address errorSrcAddress)
{
  m_errorSrcAddress = errorSrcAddress;
}

Ipv4Address RushattackdsrOptionRerrUnreachHeader::GetErrorSrc () const
{
  return m_errorSrcAddress;
}

void RushattackdsrOptionRerrUnreachHeader::SetErrorDst (Ipv4Address errorDstAddress)
{
  m_errorDstAddress = errorDstAddress;
}

Ipv4Address RushattackdsrOptionRerrUnreachHeader::GetErrorDst () const
{
  return m_errorDstAddress;
}

void RushattackdsrOptionRerrUnreachHeader::SetUnreachNode (Ipv4Address unreachNode)
{
  m_unreachNode = unreachNode;
}

Ipv4Address RushattackdsrOptionRerrUnreachHeader::GetUnreachNode () const
{
  return m_unreachNode;
}

void RushattackdsrOptionRerrUnreachHeader::SetOriginalDst (Ipv4Address originalDst)
{
  m_originalDst = originalDst;
}

Ipv4Address RushattackdsrOptionRerrUnreachHeader::GetOriginalDst () const
{
  return m_originalDst;
}

void RushattackdsrOptionRerrUnreachHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength ()
     << " errorType = " << (uint32_t)m_errorType << " salvage = " << (uint32_t)m_salvage
     << " error source = " << m_errorSrcAddress << " error dst = " << m_errorDstAddress
     << " unreach node = " <<  m_unreachNode << " )";
}

uint32_t RushattackdsrOptionRerrUnreachHeader::GetSerializedSize () const
{
  return 20;
}

void RushattackdsrOptionRerrUnreachHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetLength ());
  i.WriteU8 (GetErrorType ());
  i.WriteU8 (m_salvage);
  WriteTo (i, m_errorSrcAddress);
  WriteTo (i, m_errorDstAddress);
  WriteTo (i, m_unreachNode);
  WriteTo (i, m_originalDst);
}

uint32_t RushattackdsrOptionRerrUnreachHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetLength (i.ReadU8 ());
  SetErrorType (i.ReadU8 ());
  m_salvage = i.ReadU8 ();
  ReadFrom (i, m_errorSrcAddress);
  ReadFrom (i, m_errorDstAddress);
  ReadFrom (i, m_unreachNode);
  ReadFrom (i, m_originalDst);

  return GetSerializedSize ();
}

RushattackdsrOptionHeader::Alignment RushattackdsrOptionRerrUnreachHeader::GetAlignment () const
{
  Alignment retVal = { 4, 0 };
  return retVal;
}

NS_OBJECT_ENSURE_REGISTERED (RushattackdsrOptionRerrUnsupportHeader);

TypeId RushattackdsrOptionRerrUnsupportHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rushattackdsr::RushattackdsrOptionRerrUnsupportHeader")
    .AddConstructor<RushattackdsrOptionRerrUnsupportHeader> ()
    .SetParent<RushattackdsrOptionRerrHeader> ()
    .SetGroupName ("Rushattackdsr")
  ;
  return tid;
}

TypeId RushattackdsrOptionRerrUnsupportHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

RushattackdsrOptionRerrUnsupportHeader::RushattackdsrOptionRerrUnsupportHeader ()
  : m_salvage (0)
{
  SetType (3);
  SetLength (14);
  SetErrorType (3);
}

RushattackdsrOptionRerrUnsupportHeader::~RushattackdsrOptionRerrUnsupportHeader ()
{
}

void RushattackdsrOptionRerrUnsupportHeader::SetSalvage (uint8_t salvage)
{
  m_salvage = salvage;
}

uint8_t RushattackdsrOptionRerrUnsupportHeader::GetSalvage () const
{
  return m_salvage;
}

void RushattackdsrOptionRerrUnsupportHeader::SetErrorSrc (Ipv4Address errorSrcAddress)
{
  m_errorSrcAddress = errorSrcAddress;
}

Ipv4Address RushattackdsrOptionRerrUnsupportHeader::GetErrorSrc () const
{
  return m_errorSrcAddress;
}

void RushattackdsrOptionRerrUnsupportHeader::SetErrorDst (Ipv4Address errorDstAddress)
{
  m_errorDstAddress = errorDstAddress;
}

Ipv4Address RushattackdsrOptionRerrUnsupportHeader::GetErrorDst () const
{
  return m_errorDstAddress;
}

void RushattackdsrOptionRerrUnsupportHeader::SetUnsupported (uint16_t unsupport)
{
  m_unsupport = unsupport;
}

uint16_t RushattackdsrOptionRerrUnsupportHeader::GetUnsupported () const
{
  return m_unsupport;
}

void RushattackdsrOptionRerrUnsupportHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength ()
     << " errorType = " << (uint32_t)m_errorType << " salvage = " << (uint32_t)m_salvage
     << " error source = " << m_errorSrcAddress << " error dst = " << m_errorDstAddress
     << " unsupported option = " <<  m_unsupport << " )";

}

uint32_t RushattackdsrOptionRerrUnsupportHeader::GetSerializedSize () const
{
  return 16;
}

void RushattackdsrOptionRerrUnsupportHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetLength ());
  i.WriteU8 (GetErrorType ());
  i.WriteU8 (m_salvage);
  WriteTo (i, m_errorSrcAddress);
  WriteTo (i, m_errorDstAddress);
  i.WriteU16 (m_unsupport);

}

uint32_t RushattackdsrOptionRerrUnsupportHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetLength (i.ReadU8 ());
  SetErrorType (i.ReadU8 ());
  m_salvage = i.ReadU8 ();
  ReadFrom (i, m_errorSrcAddress);
  ReadFrom (i, m_errorDstAddress);
  m_unsupport = i.ReadU16 ();

  return GetSerializedSize ();
}

RushattackdsrOptionHeader::Alignment RushattackdsrOptionRerrUnsupportHeader::GetAlignment () const
{
  Alignment retVal = { 4, 0 };
  return retVal;
}

NS_OBJECT_ENSURE_REGISTERED (RushattackdsrOptionAckReqHeader);

TypeId RushattackdsrOptionAckReqHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rushattackdsr::RushattackdsrOptionAckReqHeader")
    .AddConstructor<RushattackdsrOptionAckReqHeader> ()
    .SetParent<RushattackdsrOptionHeader> ()
    .SetGroupName ("Rushattackdsr")
  ;
  return tid;
}

TypeId RushattackdsrOptionAckReqHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

RushattackdsrOptionAckReqHeader::RushattackdsrOptionAckReqHeader ()
  : m_identification (0)

{
  SetType (160);
  SetLength (2);
}

RushattackdsrOptionAckReqHeader::~RushattackdsrOptionAckReqHeader ()
{
}

void RushattackdsrOptionAckReqHeader::SetAckId (uint16_t identification)
{
  m_identification = identification;
}

uint16_t RushattackdsrOptionAckReqHeader::GetAckId () const
{
  return m_identification;
}

void RushattackdsrOptionAckReqHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength ()
     << " id = " << m_identification << " )";
}

uint32_t RushattackdsrOptionAckReqHeader::GetSerializedSize () const
{
  return 4;
}

void RushattackdsrOptionAckReqHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetLength ());
  i.WriteU16 (m_identification);
}

uint32_t RushattackdsrOptionAckReqHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetLength (i.ReadU8 ());
  m_identification = i.ReadU16 ();

  return GetSerializedSize ();
}

RushattackdsrOptionHeader::Alignment RushattackdsrOptionAckReqHeader::GetAlignment () const
{
  Alignment retVal = { 4, 0 };
  return retVal;
}

NS_OBJECT_ENSURE_REGISTERED (RushattackdsrOptionAckHeader);

TypeId RushattackdsrOptionAckHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::rushattackdsr::RushattackdsrOptionAckHeader")
    .AddConstructor<RushattackdsrOptionAckHeader> ()
    .SetParent<RushattackdsrOptionHeader> ()
    .SetGroupName ("Rushattackdsr")
  ;
  return tid;
}

TypeId RushattackdsrOptionAckHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

RushattackdsrOptionAckHeader::RushattackdsrOptionAckHeader ()
  :    m_identification (0)
{
  SetType (32);
  SetLength (10);
}

RushattackdsrOptionAckHeader::~RushattackdsrOptionAckHeader ()
{
}

void RushattackdsrOptionAckHeader::SetAckId (uint16_t identification)
{
  m_identification = identification;
}

uint16_t RushattackdsrOptionAckHeader::GetAckId () const
{
  return m_identification;
}

void RushattackdsrOptionAckHeader::SetRealSrc (Ipv4Address realSrcAddress)
{
  m_realSrcAddress = realSrcAddress;
}

Ipv4Address RushattackdsrOptionAckHeader::GetRealSrc () const
{
  return m_realSrcAddress;
}

void RushattackdsrOptionAckHeader::SetRealDst (Ipv4Address realDstAddress)
{
  m_realDstAddress = realDstAddress;
}

Ipv4Address RushattackdsrOptionAckHeader::GetRealDst () const
{
  return m_realDstAddress;
}

void RushattackdsrOptionAckHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength ()
     << " id = " << m_identification << " real src = " << m_realSrcAddress
     << " real dst = " << m_realDstAddress << " )";

}

uint32_t RushattackdsrOptionAckHeader::GetSerializedSize () const
{
  return 12;
}

void RushattackdsrOptionAckHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetLength ());
  i.WriteU16 (m_identification);
  WriteTo (i, m_realSrcAddress);
  WriteTo (i, m_realDstAddress);
}

uint32_t RushattackdsrOptionAckHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetLength (i.ReadU8 ());
  m_identification = i.ReadU16 ();
  ReadFrom (i, m_realSrcAddress);
  ReadFrom (i, m_realDstAddress);

  return GetSerializedSize ();
}

RushattackdsrOptionHeader::Alignment RushattackdsrOptionAckHeader::GetAlignment () const
{
  Alignment retVal = { 4, 0 };
  return retVal;
}
} /* namespace rushattackdsr */
} /* namespace ns3 */
