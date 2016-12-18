//
// Created by Moki on 2016/12/18.
//

#ifndef SAKURA_HTTP_PROXY_H_
#define SAKURA_HTTP_PROXY_H_

#include <mutex>

#include "sakura/macros.h"
#include "sakura/http_request.h"
#include "sakura/http_response.h"

NS_SKR_BEGIN
  class SKR_DLL HttpProxyClient {
  public:
    // win32 wrapper of ProcessHttpRequest
    virtual void ProcessHttpRequestWin32(HttpRequest* request,
                                         std::function<void(std::unique_ptr<HttpResponse>)> callback) = 0;

    virtual void ProcessHttpRequest(std::unique_ptr<HttpRequest> request,
                                    std::function<void(std::unique_ptr<HttpResponse>)> callback) = 0;

    // platform spec
    virtual void HandleHttpResponseCallback(std::unique_ptr<HttpResponse> response,
                                    std::function<void(std::unique_ptr<HttpResponse>)> callback) {
      callback(std::move(response));
    };

  protected:
    std::vector<std::string> request_tag_queue_;
    std::mutex request_queue_mutex_;
  };
NS_SKR_END

#endif //SAKURA_HTTP_PROXY_H_
