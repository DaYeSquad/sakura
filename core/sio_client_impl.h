//
//  sio_client_impl.hpp
//  sakura
//
//  Created by Frank Lin on 12/21/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#ifndef SAKURA_SIO_CLIENT_IMPL_H_
#define SAKURA_SIO_CLIENT_IMPL_H_

#include <string>
#include <sstream>
#include <map>

#include "macros.h"

#include "websocket.h"

NS_SKR_BEGIN

class SIOPacket;
class SIOClient;

/// Socket.IO implementations based on websocket (libwebsockets)
/// Clients/endpoints may share the same impl to accomplish multiplexing on the same websocket
///
/// @since 1.0.1
/// @author Frank Lin
class SIOClientImpl: public Websocket::Delegate {
public:
  
  // Creation and lifetime --------------------------------------------------------
  
  SIOClientImpl(const std::string& host, int port, bool use_ssl=false)
  :host_(host),
  port_(port),
  connected_(false),
  use_ssl_(use_ssl) {
    std::stringstream ss;
    ss << host << ":" << port;
    uri_ = ss.str();
    
    websocket_ = nullptr;
  }
  
  virtual ~SIOClientImpl() {}
  
  // Socket.IO statck --------------------------------------------------------
  
  void Connect(const std::map<std::string, std::string>& user_headers);
  void Handshake(const std::map<std::string, std::string>& user_headers);
  void OpenSocket();
  
  void Disconnect();
  
  void Send(const SIOPacket& packet);
  void Send(const std::string& endpoint, const std::string& s);
  void Emit(const std::string& endpoint, const std::string& eventname, const std::string& args);
  
  void ConnectToEndpoint(const std::string& endpoint);
  void DisconnectFromEndpoint(const std::string& endpoint);
  
  // Getters/Setters --------------------------------------------------------
  
  SIOClient* GetClient(const std::string& endpoint);
  void AddClient(const std::string& endpoint, SIOClient* client);
  
public:
  
  // Websocket::Delegate override --------------------------------------------------------
  
  virtual void OnOpen(const Websocket *ws) OVERRIDE;
  
  virtual void OnMessage(const Websocket *ws, const Websocket::Data& data) OVERRIDE;
  
  virtual void OnClose(const Websocket *ws) OVERRIDE;
  
  virtual void OnError(const Websocket* ws, const Websocket::ErrorCode& error) OVERRIDE;
  
private:
  int port_;
  int heartbeat_;
  int timeout_;
  bool use_ssl_;
  
  std::string host_;
  std::string sid_;
  std::string uri_;
  
  bool connected_;
  
  std::unique_ptr<Websocket> websocket_;
  
  std::map<std::string /*endpoint*/, SIOClient* /*client*/> clients_;
  
  
  DISALLOW_COPY_AND_ASSIGN(SIOClientImpl);
};

NS_SKR_END

#endif /* SAKURA_SIO_CLIENT_IMPL_H_ */
