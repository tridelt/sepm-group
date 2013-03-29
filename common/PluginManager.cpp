#include "PluginManager.h"
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <dlfcn.h>

namespace fs = boost::filesystem;

PluginManager::PluginManager(string plugin_dir) {
  fs::path pluginDir(plugin_dir);

  if (!fs::exists(pluginDir) || !fs::is_directory(pluginDir))
    throw PluginException(plugin_dir + " is not a directory");

  fs::directory_iterator end_iter;
  for(fs::directory_iterator dir_iter(pluginDir); dir_iter != end_iter; ++dir_iter) {
    if(fs::is_regular_file(dir_iter->status()) ) {
      string filename = (*dir_iter).path().filename().string();
      loadPlugin(filename);
    }
  }

  watcher = new FileWatcher(plugin_dir,
                            boost::bind(&PluginManager::fileChanged,
                                        this, _1, _2, _3));

}

void PluginManager::fileChanged(string name, bool isDir, FileWatcher::FileEvent e) {
  if(!isDir) {
    if(e == FileWatcher::CREATE || e == FileWatcher::MODIFY)
      loadPlugin(name);
  }
}

PluginManager::~PluginManager() {
  delete watcher;

  for(auto plugin : plugins.values()) {
    plugin->destroy();
  }

  for(auto handle : handles.values()) {
    dlclose(handle);
  }
}

void PluginManager::loadPlugin(string plugin_name) {
  removePlugin(plugin_name);

  string path = "./plugins/" + plugin_name;
  void* handle = dlopen(path.c_str(), RTLD_LAZY);

  if(!handle)
    throw PluginException("failed to load plugin from " + path + ": " + dlerror());

  Plugin* (*make_plugin)();
  *(void **) (&make_plugin) = dlsym(handle, "make_plugin");

  plugins[plugin_name] = make_plugin();
  handles[plugin_name] = handle;
}

void PluginManager::removePlugin(string plugin_name) {
  if(plugins.contains(plugin_name)) {
    plugins[plugin_name]->destroy();
    plugins.remove(plugin_name);

    dlclose(handles[plugin_name]);
    handles.remove(plugin_name);
  }
}

void PluginManager::listPlugins() {
  cout << "=== currently loaded plugins" << endl;

  for(auto plugin : plugins.values()) {
    cout << plugin->name() << endl;
  }
}
