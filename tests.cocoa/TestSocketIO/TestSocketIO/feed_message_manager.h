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
  class FeedMessageManager: public sakura::SocketIO::Delegate {
  public:
    
    // Creation and lifetime --------------------------------------------------------
    
    FeedMessageManager();
    
    virtual ~FeedMessageManager();
    
    void Connect();
    
    // sakura::SocketIO::Delegate --------------------------------------------------------
    
    virtual void OnConnect(sakura::SIOClient* client) {
      printf("FeedMessageManager OnConnect fire\n");
    }
    
    virtual void OnMessage(sakura::SIOClient* client, const std::string& data) {
      printf("FeedMessageManager OnMessage %s\n", data.c_str());
    }
    
    virtual void OnClose(sakura::SIOClient* client) {
      printf("FeedMessageManager socket close\n");
    }
    
    virtual void OnError(sakura::SIOClient* client, const std::string& data) {
      printf("FeedMessageManager error happens %s\n", data.c_str());
    }
    
  private:
    sakura::SIOClient *_sioClient;
  };
}

#endif /* feed_message_manager_hpp */
