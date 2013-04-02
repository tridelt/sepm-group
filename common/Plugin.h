#ifndef PLUGIN_H
#define PLUGIN_H

#include <string>

using namespace std;

/** \brief Interface that has to be implemented by all Plugins.
 *
 * All plugins need to implement this interface in order to correctly work with
 * this project. Some methods might be optional, those can be left out if they
 * are not needed.
 */
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

extern "C" Plugin* make_plugin();

#endif
