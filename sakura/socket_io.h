//
//  socket_io.hpp
//  sakura
//
//  Created by Frank Lin on 12/21/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#ifndef SAKURA_SOCKET_IO_H_
#define SAKURA_SOCKET_IO_H_

#include <string>
#include <map>

#include "macros.h"
#include "log.h"

NS_SKR_BEGIN

class SIOClientImpl;
class SIOClient;

/// SIO websockets manager.
///
/// @since 1.0.1
/// @author Frank Lin
class SocketIO {
public:
  
  /// Delegate to process socket.io events
  ///
  /// @since 1.0.1
  /// @author Frank Lin
  class Delegate {
  public:
    
    virtual ~Delegate() {}
    
    virtual void OnConnect(SIOClient* client) {
#ifdef SKR_LOG_SIO
      sakura::log_event("SocketIO::Delegate OnConnect fired");
#endif
    }
    
    virtual void OnMessage(SIOClient* client, const std::string& data) {
#ifdef SKR_LOG_SIO
      sakura::log_event("SocketIO::Delegate OnMessage %s", data.c_str());
#endif
    }
    
    virtual void OnClose(SIOClient* client) = 0;
    
    virtual void OnError(SIOClient* client, const std::string& data) = 0;
  };
  
  // Creation and lifetime --------------------------------------------------------
  
  static SocketIO* SharedInstance();
  static void PurgeSharedInstance();
  
  static SIOClient* Connect(const std::string& uri,
                            const SocketIO::Delegate& delegate,
                            const std::map<std::string, std::string>& handshake_append_headers);
  
private:
  
  // Creation and lifetime --------------------------------------------------------
  
  SocketIO();
  virtual ~SocketIO();
  
  static SocketIO* shared_instance_;
  
  std::map<std::string /*uri*/, SIOClientImpl*> sockets_;
  
  SIOClientImpl* GetSocket(const std::string& uri);
  void AddSocket(const std::string& uri, SIOClientImpl* socket);
  void RemoveSocket(const std::string& uri);
  
  
  DISALLOW_COPY_AND_ASSIGN(SocketIO);
};

NS_SKR_END

#endif /* SAKURA_SOCKET_IO_H_ */
