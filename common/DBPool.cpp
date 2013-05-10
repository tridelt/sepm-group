#include "DBPool.h"


DBPool::DBPool(string db_name) : pool_size(10),
  pool(soci::connection_pool(pool_size)) {
  for(size_t i = 0; i < pool_size; ++i) {
    soci::session &sql = pool.at(i);
    // TODO: open different db on production server
    sql.open("sqlite3://dbname=" + db_name);
  }
}

soci::connection_pool& DBPool::getPool() {
  return pool;
}
