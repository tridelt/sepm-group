#include "Plugin.h"
#include <string>
#include <iostream>

using namespace std;


class HelloPlugin : public Plugin {
public:
  HelloPlugin() {
    cout << "Constructed HelloPlugin!" << endl;
  }
  string name() {
    return "Hello World Plugin";
  }

protected:
  ~HelloPlugin() {
    cout << "Destroyed HelloPlugin!" << endl;
  }
};

extern "C" Plugin* make_plugin() {
 return new HelloPlugin();
}
