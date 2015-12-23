#include "network/http_client.h"

USING_NS_LCC;

void HttpClient::HandleHttpResponseCallback(std::unique_ptr<HttpResponse> response,
	std::function<void(std::unique_ptr<HttpResponse>)> callback) {
	callback(std::move(response));
}