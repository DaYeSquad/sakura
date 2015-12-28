#include "feed_message_manager.h"

namespace powertech {

// Websockets stack --------------------------------------------------------

void FeedMessageManager::Connect(const std::string& server) {
  websocket_->Init(*this, server);
}

}

