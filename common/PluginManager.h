#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <string>
#include "Plugin.h"
#include "FileWatcher.h"

using namespace std;

class PluginManager {
public:
  PluginManager(string plugin_dir = "./plugins");
  ~PluginManager();
  void fileChanged(string name, bool isDir, FileWatcher::FileEvent e);

  void listPlugins();
private:
  void loadPlugin(string plugin_name);
  void removePlugin(string plugin_name);

  FileWatcher *watcher;
  QMap<string, Plugin*> plugins;
  QMap<string, void*> handles;
};

class PluginException : exception {
public:
  PluginException(string reason) : reason(reason) { }
  const char* what() const throw() {
    return reason.c_str();
  }
  ~PluginException() throw() { }
private:
  string reason;
};

#endif
