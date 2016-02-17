#include "sakura/http_client.h"

USING_NS_SKR;

void HttpClient::HandleHttpResponseCallback(std::unique_ptr<HttpResponse> response,
	std::function<void(std::unique_ptr<HttpResponse>)> callback) {
	callback(std::move(response));
}