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
  _websocket = new sakura::Websocket();
  _websocket->Init(*this, "wss://echo.websocket.org:443");
//  _websocket->Init(*this, "ws://echo.websocket.org");
}

void FeedMessageManager::Send(const std::string& message) {
  _websocket->Send(message);
}