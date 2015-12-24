//
//  websocket_thread_helper.cpp
//  sakura
//
//  Created by Frank Lin on 12/21/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#include "websocket_thread_helper.h"

NS_SKR_BEGIN

////////////////////////////////////////////////////////////////////////////////
// WebsocketThreadHelper, public:

// Creation and lifetime --------------------------------------------------------

WebsocketThreadHelper::WebsocketThreadHelper()
:socket_thread_(nullptr),
websocket_(nullptr),
need_quit_(false) {
}

WebsocketThreadHelper::~WebsocketThreadHelper() {
  JoinThread();
  
  SKR_SAFE_DELETE(socket_thread_);
}

// Thread stack --------------------------------------------------------

void WebsocketThreadHelper::CreateThread(const Websocket& ws) {
  websocket_ = const_cast<Websocket*>(&ws);
  
  socket_thread_ = new std::thread(&WebsocketThreadHelper::OnThreadCreate, this);
}

void WebsocketThreadHelper::OnThreadCreate() {
  websocket_->Connect();
  
  while (!need_quit_) {
    if (websocket_->OnRunLoop()) {
      break;
    }
  }
}

void WebsocketThreadHelper::QuitThread() {
  need_quit_ = true;
}

void WebsocketThreadHelper::JoinThread() {
  if (socket_thread_->joinable()) {
    socket_thread_->join();
  }
}

void WebsocketThreadHelper::DispatchWebsocketMessage(std::unique_ptr<WebsocketMessage> ws_msg) {
  std::lock_guard<std::mutex> lg(thread_messages_mutex_);
  thread_messages_.push_back(std::move(ws_msg));
}

NS_SKR_END