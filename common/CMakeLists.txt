add_library (common FileWatcher.cpp NotifyPipe.cpp ViewRefresher.cpp ExitHandler.cpp PluginManager.cpp sdcHelper.cpp Security.cpp DBPool.cpp ${chat_HEADERS_MOC})

add_dependencies(common ice)

target_link_libraries(common soci_core)
target_link_libraries(common soci_sqlite3)
