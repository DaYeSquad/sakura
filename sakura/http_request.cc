//
//  http_request.cpp
//  LessChatCore
//
//  Created by Frank Lin on 12/5/14.
//
//

#include "sakura/http_request.h"

using std::unique_ptr;

NS_SKR_BEGIN

////////////////////////////////////////////////////////////////////////////////
// HTTPRequest, public:

// Creation and lifetime --------------------------------------------------------

std::unique_ptr<HttpRequest> HttpRequest::Clone() const {
  unique_ptr<HttpRequest> request(new HttpRequest());
  request->set_request_type(request_type_);
  request->set_url(url_);
  request->set_request_header(request_header_);
  request->set_request_data(request_data_);
  request->set_tag(tag_);
  return request;
}

// Property getters & setters --------------------------------------------------------

void HttpRequest::AddValueToHttpHeaderField(const std::string& key, const std::string& value) {
	request_header_.insert(std::pair<std::string, std::string>(key, value));
}

void HttpRequest::SetValueForHttpHeaderField(const std::string& key, const std::string& value) {
	request_header_[key] = value;
}

////////////////////////////////////////////////////////////////////////////////
// HTTPRequest, private:

std::string HttpRequest::IdentifierTag() const {
  time_t now;
  time(&now);
  return url_ + std::to_string(now);
}

NS_SKR_END