#include "DBPool.h"

DBPool* DBPool::instance;
boost::mutex DBPool::mutex;

DBPool* DBPool::i() {
if(instance == NULL) {
    boost::lock_guard<boost::mutex> lock(mutex);
    if(instance == NULL)
      instance = new DBPool();
  }
  return instance;
}

DBPool::DBPool() : pool_size(10),
  pool(soci::connection_pool(pool_size)) {
  for(size_t i = 0; i < pool_size; ++i) {
    soci::session &sql = pool.at(i);
    sql.open("sqlite3://dbname=mydb");
  }
}

soci::connection_pool& DBPool::getPool() {
  return pool;
}
