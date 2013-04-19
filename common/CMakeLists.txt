add_library (common FileWatcher.cpp NotifyPipe.cpp ViewRefresher.cpp ExitHandler.cpp PluginManager.cpp sdcHelper.cpp Security.cpp ${chat_HEADERS_MOC})

add_dependencies(common ice)
