/*
 * http_client_android.cc
 *
 *  Created on: May 12, 2015
 *      Author: FrankLin
 */

#include "sakura/http_client.h"

NS_SKR_BEGIN

void HttpClient::HandleHttpResponseCallback(
    std::unique_ptr<HttpResponse> response,
    std::function<void(std::unique_ptr<HttpResponse>)> callback) {
  callback(std::move(response));
}

NS_SKR_END