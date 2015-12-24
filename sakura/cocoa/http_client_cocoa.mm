//
//  http_client.cpp
//  lesschatcore-ios
//
//  Created by Frank Lin on 5/12/15.
//  Copyright (c) 2015 Frank Lin. All rights reserved.
//

#include "sakura/http_client.h"

#import <Foundation/Foundation.h>

USING_NS_SKR;

void HttpClient::HandleHttpResponseCallback(std::unique_ptr<HttpResponse> response,
                                            std::function<void(std::unique_ptr<HttpResponse>)> callback) {
  
  __block std::unique_ptr<HttpResponse> block_response = std::move(response);
  
  dispatch_async(dispatch_get_main_queue(), ^{
    callback(std::move(block_response));
  });
}