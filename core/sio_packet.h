//
//  sio_packet.hpp
//  sakura
//
//  Created by Frank Lin on 12/22/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#ifndef SAKURA_SIO_PACKET_H_H
#define SAKURA_SIO_PACKET_H_H

#include <string>
#include <vector>

#include "macros.h"

NS_SKR_BEGIN

/// Socket.IO packet version is 1.x
///
/// @since 1.0.1
/// @author Frank Lin
class SIOPacket {
public:
  
  enum Type {
    CONNECT      = 0,
    DISCONNECT   = 1,
    EVENT        = 2,
    ACK          = 3,
    ERROR        = 4,
    BINARY_EVENT = 5,
    BINARY_ACK   = 6
  };
  
  // Creation and lifetime --------------------------------------------------------
  
  SIOPacket();
  virtual ~SIOPacket();
  
  void InitWithType(SIOPacket::Type type);
  
  static std::unique_ptr<SIOPacket> SIOPacketWithType(SIOPacket::Type type);
  
  // Utils --------------------------------------------------------
  
  std::string ToString() const;
  
  // encode name and args to JSON
  std::string Stringify() const;
  
  // Getters/Setters --------------------------------------------------------
  
  SIOPacket::Type type() const { return type_; }
  
  std::string endpoint() const { return endpoint_; }
  void set_endpoint(const std::string& endpoint) { endpoint_ = endpoint; }
  
  std::string event_name() const { return event_name_; }
  void set_event_name(const std::string& event_name) { event_name_ = event_name; }
  
  void add_data(const std::string& data) { args_.push_back(data); }
  std::vector<std::string> args() const { return args_; }
  
private:
  
  std::string packet_id_;
  std::string ack_;
  std::string event_name_;
  std::string endpoint_;
  std::string endpoint_separator_;
  
  std::vector<std::string> args_;
  
  SIOPacket::Type type_;
  
  
  DISALLOW_COPY_AND_ASSIGN(SIOPacket);
};

NS_SKR_END

#endif /* SAKURA_SIO_PACKET_H_H */
