SET(chat_HEADERS FileWatcher.h ViewRefresher.h)
QT4_WRAP_CPP(chat_HEADERS_MOC ${chat_HEADERS})

add_library (common FileWatcher.cpp NotifyPipe.cpp ViewRefresher.cpp ${chat_HEADERS_MOC})
