#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <cstdlib>

using namespace std;

const string DB_USER = "grp_4";
const string DB_PASS = "18fef7c7-f606-4d19-87a8-d6697d28efd7";

class Config {
public:
  static string hostname() {
    if(_hostname == "") {
      auto host_env = getenv("SEPM_HOSTNAME");
      if(host_env)
        _hostname = host_env;
      else // getenv returned NULL - variable not set
        _hostname = "localhost";
    }
    return _hostname;
  }
  static void hostname(string h) {
    _hostname = h;
  }

private:
  static string _hostname;
};

#endif
