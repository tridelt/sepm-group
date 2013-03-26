#include <functional>
using namespace std;

class ExitHandler {
public:
  static ExitHandler* i();
  void setHandler(function<void(int)> h);
  void handle(int s);
private:
  ExitHandler() { }
  static ExitHandler* inst;
  function<void(int)> handler;
};
