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
#include "sakura/http_proxy_client.h"
#include "sakura/curl_proxy_client.h"

NS_SKR_BEGIN

template <typename ProxyClient = CurlProxyClient>
class SKR_DLL HttpClient : public ProxyClient {

	static HttpClient<ProxyClient>* s_shared_client_;

public:
  
  // Creation and lifetime --------------------------------------------------------

  HttpClient(){};
  virtual ~HttpClient(){};

	static HttpClient<ProxyClient>* SharedClient() {
		if (s_shared_client_ == nullptr) {
			s_shared_client_ = new HttpClient<ProxyClient>();
		}
		return s_shared_client_;
	};

  static void PurgeSharedClient() {
		SKR_SAFE_DELETE(s_shared_client_);
	};
  
  // HTTP --------------------------------------------------------
  
	void Send(std::unique_ptr<HttpRequest> request,
            std::function<void(std::unique_ptr<HttpResponse>)> callback) {
#if SKR_PLATFORM == SKR_PLATFORM_WIN32
		// since win32 std::thread cannot copy request, use raw pointer and make it as unique_ptr in callback instead.
	auto func = std::bind(&HttpProxyClient::ProcessHttpRequestWin32, this, std::placeholders::_1, std::placeholders::_2);

  std::string identifier_tag = request->IdentifierTag();
  request->set_tag(identifier_tag);

  std::lock_guard<std::mutex> lg(HttpProxyClient::request_queue_mutex_);
	HttpProxyClient::request_tag_queue_.push_back(identifier_tag);

	std::thread thread(func, request.release(), callback);
	thread.detach();
#else
		std::function<void(std::unique_ptr<HttpRequest>, std::function<void(std::unique_ptr<HttpResponse>)>)> func =
			std::bind(&ProxyClient::ProcessHttpRequest, this, std::placeholders::_1, std::placeholders::_2);

		std::string identifier_tag = request->IdentifierTag();
		request->set_tag(identifier_tag);

		std::lock_guard<std::mutex> lg(ProxyClient::request_queue_mutex_);
		ProxyClient::request_tag_queue_.push_back(identifier_tag);

		std::thread thread(func, std::move(request), callback);
		thread.detach();
#endif
	};

	void CancelAllRequests() {
		std::lock_guard<std::mutex> lg(ProxyClient::request_queue_mutex_);
		ProxyClient::request_tag_queue_.clear();
	}
  
  DISALLOW_COPY_AND_ASSIGN(HttpClient);
};

	template <typename ProxyClient>
	HttpClient<ProxyClient>* HttpClient<ProxyClient>::s_shared_client_ = nullptr;

NS_SKR_END

#endif /* defined(SAKURA_HTTP_CLIENT_H_) */
