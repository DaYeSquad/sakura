#include "sakura/websocket_thread_helper.h"

NS_SKR_BEGIN

////////////////////////////////////////////////////////////////////////////////
// WebsocketThreadHelper, public:

// Platform specific --------------------------------------------------------

void WebsocketThreadHelper::NotifyWebsocketMessage(std::unique_ptr<WebsocketMessage> ws_msg) {
  websocket_->OnWebsocketMessage(std::move(ws_msg));
}

NS_SKR_END