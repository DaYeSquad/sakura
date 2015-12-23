//
//  websocket_thread_helper.hpp
//  sakura
//
//  Created by Frank Lin on 12/21/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#ifndef SAKURA_WEBSOCKET_THREAD_HELPER_H_
#define SAKURA_WEBSOCKET_THREAD_HELPER_H_

#include <thread>
#include <list>

#include "websocket.h"
#include "log.h"

////////////////////////////////////////////////////////////////////////////////
// WebsocketThreadHelper, public:

// Threads --------------------------------------------------------

NS_SKR_BEGIN

/// Used to send message to |WebsocketThreadHelper| or notify user from |WebsocketThreadHelper|.
///
/// WARNING: Each platform can implement its |NotifyWebsocketMessage| on its preferred thread.
///
/// @since 1.0.1
/// @author Frank Lin
class WebsocketMessage {
public:
  enum Type {
    SENDING_STRING = 0,
    SENDING_BINARY = 1,
    
    NOTIFY_MESSAGE = 2,
    NOTIFY_ERROR   = 3,
    NOTIFY_CLOSE   = 4,
    NOTIFY_OPEN    = 5,
  };
  
  WebsocketMessage()
  :type(SENDING_STRING),
  obj(nullptr) {}
  
  Type type;
  void* obj;
};

NS_SKR_END

NS_SKR_BEGIN

/// Used for Websocket thread management.
///
/// @since 1.0.1
/// @author Frank Lin
class WebsocketThreadHelper {
public:
  
  friend class Websocket;
  
  // Creation and lifetime --------------------------------------------------------
  
  WebsocketThreadHelper();
  
  virtual ~WebsocketThreadHelper();
  
  // Thread stack --------------------------------------------------------
  
  void CreateThread(const Websocket& ws);
  
  void OnThreadCreate();
  
  void QuitThread();
  
  // waits the thread to exist
  void JoinThread();
  
  void DispatchWebsocketMessage(std::unique_ptr<WebsocketMessage> ws_msg);
  
  void NotifyWebsocketMessageByType(WebsocketMessage::Type type) {
    std::unique_ptr<WebsocketMessage> msg(new WebsocketMessage());
    msg->type = type;
    NotifyWebsocketMessage(std::move(msg));
  }
  
  // Platform specific --------------------------------------------------------
  
  // call |Websocket::OnWebsocketMessage| in right thread
  void NotifyWebsocketMessage(std::unique_ptr<WebsocketMessage> ws_msg);
  
private:
  std::thread* socket_thread_;
  Websocket* websocket_;
  
  std::list<std::unique_ptr<WebsocketMessage>> thread_messages_;
  std::mutex thread_messages_mutex_;
  
  bool need_quit_;
  
  
  DISALLOW_COPY_AND_ASSIGN(WebsocketThreadHelper);
};

NS_SKR_END

#endif /* SAKURA_WEBSOCKET_THREAD_HELPER_H_ */
