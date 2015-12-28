#include "sakura/sakura.h"

namespace powertech {

class FeedMessageManager: sakura::Websocket::Delegate {
public:
  // Creation and lifetime --------------------------------------------------------

  FeedMessageManager() {
    websocket_ = new sakura::Websocket();
  }

  virtual ~FeedMessageManager() { }

  // Websockets stack --------------------------------------------------------

  void Connect(const std::string& server);

  // Websocket Delegate Override --------------------------------------------------------

  virtual void OnOpen(const sakura::Websocket *ws) {
    sakura::log_error("FeedMessageManager::OnOpen");
  }

  virtual void OnMessage(const sakura::Websocket *ws, const sakura::Websocket::Data& data) {
    sakura::log_error("FeedMessageManager::OnMessage %s", data.bytes);
  }

  virtual void OnClose(const sakura::Websocket *ws) {
    sakura::log_error("FeedMessageManager::OnClose");
  }

  virtual void OnError(const sakura::Websocket* ws, const sakura::Websocket::ErrorCode& error) {
    sakura::log_error("FeedMessageManager::OnError %d", static_cast<int>(error));
  }

private:
  sakura::Websocket* websocket_;
};

}


