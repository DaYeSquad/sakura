//
//  http_client.h
//  LessChatCore
//
//  Created by Frank Lin on 11/24/14.
//  Copyright (c) 2014 Frank Lin. All rights reserved.
//

#ifndef SAKURA_HTTP_CLIENT_H_
#define SAKURA_HTTP_CLIENT_H_

#include <thread>
#include <vector>
#include <memory>
#include <map>
#include <string>
#include <mutex>

#include "sakura/macros.h"

#include "sakura/http_request.h"
#include "sakura/http_response.h"

NS_SKR_BEGIN

class SKR_DLL HttpClient {
public:
  
  // Creation and lifetime --------------------------------------------------------
  
  HttpClient();
  virtual ~HttpClient();
  
  static HttpClient* SharedClient();
  static void PurgeSharedClient();
  
  // HTTP --------------------------------------------------------
  
  void Send(std::unique_ptr<HttpRequest> request,
            std::function<void(std::unique_ptr<HttpResponse>)> callback);
  
  void CancelAllRequests();
  
private:
  std::vector<std::string> request_tag_queue_;
  std::mutex request_queue_mutex_;
  
  void ProcessHttpRequest(std::unique_ptr<HttpRequest> request,
                          std::function<void(std::unique_ptr<HttpResponse>)> callback);

	// win32 wrapper of ProcessHttpRequest
	void ProcessHttpRequestWin32(HttpRequest* request, 
		std::function<void(std::unique_ptr<HttpResponse>)> callback);

  // platform spec
  void HandleHttpResponseCallback(std::unique_ptr<HttpResponse> response,
                                  std::function<void(std::unique_ptr<HttpResponse>)> callback);
  
  bool IsCancelledRequest(const std::string& tag) const;
  
  // Returns false if not found such tag
  bool RemoveRequestTag(const std::string& tag);
  
  
  DISALLOW_COPY_AND_ASSIGN(HttpClient);
};

NS_SKR_END

#endif /* defined(SAKURA_HTTP_CLIENT_H_) */
