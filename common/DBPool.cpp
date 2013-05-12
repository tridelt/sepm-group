#include "DBPool.h"


DBPool* DBPool::TestPool() {
  return new DBPool(":memory:", 1);
}

DBPool* DBPool::ProdPool() {
  return new DBPool("mydb", 10);
}

DBPool::DBPool(string db_name, int size) : pool_size(size),
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
