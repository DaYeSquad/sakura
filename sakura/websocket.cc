//
//  Websocket.cpp
//  libwebsocketDemo
//
//  Created by Frank Lin on 12/14/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#include "websocket.h"

#include <stdlib.h>

#include <thread>
#include <list>
#include <mutex>

#include "libwebsockets.h"

#include "sakura/log.h"
#include "sakura/websocket_thread_helper.h"

using std::string;
using std::unique_ptr;

static const size_t kWebsocketWriteBufferSize = 2048;

////////////////////////////////////////////////////////////////////////////////
// WebsocketCallbackWrapper, public:

// Callback wrapper --------------------------------------------------------

NS_SKR_BEGIN

/// Convinent callback wrapper.
///
/// @since 1.0.1
/// @author Frank Lin
class WebSocketCallbackWrapper {
public:
  static int OnSocketCallback(struct libwebsocket_context *ctx,
                              struct libwebsocket *wsi,
                              enum libwebsocket_callback_reasons reason,
                              void *user,
                              void *in,
                              size_t len) {
    // Gets the user data from context. We know that it's a 'WebSocket' instance.
    Websocket* websocket = reinterpret_cast<Websocket*>(libwebsocket_context_user(ctx));
    if (websocket) {
      return websocket->OnSocketCallback(ctx, wsi, reason, user, in, len);
    }
    return 0;
  }
};

NS_SKR_END

NS_SKR_BEGIN

////////////////////////////////////////////////////////////////////////////////
// Websocket, public:

// Creation and lifetime --------------------------------------------------------

Websocket::Websocket()
:context_(nullptr),
ready_state_(State::CONNECTING),
instance_(nullptr),
ssl_connection_(0),
protocols_(nullptr),
current_data_length_(0),
current_data_(nullptr),
pending_frame_data_length_(0),
delegate_(nullptr) {
  
}

Websocket::~Websocket() {
  Close();
  
  for (int i = 0; protocols_[i].callback != nullptr; ++i) {
    delete[] protocols_[i].name;
  }
  
  delete[] protocols_;
}

bool Websocket::Init(const Websocket::Delegate& delegate, const std::string &url) {
  bool use_ssl = false;
  std::string host = url;
  size_t pos = 0;
  int port = 80;
  
  // ws://
  pos = host.find("ws://");
  if (pos == 0) host.erase(0,5);
  
  pos = host.find("wss://");
  if (pos == 0) {
    host.erase(0,6);
    use_ssl = true;
  }
  
  pos = host.find(":"); // port
  if (pos != std::string::npos) {
    port = atoi(host.substr(pos+1, host.size()).c_str());
  }
  
  pos = host.find("/", 0); // path
  std::string path = "/";
  if (pos != std::string::npos) {
    path += host.substr(pos + 1, host.size());
  }
  
  pos = host.find(":");
  if (pos != std::string::npos) {
    host.erase(pos, host.size());
  } else if((pos = host.find("/")) != std::string::npos) {
    host.erase(pos, host.size());
  }
  
  host_ = host;
  port_ = port;
  path_ = path;
  ssl_connection_ = use_ssl ? 2 : 0; // 2 means accept self-signed certificate
  
  sakura::log_event("Websocket host is %s, port is %d, path is %s \n", host_.c_str(), port_, path_.c_str());
  
  delegate_ = const_cast<Delegate*>(&delegate);
  
  protocols_ = new libwebsocket_protocols[2];
  memset(protocols_, 0, sizeof(libwebsocket_protocols) * 2);
  
  char *name = new char[20];
  strcpy(name, "default-protocol");
  protocols_[0].name = name;
  protocols_[0].callback = WebSocketCallbackWrapper::OnSocketCallback;
  
  thread_helper_ = unique_ptr<WebsocketThreadHelper>(new WebsocketThreadHelper());
  thread_helper_->CreateThread(*this);
  
  return true;
}

// Websocket --------------------------------------------------------

void Websocket::Close() {
  if (ready_state_ == State::CLOSED || ready_state_ == State::CLOSED) {
    return;
  }
  
  sakura::log_event("Websocket connection closed by client");
  
  ready_state_ = State::CLOSED;
  
  thread_helper_->JoinThread();
  
  // OnClose callback needs to be invoked at the end of this method
  // since websocket instance may be deleted in 'OnClose'.
  delegate_->OnClose(this);
}

void Websocket::Connect() {
  log_event("Websocket::Connect %d \n", protocols_ == nullptr);
  
  struct lws_context_creation_info info;
  memset(&info, 0, sizeof(info));
  
  info.port = CONTEXT_PORT_NO_LISTEN;
  info.protocols = protocols_;
  info.gid = -1;
  info.uid = -1;
  info.user = static_cast<void*>(this);
  
  context_ = libwebsocket_create_context(&info);
  
  if (context_ != nullptr) {
    ready_state_ = State::CONNECTING;
    string name = "default-protocol";
    
    instance_ = libwebsocket_client_connect(context_,
                                            host_.c_str(),
                                            port_,
                                            ssl_connection_,
                                            path_.c_str(),
                                            host_.c_str(),
                                            host_.c_str(),
                                            name.c_str(),
                                            -1);
    if (instance_ == nullptr) {
      log_error("Websocket error instance is nullptr");
      ready_state_ = State::CLOSING;
      
      thread_helper_->NotifyWebsocketMessageByType(WebsocketMessage::Type::NOTIFY_ERROR);
    }
  }
}

void Websocket::Send(const std::string& message) {
  if (ready_state_ == State::OPEN) {
    Websocket::Data* data = new Websocket::Data();
    data->bytes = new char[message.length() + 1];
    strcpy(data->bytes, message.c_str());
    data->length = static_cast<ssize_t>(message.length());
    
    unique_ptr<WebsocketMessage> ws_msg(new WebsocketMessage());
    ws_msg->type = WebsocketMessage::Type::SENDING_STRING;
    ws_msg->obj = data;
    
    thread_helper_->DispatchWebsocketMessage(std::move(ws_msg));
  }
}

// Events --------------------------------------------------------

int Websocket::OnRunLoop() {
  if (ready_state_ == State::CLOSED || ready_state_ == State::CLOSING) {
    libwebsocket_context_destroy(context_);
    return 1; // exist run loop
  }
  
  if (context_ && ready_state_ != State::CLOSING && ready_state_ != State::CLOSED) {
    libwebsocket_service(context_, 0);
  }
  
  std::this_thread::sleep_for(std::chrono::microseconds(50));
  
  return 0;
}

int Websocket::OnSocketCallback(struct libwebsocket_context *ctx,
                                struct libwebsocket *wsi,
                                int reason,
                                void *user,
                                void *in,
                                ssize_t len) {
  assert(context_ == nullptr || ctx == context_); // Invalid context
  assert(instance_ == nullptr || wsi == nullptr || wsi == instance_); //Invaild websocket instance
  
  switch (reason) {
    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
    case LWS_CALLBACK_PROTOCOL_DESTROY:
    case LWS_CALLBACK_DEL_POLL_FD: {
      log_error("OnSocketCallback some error happens %d \n", reason);
      if (reason == LWS_CALLBACK_CLIENT_CONNECTION_ERROR ||
          (reason == LWS_CALLBACK_PROTOCOL_DESTROY && ready_state_ == State::CONNECTING) ||
          (reason == LWS_CALLBACK_DEL_POLL_FD && ready_state_ == State::CONNECTING)) {
        thread_helper_->NotifyWebsocketMessageByType(WebsocketMessage::Type::NOTIFY_ERROR);
        ready_state_ = State::CLOSING;
      } else if (reason == LWS_CALLBACK_PROTOCOL_DESTROY && ready_state_ == State::CLOSING) {
        thread_helper_->NotifyWebsocketMessageByType(WebsocketMessage::Type::NOTIFY_CLOSE);
      }
    }
      break;
    case LWS_CALLBACK_CLIENT_ESTABLISHED: {
      log_event("OnSocketCallback LWS_CALLBACK_CLIENT_ESTABLISHED \n");
      ready_state_ = State::OPEN;
      libwebsocket_callback_on_writable(context_, instance_);
      thread_helper_->NotifyWebsocketMessageByType(WebsocketMessage::Type::NOTIFY_OPEN);
    }
      break;
    case LWS_CALLBACK_CLIENT_WRITEABLE: {
      int num_bytes_write = 0;
      std::lock_guard<std::mutex> lg(thread_helper_->thread_messages_mutex_);
      
      auto it = thread_helper_->thread_messages_.begin();
      for (; it != thread_helper_->thread_messages_.end();) {
        if ((*it)->type == WebsocketMessage::Type::SENDING_STRING) {
          Data* data = reinterpret_cast<Data*>((*it)->obj);
          
          size_t remaining = data->length - data->issued;
          size_t n = std::min(remaining, kWebsocketWriteBufferSize);
          
          unsigned char* buf = new unsigned char[LWS_SEND_BUFFER_PRE_PADDING + n +LWS_SEND_BUFFER_POST_PADDING];
          memcpy(reinterpret_cast<char*>(&buf[LWS_SEND_BUFFER_PRE_PADDING]), data->bytes + data->issued, n);
          
          int write_protocol = 0;
          
          if (data->issued == 0) {
            if ((*it)->type == WebsocketMessage::Type::SENDING_STRING) {
              write_protocol = LWS_WRITE_TEXT;
            } else {
              write_protocol = LWS_WRITE_BINARY;
            }
            
            // If we have more than 1 fragment
            if (data->length > kWebsocketWriteBufferSize) {
              write_protocol |= LWS_WRITE_NO_FIN;
            }
          } else {
            // we are in the middle of fragments
            write_protocol = LWS_WRITE_CONTINUATION;
            // and if not in the last fragment
            if (remaining != n)
              write_protocol |= LWS_WRITE_NO_FIN;
          }
          
          num_bytes_write = libwebsocket_write(instance_,
                                               &buf[LWS_SEND_BUFFER_PRE_PADDING],
                                               n,
                                               (libwebsocket_write_protocol)write_protocol);
          
          if (num_bytes_write < 0) { // buffer overrun?
            break;
          } else if (remaining != n) { // do we have another fragments to send?
            data->issued += n;
            break;
          } else {
            delete [] data->bytes;
            delete data;
            delete [] buf;
            thread_helper_->thread_messages_.erase(it++);
          }
        }
      }
      
      // get notified as soon as can write again
      libwebsocket_callback_on_writable(context_, instance_);
    }
      break;
    case LWS_CALLBACK_CLOSED: {
      log_event("OnSocketCallback LWS_CALLBACK_CLOSED \n");
      
      thread_helper_->QuitThread();
      if (ready_state_ != State::CLOSED) {
        ready_state_ = State::CLOSED;
        thread_helper_->NotifyWebsocketMessageByType(WebsocketMessage::Type::NOTIFY_CLOSE);
      }
    }
      break;
    case LWS_CALLBACK_CLIENT_RECEIVE: {
      log_event("OnSocketCallback LWS_CALLBACK_CLIENT_RECEIVE, len is %zd \n", len);
      
      if (in && len > 0) {
        // accumulate the data (increasing the buffer as we go)
        if (current_data_length_ == 0) {
          current_data_ = new char[len];
          memcpy(current_data_, in, len);
          current_data_length_ = len;
        } else {
          char* new_data = new char[current_data_length_ + len];
          memcpy(new_data, current_data_, current_data_length_);
          memcpy(new_data + current_data_length_, in, len);
          delete [] current_data_;
          current_data_ = new_data;
          current_data_length_ = current_data_length_ + len;
        }
        
        pending_frame_data_length_ = libwebsockets_remaining_packet_payload(instance_);
        
        if (pending_frame_data_length_ > 0) {
          // pending data to receive
        }
        
        // if no more data pending, send it to the client thread
        if (pending_frame_data_length_ == 0) {
          Websocket::Data* data = new Websocket::Data();
          
          char* bytes = nullptr;
          if (lws_frame_is_binary(instance_)) {
            bytes = new char[current_data_length_];
            data->is_binary = true;
          } else {
            bytes = new char[current_data_length_ + 1];
            bytes[current_data_length_] = '\0';
            data->is_binary = false;
          }
          
          memcpy(bytes, current_data_, current_data_length_);
          
          data->bytes = bytes;
          data->length = current_data_length_;
          
          unique_ptr<WebsocketMessage> ws_msg(new WebsocketMessage());
          ws_msg->type = WebsocketMessage::Type::NOTIFY_MESSAGE;
          ws_msg->obj = static_cast<void*>(data);
          
          delete [] current_data_;
          current_data_ = nullptr;
          current_data_length_ = 0;
          
          thread_helper_->NotifyWebsocketMessage(std::move(ws_msg));
        }
      }
    }
      break;
    case LWS_CALLBACK_PROTOCOL_INIT:
      log_event("OnSocketCallback LWS_CALLBACK_PROTOCOL_INIT \n");
      break;
    default:
      break;
  }
  return 0;
}

void Websocket::OnWebsocketMessage(std::unique_ptr<WebsocketMessage> msg) {
  if (msg->type == WebsocketMessage::Type::NOTIFY_OPEN) {
    delegate_->OnOpen(this);
  } else if (msg->type == WebsocketMessage::Type::NOTIFY_MESSAGE) {
    Websocket::Data* data = reinterpret_cast<Websocket::Data*>(msg->obj);
    delegate_->OnMessage(this, *data);
    SKR_SAFE_DELETE_ARRAY(data->bytes);
    SKR_SAFE_DELETE(data);
  } else if (msg->type == WebsocketMessage::Type::NOTIFY_CLOSE) {
    thread_helper_->JoinThread();
    delegate_->OnClose(this);
  } else if (msg->type == WebsocketMessage::Type::NOTIFY_ERROR) {
    Websocket::ErrorCode error = ErrorCode::CONNECTION_FAILURE;
    delegate_->OnError(this, error);
  }
}

NS_SKR_END