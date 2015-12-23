//
//  sio_packet.cpp
//  sakura
//
//  Created by Frank Lin on 12/22/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#include "sio_packet.h"

#include <sstream>

#include "json11/json11.hpp"

#include "log.h"

USING_NS_SKR;
using std::unique_ptr;
using std::string;

static std::string const kSeparator = "";
static std::string const kEndSeparator = ",";

////////////////////////////////////////////////////////////////////////////////
// SIOPacket, public:

// Creation and lifetime --------------------------------------------------------

SIOPacket::SIOPacket() {
  
}

SIOPacket::~SIOPacket() {
  
}

void SIOPacket::InitWithType(SIOPacket::Type type) {
  type_ = type;
}

std::unique_ptr<SIOPacket> SIOPacket::SIOPacketWithType(SIOPacket::Type type) {
  unique_ptr<SIOPacket> rtn(new SIOPacket());
  rtn->InitWithType(type);
  return rtn;
}

// Utils --------------------------------------------------------

std::string SIOPacket::ToString() const {
  std::stringstream encoded;
  encoded << type_;
  encoded << kSeparator;
  
  std::string pid_l = packet_id_;
  if (ack_ == "data")
  {
    pid_l += "+";
  }
  
  // Do not write pid for acknowledgements
  if (type_ != SIOPacket::Type::ACK) {
    encoded << pid_l;
  }
  encoded << kSeparator;
  
  // Add the endpoint for the namespace to be used if not the default namespace "" or "/", and as long as it is not an ACK, heartbeat, or disconnect packet
  if (endpoint_ != "/"
      && endpoint_ != ""
      && type_ != SIOPacket::Type::ACK
      && type_ != SIOPacket::Type::DISCONNECT) {
    encoded << endpoint_ << kEndSeparator;
  }
  encoded << kSeparator;
  
  if (args_.size() != 0) {
    std::string ack_pid = "";
    // This is an acknowledgement packet, so, prepend the ack pid to the data
    if (type_ == SIOPacket::Type::ACK) {
      ack_pid += pid_l + "+";
    }
    
    encoded << ack_pid << Stringify();
  }
  
  return encoded.str();
}

std::string SIOPacket::Stringify() const {
  json11::Json::array args_json_array;
  for (auto it : args_) {
    args_json_array.push_back(it);
  }
  
  json11::Json json = json11::Json::object {
    { event_name_,  args_json_array },
  };
  
  string encode_str = json.dump();
  
  log_event("SIOPacket::Stringify args is %s", encode_str.c_str());
  
  return encode_str;
}
