//
//  http_client.cpp
//  SQLiteCpp
//
//  Created by Frank Lin on 11/24/14.
//  Copyright (c) 2014 Frank Lin. All rights reserved.
//
#include "http_client.h"

#if SKR_PLATFORM == SKR_PLATFORM_IOS || SKR_PLATFORM == SKR_PLATFORM_ANDROID
#include "curl.h"
#include "easy.h"
#elif SKR_PLATFORM == SKR_PLATFORM_MAC
#include <curl/curl.h>
#include <curl/easy.h>
#elif SKR_PLATFORM == SKR_PLATFORM_WIN32
#include "curl/curl.h"
#undef DELETE
//#pragma comment(lib, "..\..\external\curl\prebuilt\win32\libcurl.dll")
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#endif

#include "sakura/log.h"

using std::vector;
using std::unique_ptr;

typedef size_t (*write_callback)(void *ptr, size_t size, size_t nmemb, void *stream);

#define LCC_CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define LCC_CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define LCC_CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define LCC_CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ##__VA_ARGS__)

NS_SKR_BEGIN

static HttpClient* s_shared_client_ = nullptr;

////////////////////////////////////////////////////////////////////////////////
// HttpClient, C:

// Callback function used by libcurl for collect response data
static size_t on_http_write_stream(void *ptr, size_t size, size_t nmemb, void *stream)
{
  std::vector<char> *recv_buffer = (std::vector<char>*)stream;
  size_t sizes = size * nmemb;

  // add data to the end of recv_buffer
  // write data maybe called more than once in a single request
  recv_buffer->insert(recv_buffer->end(), (char*)ptr, (char*)ptr+sizes);

  return sizes;
}

// Callback function used by libcurl for collect header data
static size_t on_http_write_header(void *ptr, size_t size, size_t nmemb, void *stream)
{
  std::vector<char> *recv_buffer = (std::vector<char>*)stream;
  size_t sizes = size * nmemb;

  // add data to the end of recv_buffer
  // write data maybe called more than once in a single request
  recv_buffer->insert(recv_buffer->end(), (char*)ptr, (char*)ptr+sizes);

  return sizes;
}

size_t on_file_write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
  size_t written;
  written = fwrite(ptr, size, nmemb, stream);
  return written;
}

// Thanks to http://curl.haxx.se/libcurl/c/ftpupload.html
/* NOTE: if you want this example to work on Windows with libcurl as a
DLL, you MUST also provide a read callback with CURLOPT_READFUNCTION.
Failing to do so will give you a crash since a DLL may not use the
variable's memory when passed in to it from an app like this. */
//static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
//{
//	curl_off_t nread;
//	/* in real-world cases, this would probably get this data differently
//	as this fread() stuff is exactly what the library already would do
//	by default internally */
//	size_t retcode = fread(ptr, size, nmemb, static_cast<FILE*>(stream));
//
//	nread = (curl_off_t)retcode;
//
//	fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
//		" bytes from file\n", nread);
//	return retcode;
//}

////////////////////////////////////////////////////////////////////////////////
// HttpClient, CurlRaii:

//Simple wrapper of cURL using RAII
class Curl {
public:

  // Creation and lifetime --------------------------------------------------------

  Curl()
  : handler_(curl_easy_init()),
  headers_(nullptr) {

  }

  virtual ~Curl() {
    if (handler_) {
      curl_easy_cleanup(handler_);
    }
    if (headers_) {
      curl_slist_free_all(headers_);
    }
  }

  bool InitOrDie(const HttpRequest* request,
                 write_callback callback,
                 void *stream,
                 write_callback header_callback,
                 void *header_stream,
                 char *error_buffer) {
    if (!handler_) {
      return false;
    }
    if (!ConfigureCurlOrDie(handler_, error_buffer)) {
      return false;
    }

    //set headers
    std::map<std::string, std::string> request_header = request->request_header();
    if (!request_header.empty()) {
      for (auto it = request_header.begin(); it != request_header.end(); ++it) {
        std::string header_string = it->first + ":" + it->second;
        headers_ = curl_slist_append(headers_, header_string.c_str());

        if (!SetOptionOrDie(CURLOPT_HTTPHEADER, headers_)) {
          return false;
        }
      }
    }

    return SetOptionOrDie(CURLOPT_URL, request->url().c_str())
    && SetOptionOrDie(CURLOPT_WRITEFUNCTION, callback)
    && SetOptionOrDie(CURLOPT_WRITEDATA, stream)
    && SetOptionOrDie(CURLOPT_HEADERFUNCTION, header_callback)
    && SetOptionOrDie(CURLOPT_HEADERDATA, header_stream)
    && SetOptionOrDie(CURLOPT_SSL_VERIFYPEER, 0L);

    return true;
  }

  // Utils --------------------------------------------------------

  template<class T>
  bool SetOptionOrDie(CURLoption option, T data) {
    return CURLE_OK == curl_easy_setopt(handler_, option, data);
  }

	template<class T>
	bool GetInfoOrDie(CURLINFO option, T data) {
		return CURLE_OK == curl_easy_getinfo(handler_, option, data);
	}

  bool ConfigureCurlOrDie(CURL *handle, char *error_buffer)
  {
    if (!handle) {
      return false;
    }

    int32_t code;
    code = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, error_buffer);
    if (code != CURLE_OK) {
      return false;
    }
    code = curl_easy_setopt(handle, CURLOPT_TIMEOUT, 30);
    if (code != CURLE_OK) {
      return false;
    }
    code = curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 30);
    if (code != CURLE_OK) {
      return false;
    }
    //        if (s_sslCaFilename.empty()) {
    //            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
    //            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L);
    //        } else {
    //            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 1L);
    //            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 2L);
    //            curl_easy_setopt(handle, CURLOPT_CAINFO, s_sslCaFilename.c_str());
    //        }

    // FIXED #3224: The subthread of CCHttpClient interrupts main thread if timeout comes.
    // Document is here: http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTNOSIGNAL
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);

    return true;
  }

  bool PerformOrDie(int64_t *response_code) {
    if (CURLE_OK != curl_easy_perform(handler_)) {
      return false;
    }

    long curl_response_code;
    CURLcode code = curl_easy_getinfo(handler_, CURLINFO_RESPONSE_CODE, &curl_response_code);
    if (response_code) {
      *response_code = static_cast<int64_t>(curl_response_code);
    }

    if ((code != CURLE_OK) || (*response_code != 200)) {
      log_error("Curl curl_easy_getinfo failed: %s", curl_easy_strerror(code));
      return false;
    }

    return true;
  }

private:

  CURL* handler_;
  curl_slist* headers_;

  DISALLOW_COPY_AND_ASSIGN(Curl);
};

////////////////////////////////////////////////////////////////////////////////
// HTTPClient, public:

// Creation and lifetime --------------------------------------------------------

HttpClient::HttpClient() {

}

HttpClient::~HttpClient() {

}

HttpClient* HttpClient::SharedClient() {
  if (s_shared_client_ == nullptr) {
    s_shared_client_ = new HttpClient();
  }
  return s_shared_client_;
}

void HttpClient::PurgeSharedClient() {
  SKR_SAFE_DELETE(s_shared_client_);
}

// HTTP --------------------------------------------------------

void HttpClient::Send(std::unique_ptr<HttpRequest> request,
                      std::function<void(std::unique_ptr<HttpResponse>)> callback) {
#if SKR_PLATFORM == SKR_PLATFORM_WIN32
	// since win32 std::thread cannot copy request, use raw pointer and make it as unique_ptr in callback instead.
	auto func = std::bind(&HttpClient::ProcessHttpRequestWin32, this, std::placeholders::_1, std::placeholders::_2);

  std::string identifier_tag = request->IdentifierTag();
  request->set_tag(identifier_tag);
  
  std::lock_guard<std::mutex> lg(request_queue_mutex_);
  request_tag_queue_.push_back(identifier_tag);
  
	std::thread thread(func, request.release(), callback);
	thread.detach();
#else
  std::function<void(std::unique_ptr<HttpRequest>, std::function<void(std::unique_ptr<HttpResponse>)>)> func =
		std::bind(&HttpClient::ProcessHttpRequest, this, std::placeholders::_1, std::placeholders::_2);
  
  std::string identifier_tag = request->IdentifierTag();
  request->set_tag(identifier_tag);
  
  std::lock_guard<std::mutex> lg(request_queue_mutex_);
  request_tag_queue_.push_back(identifier_tag);
  
  std::thread thread(func, std::move(request), callback);
  thread.detach();
#endif
}

void HttpClient::CancelAllRequests() {
  std::lock_guard<std::mutex> lg(request_queue_mutex_);
  request_tag_queue_.clear();
}

////////////////////////////////////////////////////////////////////////////////
// HttpClient, private:

bool HttpClient::IsCancelledRequest(const std::string& tag) const {
  for (auto it = request_tag_queue_.begin(); it != request_tag_queue_.end(); ++it) {
    if (tag == *it) {
      return false;
    }
  }
  return true;
}

void HttpClient::ProcessHttpRequestWin32(HttpRequest* request,
                                         std::function<void(std::unique_ptr<HttpResponse>)> callback) {
	ProcessHttpRequest(std::unique_ptr<HttpRequest>(request), callback);
}

void HttpClient::ProcessHttpRequest(std::unique_ptr<HttpRequest> request,
                                    std::function<void(std::unique_ptr<HttpResponse>)> callback) {
	//sakura::log_event("Request URL is %s, method %d", request->url().c_str(), request->request_type());

  // request has already been cancelled
  if (IsCancelledRequest(request->tag())) {
    return;
  }
  
  std::vector<char> response_data;
  std::vector<char> response_header;
  int64_t response_code = -1;
  char error_buffer[256];
  bool success = false;
  
  // supports raw data
  if (!request->request_data().empty()) {
    std::string request_data = request->request_data();
    for (size_t i = 0; i < request_data.size(); ++i) {
      request->raw_data_.push_back(request_data[i]);
    }
  }
  
  //set HTTP method and HTTP body
  //CURLOPT_FOLLOWLOCATION: tells the library to follow any Location: header that the server sends as part of a HTTP header in a 3xx response.
  if (request->request_type() == HttpRequest::Type::GET) {
    Curl curl;
    success = curl.InitOrDie(request.get(), on_http_write_stream, &response_data, on_http_write_header, &response_header, error_buffer)
    && curl.SetOptionOrDie(CURLOPT_FOLLOWLOCATION, true)
    && curl.PerformOrDie(&response_code);
  } else if (request->request_type() == HttpRequest::Type::POST) {
    Curl curl;
		success = curl.InitOrDie(request.get(), on_http_write_stream, &response_data, on_http_write_header, &response_header, error_buffer)
			&& curl.SetOptionOrDie(CURLOPT_POST, 1)
			&& curl.SetOptionOrDie(CURLOPT_POSTFIELDS, &request->raw_data_[0])
			&& curl.SetOptionOrDie(CURLOPT_POSTFIELDSIZE, request->raw_data_.size())
			&& curl.PerformOrDie(&response_code);
  } else if (request->request_type() == HttpRequest::Type::PUT) {
    Curl curl;
    success = curl.InitOrDie(request.get(), on_http_write_stream, &response_data, on_http_write_header, &response_header, error_buffer)
    && curl.SetOptionOrDie(CURLOPT_CUSTOMREQUEST, "PUT")
    && curl.SetOptionOrDie(CURLOPT_POSTFIELDS, &request->raw_data_[0])
    && curl.SetOptionOrDie(CURLOPT_POSTFIELDSIZE, request->raw_data_.size())
    && curl.PerformOrDie(&response_code);
  } else if (request->request_type() == HttpRequest::Type::DELETE) {
    Curl curl;
    success = curl.InitOrDie(request.get(), on_http_write_stream, &response_data, on_http_write_header, &response_header, error_buffer)
    && curl.SetOptionOrDie(CURLOPT_CUSTOMREQUEST, "DELETE")
    && curl.SetOptionOrDie(CURLOPT_FOLLOWLOCATION, true)
    && curl.PerformOrDie(&response_code);
  } else {
    //log_error("HttpClient: Unsupported HTTP method");
  }

  // request has already been cancelled
  if (IsCancelledRequest(request->tag())) {
    log_event("HttpClient: request %s has been cancelled", request->url().c_str());
    return;
  }
  
  //setup response
  std::unique_ptr<HttpResponse> http_response(new HttpResponse(std::move(request)));
  http_response->set_response_code(response_code);
  http_response->set_error_buffer(error_buffer);
  http_response->set_succeed(success);

  if (success) {
    std::string response_header_string(response_header.begin(), response_header.end());
    http_response->set_response_data(response_data);

#ifdef SKR_HTTP_TRACE_HEADER
    log_event("HTTP response http header is : %s", response_header_string.c_str());
#endif

#ifdef SKR_HTTP_TRACE_RESPONSE
    log_event("HTTP response body is : %s", response_data_string.c_str());
#endif

  }
#ifdef SKR_HTTP_TRACE_HEADER
  else {
    log_event("HTTP response error is : %s", error_buffer);
  }
#endif

  HandleHttpResponseCallback(std::move(http_response), callback);
}

NS_SKR_END
