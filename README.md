#Sakura

Sakura is a solution for building iOS/Android/Win32/OS X app's logic part, it is the base of Worktile and Worktile Pro (worktile.com).

Every company wants to have a cross platform developement solution to reduce its cost of building a product on iOS, Android, Windows and OS X. Sakura aims to make the main logic code portable and reliable.

This git itself just provides HTTP request and log ability, for model building, you can refer to [Model builder](https://github.com/DaYeSquad/cpp-core-model-builder) (we will provide more examples soon).


#Example

```cpp
std::unique_ptr<sakura::HttpRequest> request(new sakura::HttpRequest());
request->AddValueToHttpHeaderField("app-key", app_key());
request->AddValueToHttpHeaderField("version", configuration_->api_version());

request->set_request_type(sakura::HttpRequest::Type::GET);

HttpClient::SharedClient()->Send(std::move(request), [](unique_ptr<HttpResponse> response) {
  if (response->is_succeed()) {
    ParseJson(response->response_data_as_string()); // we are using json11 as json parser.
    callback();
  } else {
    ErrorCallback();
  }
}
```


#Vendors

- curl : HTTP request (prebuilt)

- gtest : Unit test (prebuilt)

- json11: JSON parser

- openssl : SSL (prebuilt)

- websockets : websockets and Socket.IO (prebuilt)

- tinyxml2 : XML parser

- easySQLite : fork from easySQLite with lots of additions


#Available options (Preprocessor flags):

SKR_PLATFORM : Can be SKR_PLATFORM_MAC or SKR_PLATFORM_IOS or SKR_PLATFORM_ANDROID

SKR_LOG_LEVEL : Can be SKR_LOG_LEVEL_ALL or SKR_LOG_LEVEL_WARNING or SKR_LOG_LEVEL_ERROR

SKR_USING_ASSERT : assert() will be excuted

SKR_LOG_WEBSOCKET : Log websockets events

SKR_LOG_SIO : Log socket.io events


#Licence
MIT
