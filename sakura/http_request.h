//
//  http_request.h
//  LessChatCore
//
//  Created by Frank Lin on 11/24/14.
//  Copyright (c) 2014 Frank Lin. All rights reserved.
//

#ifndef SAKURA_HTTP_REQUEST_H_
#define SAKURA_HTTP_REQUEST_H_

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>

#include "sakura/macros.h"

#if SKR_PLATFORM == SKR_PLATFORM_WIN32
#undef DELETE
#endif

NS_SKR_BEGIN

class SKR_DLL HttpRequest {
  friend class HttpClient;
  
public:
  enum Type {
    GET,
    POST,
    PUT,
    DELETE,
    UNKNOWN
  };
  
  // Creation and lifetime --------------------------------------------------------
  
  HttpRequest() {
    request_type_ = HttpRequest::Type::UNKNOWN;
    url_.clear();
    request_data_.clear();
    tag_.clear();
  }
  
  virtual ~HttpRequest() {}
  
  std::unique_ptr<HttpRequest> Clone() const;
  
  // Attribute --------------------------------------------------------
  
  Type request_type() const { return request_type_; }
  void set_request_type(Type type) { request_type_ = type; }
  
  void set_url(const std::string& url) { url_ = url; }
  std::string url() const { return url_; }
  
  std::string request_data() const { return request_data_; }
  void set_request_data(const std::string& buffer) { request_data_ = buffer; }
  
  std::string tag() const { return tag_; }
  void set_tag(const std::string tag) { tag_ = tag; }
  
  void AddValueToHttpHeaderField(const std::string& key, const std::string& value);
  
  void SetValueForHttpHeaderField(const std::string& key, const std::string& value);
  
  std::map<std::string /*key*/, std::string /*value*/> request_header() const { return request_header_; }
  void set_request_header(std::map<std::string /*key*/, std::string /*value*/> header) { request_header_ = header; }
  
  // Convinence method --------------------------------------------------------
  
  static std::string StringFromHttpRequestType(HttpRequest::Type type) {
    if (type == sakura::HttpRequest::Type::GET) {
      return "GET";
    } else if (type == sakura::HttpRequest::Type::POST) {
      return "POST";
    } else if (type == sakura::HttpRequest::Type::DELETE) {
      return "DELETE";
    } else if (type == sakura::HttpRequest::Type::PUT) {
      return "PUT";
    } else {
      return "";
    }
  }
  
protected:
  std::vector<char> raw_data_;
  
private:
  Type request_type_;
  std::string url_;
  std::string request_data_;
  std::string tag_;
  std::map<std::string, std::string> request_header_;
  
  
  DISALLOW_COPY_AND_ASSIGN(HttpRequest);
};

NS_SKR_END

#endif /* defined(SAKURA_HTTP_REQUEST_H_) */
