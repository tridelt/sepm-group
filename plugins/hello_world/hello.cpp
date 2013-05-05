#include "Plugin.h"
#include <string>
#include <iostream>
#include "Logging.h"

using namespace std;


class HelloPlugin : public Plugin {
public:
  HelloPlugin() {
    INFO("Constructed HelloPlugin!");
  }
  string name() {
    return "Hello World Plugin";
  }

protected:
  ~HelloPlugin() {
    INFO("Destroyed HelloPlugin!");
  }
};

extern "C" Plugin* make_plugin() {
 return new HelloPlugin();
}
