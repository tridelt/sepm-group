#ifndef DB_POOL_H
#define DB_POOL_H

#include "soci/soci.h"
#include <boost/thread.hpp>

class DBPool {
public:
  static DBPool* i();

  soci::connection_pool& getPool();
private:
  static DBPool* instance;
  static boost::mutex mutex;

  DBPool();
  size_t pool_size;
  soci::connection_pool pool;
};

#endif
