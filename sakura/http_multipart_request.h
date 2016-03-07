//
//  http_multipart_request.hpp
//  WorktileCore
//
//  Created by Frank Lin on 11/10/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#ifndef SAKURA_HTTP_MULTIPART_REQUEST_H_
#define SAKURA_HTTP_MULTIPART_REQUEST_H_

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>

#include "sakura/macros.h"
#include "sakura/http_request.h"

NS_SKR_BEGIN

/// Used for multipart request to PUT or POST.
class SKR_DLL HttpMultipartRequest: public sakura::HttpRequest {
public:
  
  // Creation and lifetime --------------------------------------------------------
  
  HttpMultipartRequest();
  
  virtual ~HttpMultipartRequest() {}
  
  // Multipart --------------------------------------------------------
  
  // adds string body
  void AddPart(const std::string& name, const std::string& value);
  
  // adds binary file body
  void AddFileBody(const std::string& name,
                   const std::string& file_name,
                   const std::string& mime_type,
                   const std::vector<char>& file_data);
  
  // Getter --------------------------------------------------------
  
  std::vector<char> raw_data() const { return raw_data_; }
  
private:
  DISALLOW_COPY_AND_ASSIGN(HttpMultipartRequest);
};

NS_SKR_END

#endif /* http_multipart_request_hpp */
