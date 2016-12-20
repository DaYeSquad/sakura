//
// Created by Moki on 2016/12/18.
//

#ifndef SAKURA_CURL_PROXY_H_
#define SAKURA_CURL_PROXY_H_

#include <mutex>
#include "macros.h"
#include "sakura/http_proxy_client.h"

NS_SKR_BEGIN
  class SKR_DLL CurlProxyClient: public HttpProxyClient {

  public:
    void CancelAllRequests();

    virtual void ProcessHttpRequestWin32(HttpRequest *request,
                                         std::function<void(std::unique_ptr<HttpResponse>)> callback) OVERRIDE;

    virtual void ProcessHttpRequest(std::unique_ptr<HttpRequest> request,
                                    std::function<void(std::unique_ptr<HttpResponse>)> callback) OVERRIDE;

  private:

    bool IsCancelledRequest(const std::string& tag) const;

    // Returns false if not found such tag
    bool RemoveRequestTag(const std::string& tag);
  };
NS_SKR_END

#endif //SAKURA_CURL_PROXY_H_
