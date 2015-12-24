//
//  http_multipart_request.cpp
//  WorktileCore
//
//  Created by Frank Lin on 11/10/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#include "http_multipart_request.h"

using std::string;
using std::vector;
using std::unique_ptr;

static std::string kboundary = "----WebKitFormBoundaryQH5P0ytGqOFkQEh6";
static std::string kEscapeQuote = "\"";

NS_SKR_BEGIN

////////////////////////////////////////////////////////////////////////////////
// HttpMultipartRequest, public:

// Creation and lifetime --------------------------------------------------------

HttpMultipartRequest::HttpMultipartRequest()
:HttpRequest() {
  SetValueForHttpHeaderField("Content-type", "multipart/form-data; boundary=" + kboundary);
  
  string end_boundary = "\r\n--" + kboundary + "--" + "\r\n";
  for (size_t i = 0; i < end_boundary.size(); ++i) {
    raw_data_.push_back(end_boundary[i]);
  }
}

void HttpMultipartRequest::AddPart(const std::string& name, const std::string& value) {
  string part = "--" + kboundary + "\r\n";
  part += "Content-Disposition: form-data; name=\"" + name + "\"\r\n\r\n";
  part += value + "\r\n";
  
  // add to raw data
  vector<char>::iterator it = raw_data_.begin();
  for (size_t i = 0; i < part.size(); ++i) {
    it = raw_data_.insert(it, part[i]);
    ++it;
  }
}

void HttpMultipartRequest::AddFileBody(const std::string& name,
                                       const std::string& file_name,
                                       const std::string& file_type,
                                       const std::vector<char>& file_data) {
  string part = "--" + kboundary + "\r\n";
  part += "Content-Disposition: form-data; name=\"" + name + "\"; filename=\"" + file_name + kEscapeQuote + "\r\n";
  part += "Content-Type: " + file_type + "\r\n\r\n";
  
  // add to raw data
  vector<char>::iterator it = raw_data_.begin();
  for (size_t i = 0; i < part.size(); ++i) {
    it = raw_data_.insert(it, part[i]);
    ++it;
  }
  
  // add file to raw data
  for (size_t i = 0; i < file_data.size(); ++i) {
    it = raw_data_.insert(it, file_data[i]);
    ++it;
  }
}

NS_SKR_END
