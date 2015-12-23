//
//  feed_message_manager.cpp
//  TestSocketIO
//
//  Created by Frank Lin on 12/22/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#include "feed_message_manager.h"

using namespace worktile;

FeedMessageManager::FeedMessageManager() {
  
}

FeedMessageManager::~FeedMessageManager() {
  
}

void FeedMessageManager::Connect() {
  std::map<std::string, std::string> handshakeHeader;
  _sioClient = sakura::SocketIO::Connect("wss://xxx.com:443", *this, handshakeHeader);
}
