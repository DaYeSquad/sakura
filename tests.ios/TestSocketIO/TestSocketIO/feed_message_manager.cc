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
//  std::map<std::string, std::string> handshakeHeader;
//  handshakeHeader.insert(std::make_pair("app_key", "12bed94c25c8df"));
//  handshakeHeader.insert(std::make_pair("client_version", "3.9.0"));
//  handshakeHeader.insert(std::make_pair("device_id", "C214C7F0-0783-4ADC-B852-0B35F960B5BA"));
//  handshakeHeader.insert(std::make_pair("signature", "db1d2234"));
//  handshakeHeader.insert(std::make_pair("store_from", "AppStore"));
//  handshakeHeader.insert(std::make_pair("timestamp", "1453348983"));
//  handshakeHeader.insert(std::make_pair("version", "1"));
//  handshakeHeader.insert(std::make_pair("access_token", "7de8498a92d648fe8b545dd009a8693a"));
//  
//  _sioClient = sakura::SocketIO::Connect("wss://feed2.worktile.com:443", *this, handshakeHeader);
  
    std::map<std::string, std::string> handshakeHeader;
    handshakeHeader.insert(std::make_pair("app_key", "12bed94c25c8df"));
    handshakeHeader.insert(std::make_pair("client_version", "3.9.0"));
    handshakeHeader.insert(std::make_pair("device_id", "C214C7F0-0783-4ADC-B852-0B35F960B5BA"));
    handshakeHeader.insert(std::make_pair("signature", "06d42915"));
    handshakeHeader.insert(std::make_pair("store_from", "AppStore"));
    handshakeHeader.insert(std::make_pair("timestamp", "1450775132"));
    handshakeHeader.insert(std::make_pair("version", "1"));
    handshakeHeader.insert(std::make_pair("access_token", "e16946a1ba1748a784970f25063e4662"));
//    _sioClient = sakura::SocketIO::Connect("wss://feed.worktile.local:443", *this, handshakeHeader);
    _sioClient = sakura::SocketIO::Connect("ws://feed.worktile.local:5280", *this, handshakeHeader);
}
