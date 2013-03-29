#include "plugin.h"
#include <string>

using namespace std;

class HelloPlugin : public Plugin {
public:
  string name() {
    return "Hello World Plugin";
  }
};

extern "C" Plugin* make_plugin() {
 return new HelloPlugin();
}

extern "C" void destroy_plugin(Plugin* p) {
  delete p;
}
