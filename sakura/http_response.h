//
//  http_response.h
//  LessChatCore
//
//  Created by Frank Lin on 11/26/14.
//  Copyright (c) 2014 Frank Lin. All rights reserved.
//

#ifndef SAKURA_HTTP_RESPONSE_H_
#define SAKURA_HTTP_RESPONSE_H_

#include <map>
#include <memory>
#include <vector>
#include <string>

#include "sakura/macros.h"

NS_SKR_BEGIN

class HttpRequest;

class SKR_DLL HttpResponse {
public:
  
  // Creation and lifetime --------------------------------------------------------
  
  explicit HttpResponse(std::unique_ptr<HttpRequest> request);
  
  virtual ~HttpResponse() {}
  
  // API --------------------------------------------------------
  
  const HttpRequest* http_request() const;
  
  bool is_succeed() const { return succeed_; }
  void set_succeed(bool succeed) { succeed_ = succeed; }
  
  // Returns response data as string
  std::string response_data_as_string() const;
  
  std::vector<char> response_data() const { return response_data_; }
  void set_response_data(std::vector<char> buffer) { response_data_ = buffer; }
  
  void AddValueToHttpHeaderField(const std::string& key, const std::string& value) {
    response_header_.insert(std::pair<std::string, std::string>(key, value));
  }
  
  std::string error_buffer() const { return error_buffer_; }
  void set_error_buffer(const std::string& error) { error_buffer_ = error; }
  
  int64_t response_code() const { return response_code_; }
  void set_response_code(int64_t code) { response_code_ = code; }
  
  std::map<std::string, std::string> response_header() const { return response_header_;  }

public:
  
  std::unique_ptr<HttpRequest> http_request_;
  bool succeed_;
  std::vector<char> response_data_;
  std::map<std::string, std::string> response_header_;
  int64_t response_code_;
  std::string error_buffer_;
  
private:
  DISALLOW_COPY_AND_ASSIGN(HttpResponse);
};

NS_SKR_END

#endif /* defined(SAKURA_HTTP_RESPONSE_H_) */
