/*
 * http_client_android.h
 *
 *  Created on: May 12, 2015
 *      Author: FrankLin
 */

#ifndef LESSCHATCORE_NETWORK_ANDROID_HTTP_CLIENT_ANDROID_H_
#define LESSCHATCORE_NETWORK_ANDROID_HTTP_CLIENT_ANDROID_H_

#include "sakura/http_response.h"

#include "base/base.h"
#include "sakura/log.h"

NS_SKR_BEGIN

class HttpResponseCallback {
 public:
  HttpResponseCallback(
      std::unique_ptr<HttpResponse> response,
      std::function<void(std::unique_ptr<HttpResponse>)> callback) {
    response_ = std::move(response);
    callback_ = callback;
  }

  virtual ~HttpResponseCallback() {

  }

  void execute() {
    callback_(std::move(response_));
  }

 private:
  std::unique_ptr<HttpResponse> response_;
  std::function<void(std::unique_ptr<HttpResponse>)> callback_;


  DISALLOW_COPY_AND_ASSIGN(HttpResponseCallback);
};

NS_SKR_END

#endif /* LESSCHATCORE_NETWORK_ANDROID_HTTP_CLIENT_ANDROID_H_ */
