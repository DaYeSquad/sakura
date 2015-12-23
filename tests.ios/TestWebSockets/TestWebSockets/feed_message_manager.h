//
//  feed_message_manager.hpp
//  TestSocketIO
//
//  Created by Frank Lin on 12/22/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#ifndef feed_message_manager_hpp
#define feed_message_manager_hpp

#include <string>
#include <sakura/sakura.h>

namespace worktile {
  class FeedMessageManager: public sakura::Websocket::Delegate {
  public:
    
    // Creation and lifetime --------------------------------------------------------
    
    FeedMessageManager();
    
    virtual ~FeedMessageManager();
    
    void Connect();
    
    void Send(const std::string& message);
    
    // sakura::SocketIO::Delegate --------------------------------------------------------
    
    // This function to be called after the client connection complete a handshake with the remote server.
    virtual void OnOpen(const sakura::Websocket *ws) {
      
    }
    
    virtual void OnMessage(const sakura::Websocket *ws, const sakura::Websocket::Data& data) {
      sakura::log_event("on message %s", data.bytes);
    }
    
    virtual void OnClose(const sakura::Websocket *ws) {
      
    }
    
    virtual void OnError(const sakura::Websocket* ws, const sakura::Websocket::ErrorCode& error) {
      
    }
    
  private:
    sakura::Websocket *_websocket;
  };
}

#endif /* feed_message_manager_hpp */
