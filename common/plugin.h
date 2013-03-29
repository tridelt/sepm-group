#ifndef PLUGIN_H
#define PLUGIN_H

#include <string>

using namespace std;

class Plugin {
public:
  virtual string name() = 0;
  virtual ~Plugin();
};

#endif
