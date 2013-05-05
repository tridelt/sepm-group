add_library (common SHARED FileWatcher.cpp NotifyPipe.cpp ViewRefresher.cpp ExitHandler.cpp PluginManager.cpp sdcHelper.cpp Security.cpp DBPool.cpp Logging.cpp ${chat_HEADERS_MOC})

add_dependencies(common ice)
add_dependencies(common soci-3-2-1)

target_link_libraries(common soci_core)
target_link_libraries(common qca)
target_link_libraries(common ice)
target_link_libraries(common soci_sqlite3)
