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
#include "sdattackdsr-option-header.h"
#include "ns3/ipv4-address.h"
#include "ns3/address-utils.h"
#include "ns3/packet.h"
#include "ns3/enum.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("SdattackdsrOptionHeader");

namespace sdattackdsr {

NS_OBJECT_ENSURE_REGISTERED (SdattackdsrOptionHeader);

TypeId SdattackdsrOptionHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::sdattackdsr::SdattackdsrOptionHeader")
    .AddConstructor<SdattackdsrOptionHeader> ()
    .SetParent<Header> ()
    .SetGroupName ("Sdattackdsr")
  ;
  return tid;
}

TypeId SdattackdsrOptionHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

SdattackdsrOptionHeader::SdattackdsrOptionHeader ()
  : m_type (0),
    m_length (0)
{
}

SdattackdsrOptionHeader::~SdattackdsrOptionHeader ()
{
}

void SdattackdsrOptionHeader::SetType (uint8_t type)
{
  m_type = type;
}

uint8_t SdattackdsrOptionHeader::GetType () const
{
  return m_type;
}

void SdattackdsrOptionHeader::SetLength (uint8_t length)
{
  m_length = length;
}

uint8_t SdattackdsrOptionHeader::GetLength () const
{
  return m_length;
}

void SdattackdsrOptionHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)m_type << " length = " << (uint32_t)m_length << " )";
}

uint32_t SdattackdsrOptionHeader::GetSerializedSize () const
{
  return m_length + 2;
}

void SdattackdsrOptionHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (m_type);
  i.WriteU8 (m_length);
  i.Write (m_data.Begin (), m_data.End ());
}

uint32_t SdattackdsrOptionHeader::Deserialize (Buffer::Iterator start)
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

SdattackdsrOptionHeader::Alignment SdattackdsrOptionHeader::GetAlignment () const
{
  Alignment retVal = { 1, 0 };
  return retVal;
}

NS_OBJECT_ENSURE_REGISTERED (SdattackdsrOptionPad1Header);

TypeId SdattackdsrOptionPad1Header::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::sdattackdsr::SdattackdsrOptionPad1Header")
    .AddConstructor<SdattackdsrOptionPad1Header> ()
    .SetParent<SdattackdsrOptionHeader> ()
    .SetGroupName ("Sdattackdsr")
  ;
  return tid;
}

TypeId SdattackdsrOptionPad1Header::GetInstanceTypeId () const
{
  return GetTypeId ();
}

SdattackdsrOptionPad1Header::SdattackdsrOptionPad1Header ()
{
  SetType (224);
}

SdattackdsrOptionPad1Header::~SdattackdsrOptionPad1Header ()
{
}

void SdattackdsrOptionPad1Header::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " )";
}

uint32_t SdattackdsrOptionPad1Header::GetSerializedSize () const
{
  return 1;
}

void SdattackdsrOptionPad1Header::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
}

uint32_t SdattackdsrOptionPad1Header::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());

  return GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (SdattackdsrOptionPadnHeader);

TypeId SdattackdsrOptionPadnHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::sdattackdsr::SdattackdsrOptionPadnHeader")
    .AddConstructor<SdattackdsrOptionPadnHeader> ()
    .SetParent<SdattackdsrOptionHeader> ()
    .SetGroupName ("Sdattackdsr")
  ;
  return tid;
}

TypeId SdattackdsrOptionPadnHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

SdattackdsrOptionPadnHeader::SdattackdsrOptionPadnHeader (uint32_t pad)
{
  SetType (0);
  NS_ASSERT_MSG (pad >= 2, "PadN must be at least 2 bytes long");
  SetLength (pad - 2);
}

SdattackdsrOptionPadnHeader::~SdattackdsrOptionPadnHeader ()
{
}

void SdattackdsrOptionPadnHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength () << " )";
}

uint32_t SdattackdsrOptionPadnHeader::GetSerializedSize () const
{
  return GetLength () + 2;
}

void SdattackdsrOptionPadnHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetLength ());

  for (int padding = 0; padding < GetLength (); padding++)
    {
      i.WriteU8 (0);
    }
}

uint32_t SdattackdsrOptionPadnHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetLength (i.ReadU8 ());

  return GetSerializedSize ();
}

NS_OBJECT_ENSURE_REGISTERED (SdattackdsrOptionRreqHeader);

TypeId SdattackdsrOptionRreqHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::sdattackdsr::SdattackdsrOptionRreqHeader")
    .AddConstructor<SdattackdsrOptionRreqHeader> ()
    .SetParent<SdattackdsrOptionHeader> ()
    .SetGroupName ("Sdattackdsr")
  ;
  return tid;
}

TypeId SdattackdsrOptionRreqHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

SdattackdsrOptionRreqHeader::SdattackdsrOptionRreqHeader ()
  : m_ipv4Address (0)
{
  SetType (1);
  SetLength (6 + m_ipv4Address.size () * 4);
}

SdattackdsrOptionRreqHeader::~SdattackdsrOptionRreqHeader ()
{
}

void SdattackdsrOptionRreqHeader::SetNumberAddress (uint8_t n)
{
  m_ipv4Address.clear ();
  m_ipv4Address.assign (n, Ipv4Address ());
}

Ipv4Address SdattackdsrOptionRreqHeader::GetTarget ()
{
  return m_target;
}

void SdattackdsrOptionRreqHeader::SetTarget (Ipv4Address target)
{
  m_target = target;
}

void SdattackdsrOptionRreqHeader::AddNodeAddress (Ipv4Address ipv4)
{
  m_ipv4Address.push_back (ipv4);
  SetLength (6 + m_ipv4Address.size () * 4);
}

void SdattackdsrOptionRreqHeader::SetNodesAddress (std::vector<Ipv4Address> ipv4Address)
{
  m_ipv4Address = ipv4Address;
  SetLength (6 + m_ipv4Address.size () * 4);
}

std::vector<Ipv4Address> SdattackdsrOptionRreqHeader::GetNodesAddresses () const
{
  return m_ipv4Address;
}

uint32_t SdattackdsrOptionRreqHeader::GetNodesNumber () const
{
  return m_ipv4Address.size ();
}

void SdattackdsrOptionRreqHeader::SetNodeAddress (uint8_t index, Ipv4Address addr)
{
  m_ipv4Address.at (index) = addr;
}

Ipv4Address SdattackdsrOptionRreqHeader::GetNodeAddress (uint8_t index) const
{
  return m_ipv4Address.at (index);
}

void SdattackdsrOptionRreqHeader::SetId (uint16_t identification)
{
  m_identification = identification;
}

uint16_t SdattackdsrOptionRreqHeader::GetId () const
{
  return m_identification;
}

void SdattackdsrOptionRreqHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength () << "";

  for (std::vector<Ipv4Address>::const_iterator it = m_ipv4Address.begin (); it != m_ipv4Address.end (); it++)
    {
      os << *it << " ";
    }

  os << ")";
}

uint32_t SdattackdsrOptionRreqHeader::GetSerializedSize () const
{
  return 8 + m_ipv4Address.size () * 4;
}

void SdattackdsrOptionRreqHeader::Serialize (Buffer::Iterator start) const
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

uint32_t SdattackdsrOptionRreqHeader::Deserialize (Buffer::Iterator start)
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

SdattackdsrOptionHeader::Alignment SdattackdsrOptionRreqHeader::GetAlignment () const
{
  Alignment retVal = { 4, 0 };
  return retVal;
}

NS_OBJECT_ENSURE_REGISTERED (SdattackdsrOptionRrepHeader);

TypeId SdattackdsrOptionRrepHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::sdattackdsr::SdattackdsrOptionRrepHeader")
    .AddConstructor<SdattackdsrOptionRrepHeader> ()
    .SetParent<SdattackdsrOptionHeader> ()
    .SetGroupName ("Sdattackdsr")
  ;
  return tid;
}

TypeId SdattackdsrOptionRrepHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

SdattackdsrOptionRrepHeader::SdattackdsrOptionRrepHeader ()
  : m_ipv4Address (0)
{
  SetType (2);
  SetLength (2 + m_ipv4Address.size () * 4);
}

SdattackdsrOptionRrepHeader::~SdattackdsrOptionRrepHeader ()
{
}

void SdattackdsrOptionRrepHeader::SetNumberAddress (uint8_t n)
{
  m_ipv4Address.clear ();
  m_ipv4Address.assign (n, Ipv4Address ());
}

void SdattackdsrOptionRrepHeader::SetNodesAddress (std::vector<Ipv4Address> ipv4Address)
{
  m_ipv4Address = ipv4Address;
  SetLength (2 + m_ipv4Address.size () * 4);
}

std::vector<Ipv4Address> SdattackdsrOptionRrepHeader::GetNodesAddress () const
{
  return m_ipv4Address;
}

void SdattackdsrOptionRrepHeader::SetNodeAddress (uint8_t index, Ipv4Address addr)
{
  m_ipv4Address.at (index) = addr;
}

Ipv4Address SdattackdsrOptionRrepHeader::GetNodeAddress (uint8_t index) const
{
  return m_ipv4Address.at (index);
}

Ipv4Address SdattackdsrOptionRrepHeader::GetTargetAddress (std::vector<Ipv4Address> ipv4Address) const
{
  return m_ipv4Address.at (ipv4Address.size () - 1);
}

void SdattackdsrOptionRrepHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength () << "";

  for (std::vector<Ipv4Address>::const_iterator it = m_ipv4Address.begin (); it != m_ipv4Address.end (); it++)
    {
      os << *it << " ";
    }

  os << ")";
}

uint32_t SdattackdsrOptionRrepHeader::GetSerializedSize () const
{
  return 4 + m_ipv4Address.size () * 4;
}

void SdattackdsrOptionRrepHeader::Serialize (Buffer::Iterator start) const
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

uint32_t SdattackdsrOptionRrepHeader::Deserialize (Buffer::Iterator start)
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

SdattackdsrOptionHeader::Alignment SdattackdsrOptionRrepHeader::GetAlignment () const
{
  Alignment retVal = { 4, 0 };
  return retVal;
}

NS_OBJECT_ENSURE_REGISTERED (SdattackdsrOptionSRHeader);

TypeId SdattackdsrOptionSRHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::sdattackdsr::SdattackdsrOptionSRHeader")
    .AddConstructor<SdattackdsrOptionSRHeader> ()
    .SetParent<SdattackdsrOptionHeader> ()
    .SetGroupName ("Sdattackdsr")
  ;
  return tid;
}

TypeId SdattackdsrOptionSRHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

SdattackdsrOptionSRHeader::SdattackdsrOptionSRHeader ()
  : m_segmentsLeft (0),
    m_ipv4Address (0)
{
  SetType (96);
  SetLength (2 + m_ipv4Address.size () * 4);
}

SdattackdsrOptionSRHeader::~SdattackdsrOptionSRHeader ()
{
}

void SdattackdsrOptionSRHeader::SetSegmentsLeft (uint8_t segmentsLeft)
{
  m_segmentsLeft = segmentsLeft;
}

uint8_t SdattackdsrOptionSRHeader::GetSegmentsLeft () const
{
  return m_segmentsLeft;
}

void SdattackdsrOptionSRHeader::SetSalvage (uint8_t salvage)
{
  m_salvage = salvage;
}

uint8_t SdattackdsrOptionSRHeader::GetSalvage () const
{
  return m_salvage;
}

void SdattackdsrOptionSRHeader::SetNumberAddress (uint8_t n)
{
  m_ipv4Address.clear ();
  m_ipv4Address.assign (n, Ipv4Address ());
}

void SdattackdsrOptionSRHeader::SetNodesAddress (std::vector<Ipv4Address> ipv4Address)
{
  m_ipv4Address = ipv4Address;
  SetLength (2 + m_ipv4Address.size () * 4);
}

std::vector<Ipv4Address> SdattackdsrOptionSRHeader::GetNodesAddress () const
{
  return m_ipv4Address;
}

void SdattackdsrOptionSRHeader::SetNodeAddress (uint8_t index, Ipv4Address addr)
{
  m_ipv4Address.at (index) = addr;
}

Ipv4Address SdattackdsrOptionSRHeader::GetNodeAddress (uint8_t index) const
{
  return m_ipv4Address.at (index);
}

uint8_t SdattackdsrOptionSRHeader::GetNodeListSize () const
{
  return m_ipv4Address.size ();
}

void SdattackdsrOptionSRHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength () << "";

  for (std::vector<Ipv4Address>::const_iterator it = m_ipv4Address.begin (); it != m_ipv4Address.end (); it++)
    {
      os << *it << " ";
    }

  os << ")";
}

uint32_t SdattackdsrOptionSRHeader::GetSerializedSize () const
{
  return 4 + m_ipv4Address.size () * 4;
}

void SdattackdsrOptionSRHeader::Serialize (Buffer::Iterator start) const
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

uint32_t SdattackdsrOptionSRHeader::Deserialize (Buffer::Iterator start)
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

SdattackdsrOptionHeader::Alignment SdattackdsrOptionSRHeader::GetAlignment () const
{
  Alignment retVal = { 4, 0 };
  return retVal;
}

NS_OBJECT_ENSURE_REGISTERED (SdattackdsrOptionRerrHeader);

TypeId SdattackdsrOptionRerrHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::sdattackdsr::SdattackdsrOptionRerrHeader")
    .AddConstructor<SdattackdsrOptionRerrHeader> ()
    .SetParent<SdattackdsrOptionHeader> ()
    .SetGroupName ("Sdattackdsr")
  ;
  return tid;
}

TypeId SdattackdsrOptionRerrHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

SdattackdsrOptionRerrHeader::SdattackdsrOptionRerrHeader ()
  : m_errorType (0),
    m_salvage (0),
    m_errorLength (4)
{
  SetType (3);
  SetLength (18);
}

SdattackdsrOptionRerrHeader::~SdattackdsrOptionRerrHeader ()
{
}

void SdattackdsrOptionRerrHeader::SetErrorType (uint8_t errorType)
{
  m_errorType = errorType;
}

uint8_t SdattackdsrOptionRerrHeader::GetErrorType () const
{
  return m_errorType;
}

void SdattackdsrOptionRerrHeader::SetSalvage (uint8_t salvage)
{
  m_salvage = salvage;
}

uint8_t SdattackdsrOptionRerrHeader::GetSalvage () const
{
  return m_salvage;
}

void SdattackdsrOptionRerrHeader::SetErrorSrc (Ipv4Address errorSrcAddress)
{
  m_errorSrcAddress = errorSrcAddress;
}

Ipv4Address SdattackdsrOptionRerrHeader::GetErrorSrc () const
{
  return m_errorSrcAddress;
}

void SdattackdsrOptionRerrHeader::SetErrorDst (Ipv4Address errorDstAddress)
{
  m_errorDstAddress = errorDstAddress;
}

Ipv4Address SdattackdsrOptionRerrHeader::GetErrorDst () const
{
  return m_errorDstAddress;
}

void SdattackdsrOptionRerrHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength ()
     << " errorType = " << (uint32_t)m_errorType << " salvage = " << (uint32_t)m_salvage
     << " error source = " << m_errorSrcAddress << " error dst = " << m_errorDstAddress << " )";

}

uint32_t SdattackdsrOptionRerrHeader::GetSerializedSize () const
{
  return 20;
}

void SdattackdsrOptionRerrHeader::Serialize (Buffer::Iterator start) const
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

uint32_t SdattackdsrOptionRerrHeader::Deserialize (Buffer::Iterator start)
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

SdattackdsrOptionHeader::Alignment SdattackdsrOptionRerrHeader::GetAlignment () const
{
  Alignment retVal = { 4, 0 };
  return retVal;
}

NS_OBJECT_ENSURE_REGISTERED (SdattackdsrOptionRerrUnreachHeader);

TypeId SdattackdsrOptionRerrUnreachHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::sdattackdsr::SdattackdsrOptionRerrUnreachHeader")
    .AddConstructor<SdattackdsrOptionRerrUnreachHeader> ()
    .SetParent<SdattackdsrOptionRerrHeader> ()
    .SetGroupName ("Sdattackdsr")
  ;
  return tid;
}

TypeId SdattackdsrOptionRerrUnreachHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

SdattackdsrOptionRerrUnreachHeader::SdattackdsrOptionRerrUnreachHeader ()
  : m_salvage (0)
{
  SetType (3);
  SetLength (18);
  SetErrorType (1);
}

SdattackdsrOptionRerrUnreachHeader::~SdattackdsrOptionRerrUnreachHeader ()
{
}

void SdattackdsrOptionRerrUnreachHeader::SetSalvage (uint8_t salvage)
{
  m_salvage = salvage;
}

uint8_t SdattackdsrOptionRerrUnreachHeader::GetSalvage () const
{
  return m_salvage;
}

void SdattackdsrOptionRerrUnreachHeader::SetErrorSrc (Ipv4Address errorSrcAddress)
{
  m_errorSrcAddress = errorSrcAddress;
}

Ipv4Address SdattackdsrOptionRerrUnreachHeader::GetErrorSrc () const
{
  return m_errorSrcAddress;
}

void SdattackdsrOptionRerrUnreachHeader::SetErrorDst (Ipv4Address errorDstAddress)
{
  m_errorDstAddress = errorDstAddress;
}

Ipv4Address SdattackdsrOptionRerrUnreachHeader::GetErrorDst () const
{
  return m_errorDstAddress;
}

void SdattackdsrOptionRerrUnreachHeader::SetUnreachNode (Ipv4Address unreachNode)
{
  m_unreachNode = unreachNode;
}

Ipv4Address SdattackdsrOptionRerrUnreachHeader::GetUnreachNode () const
{
  return m_unreachNode;
}

void SdattackdsrOptionRerrUnreachHeader::SetOriginalDst (Ipv4Address originalDst)
{
  m_originalDst = originalDst;
}

Ipv4Address SdattackdsrOptionRerrUnreachHeader::GetOriginalDst () const
{
  return m_originalDst;
}

void SdattackdsrOptionRerrUnreachHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength ()
     << " errorType = " << (uint32_t)m_errorType << " salvage = " << (uint32_t)m_salvage
     << " error source = " << m_errorSrcAddress << " error dst = " << m_errorDstAddress
     << " unreach node = " <<  m_unreachNode << " )";
}

uint32_t SdattackdsrOptionRerrUnreachHeader::GetSerializedSize () const
{
  return 20;
}

void SdattackdsrOptionRerrUnreachHeader::Serialize (Buffer::Iterator start) const
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

uint32_t SdattackdsrOptionRerrUnreachHeader::Deserialize (Buffer::Iterator start)
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

SdattackdsrOptionHeader::Alignment SdattackdsrOptionRerrUnreachHeader::GetAlignment () const
{
  Alignment retVal = { 4, 0 };
  return retVal;
}

NS_OBJECT_ENSURE_REGISTERED (SdattackdsrOptionRerrUnsupportHeader);

TypeId SdattackdsrOptionRerrUnsupportHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::sdattackdsr::SdattackdsrOptionRerrUnsupportHeader")
    .AddConstructor<SdattackdsrOptionRerrUnsupportHeader> ()
    .SetParent<SdattackdsrOptionRerrHeader> ()
    .SetGroupName ("Sdattackdsr")
  ;
  return tid;
}

TypeId SdattackdsrOptionRerrUnsupportHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

SdattackdsrOptionRerrUnsupportHeader::SdattackdsrOptionRerrUnsupportHeader ()
  : m_salvage (0)
{
  SetType (3);
  SetLength (14);
  SetErrorType (3);
}

SdattackdsrOptionRerrUnsupportHeader::~SdattackdsrOptionRerrUnsupportHeader ()
{
}

void SdattackdsrOptionRerrUnsupportHeader::SetSalvage (uint8_t salvage)
{
  m_salvage = salvage;
}

uint8_t SdattackdsrOptionRerrUnsupportHeader::GetSalvage () const
{
  return m_salvage;
}

void SdattackdsrOptionRerrUnsupportHeader::SetErrorSrc (Ipv4Address errorSrcAddress)
{
  m_errorSrcAddress = errorSrcAddress;
}

Ipv4Address SdattackdsrOptionRerrUnsupportHeader::GetErrorSrc () const
{
  return m_errorSrcAddress;
}

void SdattackdsrOptionRerrUnsupportHeader::SetErrorDst (Ipv4Address errorDstAddress)
{
  m_errorDstAddress = errorDstAddress;
}

Ipv4Address SdattackdsrOptionRerrUnsupportHeader::GetErrorDst () const
{
  return m_errorDstAddress;
}

void SdattackdsrOptionRerrUnsupportHeader::SetUnsupported (uint16_t unsupport)
{
  m_unsupport = unsupport;
}

uint16_t SdattackdsrOptionRerrUnsupportHeader::GetUnsupported () const
{
  return m_unsupport;
}

void SdattackdsrOptionRerrUnsupportHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength ()
     << " errorType = " << (uint32_t)m_errorType << " salvage = " << (uint32_t)m_salvage
     << " error source = " << m_errorSrcAddress << " error dst = " << m_errorDstAddress
     << " unsupported option = " <<  m_unsupport << " )";

}

uint32_t SdattackdsrOptionRerrUnsupportHeader::GetSerializedSize () const
{
  return 16;
}

void SdattackdsrOptionRerrUnsupportHeader::Serialize (Buffer::Iterator start) const
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

uint32_t SdattackdsrOptionRerrUnsupportHeader::Deserialize (Buffer::Iterator start)
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

SdattackdsrOptionHeader::Alignment SdattackdsrOptionRerrUnsupportHeader::GetAlignment () const
{
  Alignment retVal = { 4, 0 };
  return retVal;
}

NS_OBJECT_ENSURE_REGISTERED (SdattackdsrOptionAckReqHeader);

TypeId SdattackdsrOptionAckReqHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::sdattackdsr::SdattackdsrOptionAckReqHeader")
    .AddConstructor<SdattackdsrOptionAckReqHeader> ()
    .SetParent<SdattackdsrOptionHeader> ()
    .SetGroupName ("Sdattackdsr")
  ;
  return tid;
}

TypeId SdattackdsrOptionAckReqHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

SdattackdsrOptionAckReqHeader::SdattackdsrOptionAckReqHeader ()
  : m_identification (0)

{
  SetType (160);
  SetLength (2);
}

SdattackdsrOptionAckReqHeader::~SdattackdsrOptionAckReqHeader ()
{
}

void SdattackdsrOptionAckReqHeader::SetAckId (uint16_t identification)
{
  m_identification = identification;
}

uint16_t SdattackdsrOptionAckReqHeader::GetAckId () const
{
  return m_identification;
}

void SdattackdsrOptionAckReqHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength ()
     << " id = " << m_identification << " )";
}

uint32_t SdattackdsrOptionAckReqHeader::GetSerializedSize () const
{
  return 4;
}

void SdattackdsrOptionAckReqHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetLength ());
  i.WriteU16 (m_identification);
}

uint32_t SdattackdsrOptionAckReqHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetLength (i.ReadU8 ());
  m_identification = i.ReadU16 ();

  return GetSerializedSize ();
}

SdattackdsrOptionHeader::Alignment SdattackdsrOptionAckReqHeader::GetAlignment () const
{
  Alignment retVal = { 4, 0 };
  return retVal;
}

NS_OBJECT_ENSURE_REGISTERED (SdattackdsrOptionAckHeader);

TypeId SdattackdsrOptionAckHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::sdattackdsr::SdattackdsrOptionAckHeader")
    .AddConstructor<SdattackdsrOptionAckHeader> ()
    .SetParent<SdattackdsrOptionHeader> ()
    .SetGroupName ("Sdattackdsr")
  ;
  return tid;
}

TypeId SdattackdsrOptionAckHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

SdattackdsrOptionAckHeader::SdattackdsrOptionAckHeader ()
  :    m_identification (0)
{
  SetType (32);
  SetLength (10);
}

SdattackdsrOptionAckHeader::~SdattackdsrOptionAckHeader ()
{
}

void SdattackdsrOptionAckHeader::SetAckId (uint16_t identification)
{
  m_identification = identification;
}

uint16_t SdattackdsrOptionAckHeader::GetAckId () const
{
  return m_identification;
}

void SdattackdsrOptionAckHeader::SetRealSrc (Ipv4Address realSrcAddress)
{
  m_realSrcAddress = realSrcAddress;
}

Ipv4Address SdattackdsrOptionAckHeader::GetRealSrc () const
{
  return m_realSrcAddress;
}

void SdattackdsrOptionAckHeader::SetRealDst (Ipv4Address realDstAddress)
{
  m_realDstAddress = realDstAddress;
}

Ipv4Address SdattackdsrOptionAckHeader::GetRealDst () const
{
  return m_realDstAddress;
}

void SdattackdsrOptionAckHeader::Print (std::ostream &os) const
{
  os << "( type = " << (uint32_t)GetType () << " length = " << (uint32_t)GetLength ()
     << " id = " << m_identification << " real src = " << m_realSrcAddress
     << " real dst = " << m_realDstAddress << " )";

}

uint32_t SdattackdsrOptionAckHeader::GetSerializedSize () const
{
  return 12;
}

void SdattackdsrOptionAckHeader::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;

  i.WriteU8 (GetType ());
  i.WriteU8 (GetLength ());
  i.WriteU16 (m_identification);
  WriteTo (i, m_realSrcAddress);
  WriteTo (i, m_realDstAddress);
}

uint32_t SdattackdsrOptionAckHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  SetType (i.ReadU8 ());
  SetLength (i.ReadU8 ());
  m_identification = i.ReadU16 ();
  ReadFrom (i, m_realSrcAddress);
  ReadFrom (i, m_realDstAddress);

  return GetSerializedSize ();
}

SdattackdsrOptionHeader::Alignment SdattackdsrOptionAckHeader::GetAlignment () const
{
  Alignment retVal = { 4, 0 };
  return retVal;
}
} /* namespace sdattackdsr */
} /* namespace ns3 */
