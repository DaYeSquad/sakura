//
//  http_response.cpp
//  lesschatcore-ios
//
//  Created by darui on 15/9/12.
//  Copyright (c) 2015年 Frank Lin. All rights reserved.
//

#include "http_response.h"

#include "http_request.h"

NS_SKR_BEGIN

////////////////////////////////////////////////////////////////////////////////
// HttpResponse, public:

HttpResponse::HttpResponse(std::unique_ptr<HttpRequest> request)
:http_request_(std::move(request)),
succeed_(false) {}

// Getter --------------------------------------------------------

const HttpRequest* HttpResponse::http_request() const { return http_request_.get(); }

std::string HttpResponse::response_data_as_string() const {
  std::string response_data_string(response_data_.begin(), response_data_.end());
  return response_data_string;
}

NS_SKR_END
