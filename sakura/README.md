# Sakura - C++ 11 development kit for YCTech app developers
# Written in C++ 11 and support iOS, Android, OS X, Win32
# Features included:

- File I/O   : file_utils
- Log        : log
- Network    : http_client
- Websockets : websocket
- SocketIO   : socket_io

@author Frank Lin (lin.xiaoe.f@gmail.com)

*Getting started:

1. Set some required pre-defined macros:

- Platform                  : SKR_PLATFORM (eg: SKR_PLATFORM=SKR_PLATFORM_IOS)
- What to log               : SKR_LOG_LEVEL (eg: SKR_LOG_LEVEL_ALL)
- Using assert during debug : SKR_USING_ASSERT
- Log HTTP response         : SKR_HTTP_TRACE_HEADER, SKR_HTTP_TRACE_RESPONSE, SKR_HTTP_TRACE_ERROR

2. Add required library:

- libcurl
- libz
- libssl
- libcryptor
- libwebsockets


