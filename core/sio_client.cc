//
//  sio_client.cpp
//  sakura
//
//  Created by Frank Lin on 12/22/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#include "sio_client.h"

#include <string>
#include <sstream>

#include "log.h"

#include "sio_client_impl.h"

USING_NS_SKR;

using std::string;
using std::unique_ptr;

////////////////////////////////////////////////////////////////////////////////
// SIOClient, public:

// Creation and lifetime --------------------------------------------------------

SIOClient::SIOClient(const std::string& host,
                     int port,
                     const std::string& path,
                     SIOClientImpl* impl,
                     const SocketIO::Delegate& delegate)
:port_(port),
host_(host),
path_(path),
connected_(false),
socket_(impl) {
  delegate_ = const_cast<SocketIO::Delegate*>(&delegate);
}

SIOClient::~SIOClient() {
  if (connected_) {
    socket_->DisconnectFromEndpoint(path_);
  }
}

// Socket.IO stack --------------------------------------------------------

void SIOClient::Disconnect() {
  connected_ = false;
  socket_->DisconnectFromEndpoint(path_);
}

////////////////////////////////////////////////////////////////////////////////
// SIOClient, private:

// Events --------------------------------------------------------

void SIOClient::OnOpen() {
  if (path_ != "/") {
    socket_->ConnectToEndpoint(path_);
  }
}

void SIOClient::OnConnect() {
  connected_ = true;
}

void SIOClient::OnSocketClosed() {
  connected_ = false;
  delegate_->OnClose(this);
}

void SIOClient::FireEvent(const std::string& eventname, const std::string& data) {
  
}
