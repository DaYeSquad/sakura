//
//  websocket_thread_helper.cpp
//  sakura
//
//  Created by Frank Lin on 12/21/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#include "websocket_thread_helper.h"

#import <Foundation/Foundation.h>

USING_NS_SKR;

////////////////////////////////////////////////////////////////////////////////
// WebsocketThreadHelper, public:

// Notify --------------------------------------------------------

void WebsocketThreadHelper::NotifyWebsocketMessage(std::unique_ptr<WebsocketMessage> ws_msg) {
  __block std::unique_ptr<WebsocketMessage> block_msg(new WebsocketMessage());
  block_msg->type = ws_msg->type;
  block_msg->obj = ws_msg->obj;
  
  dispatch_async(dispatch_get_main_queue(), ^{
    if (websocket_) {
      websocket_->OnWebsocketMessage(std::move(block_msg));
    }
  });
}
