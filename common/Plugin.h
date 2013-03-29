#ifndef PLUGIN_H
#define PLUGIN_H

#include <string>

using namespace std;

class Plugin {
public:
  virtual string name() = 0;
  /* since the plugin can be linked against a different std lib, we need to make
     sure it uses the correct delete to destroy itself.
     This is the only way to delete a Plugin, never try to delete it manually! */
  void destroy() { delete this; }

protected:
  virtual ~Plugin() { }
};

#endif
