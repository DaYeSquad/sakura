//
//  sio_client.hpp
//  sakura
//
//  Created by Frank Lin on 12/22/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#ifndef SAKURA_SIO_CLIENT_H_
#define SAKURA_SIO_CLIENT_H_

#include <string>
#include <map>

#include "macros.h"
#include "socket_io.h"

NS_SKR_BEGIN

class SIOClientImpl;

/// A single connection to a socket.io endpoint.
/// based on the SocketIO library created by LearnBoost at http://socket.io
/// using spec version 1 found at https://github.com/LearnBoost/socket.io-spec
///
/// @since 1.0.1
/// @author Frank Lin
class SIOClient {
public:
  
  friend class SIOClientImpl;
  
  // Creation and lifetime --------------------------------------------------------
  
  SIOClient(const std::string& host, int port, const std::string& path, SIOClientImpl* impl, const SocketIO::Delegate& delegate);
  virtual ~SIOClient();
  
  // Socket.IO stack --------------------------------------------------------
  
  void Disconnect();
  
private:
  std::string host_;
  std::string path_;
  int port_;
  bool connected_;
  
  SocketIO::Delegate* delegate_;
  
  SIOClientImpl* socket_;
  
  // Events --------------------------------------------------------
  
  void OnOpen();
  
  void OnConnect();
  
  void OnSocketClosed();
  
  void FireEvent(const std::string& eventname, const std::string& data);
  
  
  DISALLOW_COPY_AND_ASSIGN(SIOClient);
};

NS_SKR_END

#endif /* SAKURA_SIO_CLIENT_H_ */
