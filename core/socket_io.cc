//
//  socket_io.cpp
//  sakura
//
//  Created by Frank Lin on 12/21/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#include "socket_io.h"

#include <sstream>

#include "sio_client_impl.h"
#include "sio_client.h"

USING_NS_SKR;
using std::string;

SocketIO* SocketIO::shared_instance_ = nullptr;

////////////////////////////////////////////////////////////////////////////////
// SocketIO, public:

// Creation and lifetime --------------------------------------------------------

SocketIO* SocketIO::SharedInstance() {
  if (shared_instance_ == nullptr) {
    shared_instance_ = new SocketIO();
  }
  return shared_instance_;
}

void SocketIO::PurgeSharedInstance() {
  SKR_SAFE_DELETE(shared_instance_);
}

SIOClient* SocketIO::Connect(const std::string& uri,
                             const SocketIO::Delegate& delegate,
                             const std::map<std::string, std::string>& handshake_append_headers) {
  string host = uri;
  int port = 0;
  size_t pos = 0;
  bool use_ssl = false;
  
  pos = host.find("wss://");
  if (pos != string::npos) {
    use_ssl = true;
  }
  
  pos = host.find("//");
  if (pos != std::string::npos) {
    host.erase(0, pos + 2);
  }
  
  pos = host.find(":");
  if (pos != std::string::npos) {
    port = atoi(host.substr(pos + 1, host.size()).c_str());
  }
  
  pos = host.find("/", 0);
  string path = "/";
  if (pos != std::string::npos) {
    path += host.substr(pos + 1, host.size());
  }
  
  pos = host.find(":");
  if (pos != std::string::npos) {
    host.erase(pos, host.size());
  }
  else if ((pos = host.find("/")) != std::string::npos) {
    host.erase(pos, host.size());
  }
  
  std::stringstream ss;
  ss << host << ":" << port;

  SIOClientImpl* socket = nullptr;
  SIOClient* connection = nullptr;
  
  socket = SocketIO::SharedInstance()->GetSocket(uri);
  if (socket == nullptr) {
    socket = new SIOClientImpl(host, port, use_ssl);
    connection = new SIOClient(host, port, path, socket, delegate);
    
    socket->AddClient(path, connection);
    socket->Connect(handshake_append_headers);
  } else {
    connection = socket->GetClient(path);
    
    if (!connection) {
      connection = new SIOClient(host, port, path, socket, delegate);
      socket->AddClient(path, connection);
      socket->ConnectToEndpoint(path);
    } else {
      log_event("SocketIO disconnect from previous client");
      
      connection->Disconnect();
      
      SIOClientImpl* new_socket = nullptr;
      SIOClient *new_connection = nullptr;
      
      new_socket = new SIOClientImpl(host, port, use_ssl);
      new_connection = new SIOClient(host, port, path, new_socket, delegate);
      
      new_socket->AddClient(path, connection);
      new_socket->Connect(handshake_append_headers);
      
      return new_connection;
    }
  }
  return connection;
}

////////////////////////////////////////////////////////////////////////////////
// SocketIO, private:

// Creation and lifetime --------------------------------------------------------

SocketIO::SocketIO() {
  
}

SocketIO::~SocketIO() {
  
}

SIOClientImpl* SocketIO::GetSocket(const std::string& uri) {
  if (sockets_.find(uri) != sockets_.end()) {
    return sockets_.at(uri);
  } else {
    return nullptr;
  }
}

void SocketIO::AddSocket(const std::string& uri, SIOClientImpl* socket) {
  sockets_.insert(std::make_pair(uri, socket));
}

void SocketIO::RemoveSocket(const std::string& uri) {
  sockets_.erase(uri);
}
