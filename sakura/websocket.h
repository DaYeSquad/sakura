//
//  Websocket.hpp
//  libwebsocketDemo
//
//  Created by Frank Lin on 12/14/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#ifndef SAKURA_WEBSOCKET_H_
#define SAKURA_WEBSOCKET_H_

#include <stdio.h>

#include <string>
#include <memory>

#include "macros.h"

#if SKR_PLATFORM == SKR_PLATFORM_WIN32
#include "sakura/win32/compact/sakura_stdc.h"
#endif

struct libwebsocket;
struct libwebsocket_context;
struct libwebsocket_protocols;

NS_SKR_BEGIN

class WebsocketThreadHelper;
class WebsocketMessage;

/// C++ wrapper of libwebsockets inspired by Cocos2d-x::Network::WebSockets.
///
/// @since 1.0.1
/// @author Frank Lin
class SKR_DLL Websocket {
public:
  
  enum class State {
    CONNECTING = 0,
    OPEN       = 1,
    CLOSING    = 2,
    CLOSED     = 3
  };
  
  enum class ErrorCode {
    TIME_OUT           = 0,
    CONNECTION_FAILURE = 1,
    UNKNOWN            = 2,
  };
  
  struct Data {
    Data()
    :bytes(nullptr),
    length(0),
    issued(0),
    is_binary(false) {}
    
    char* bytes;
    ssize_t length;
    ssize_t issued;
    bool is_binary;
  };
  
  // Delegate --------------------------------------------------------
  
  /// Delegate to process websocket events.
  ///
  /// @since 1.0.1
  /// @author Frank Lin
  class Delegate {
  public:
    
    virtual ~Delegate() {}
    
    // This function to be called after the client connection complete a handshake with the remote server.
    virtual void OnOpen(const Websocket *ws) = 0;
    
    // Server to client message
    virtual void OnMessage(const Websocket *ws, const Websocket::Data& data) = 0;
    
    // Called on websocket trying to close whose state is |CLOSING|.
    virtual void OnClose(const Websocket *ws) = 0;
    
    // 1. client connection is failed.
    // 2. the request client connection has been unable to complete a handshake with the remote server.
    // 3. the protocol won't get used at all after this callback and current _readyState is State::CONNECTING.
    // 4. when a socket descriptor needs to be removed from an external polling array.
    //  in is again the struct libwebsocket_pollargs containing the fd member to be removed.
    //  If you are using the internal polling loop, you can just ignore it and current _readyState is State::CONNECTING.
    virtual void OnError(const Websocket* ws, const Websocket::ErrorCode& error) = 0;
  };
  
  // Lifecycle --------------------------------------------------------
  
  Websocket();
  
  virtual ~Websocket();
  
  bool Init(const Websocket::Delegate& delegate, const std::string& url);
  
  // Getters/Setters --------------------------------------------------------
  
  Websocket::State ready_state() const { return ready_state_; }
  
  // Websocket --------------------------------------------------------
  
  void Connect();
  
  void Close();
  
  void Send(const std::string& message);
  
public:
  
  // Events --------------------------------------------------------
  
  int OnRunLoop();
  
  // Response to websocket message, should be called in |WebsocketThreadHelper::NotifyWebsocketMessage|
  void OnWebsocketMessage(std::unique_ptr<WebsocketMessage> msg);
  
  int OnSocketCallback(struct libwebsocket_context *ctx,
                       struct libwebsocket *wsi,
                       int reason,
                       void *user,
                       void *in,
                       ssize_t len);
private:
  std::string host_;
  unsigned int port_;
  std::string path_;
  int ssl_connection_;
  
  char* current_data_;
  ssize_t current_data_length_;
  ssize_t pending_frame_data_length_;
  
  struct libwebsocket* instance_;
  struct libwebsocket_context* context_;
  
  struct libwebsocket_protocols* protocols_;
  
  State ready_state_;
  
  Delegate* delegate_;
  
  std::unique_ptr<WebsocketThreadHelper> thread_helper_;
};

NS_SKR_END

#endif /* SAKURA_WEBSOCKET_H_ */
