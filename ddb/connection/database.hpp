/*
 *
 * Copyright 2021 ddb authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __drift_db_database_hpp__
#define __drift_db_database_hpp__

#include "connection_pool.hpp"
#include "redis_connection.hpp"

#include "statement.hpp"

#include <vector>

namespace drift {
namespace db {

class DataBase {
public:
    DataBase() : _cache_connections(4, nullptr) {}
    ~DataBase() {}

    template<typename connection, uint32_t pool_size = 4>
    bool connect(std::string args) {
        auto conn = new ConnectionPool<connection>(pool_size);
        conn->connect(args);

        _cache_connections[0] = static_cast<void*>(conn);
//        _connections.emplace_back(static_cast<void*>(conn));
        return true;
    }

    bool raw_query(std::string&& sql_query_);

    template<typename Queryable>
    bool insert(Queryable queryable_) {
//        auto table_name = Queryable::table_name();
//        auto primary_key = Queryable::primary_key();

        auto redis = static_cast<ConnectionPool<RedisConnection>*>(_cache_connections[0]);
        auto connection = redis->get_connection();
        (static_cast<RedisConnection*>(connection))->insert_into(queryable_);

        return true;
    }

    template<typename connection, typename Queryable>
    bool insert(Queryable&& queryable_) {

        auto redis = static_cast<ConnectionPool<connection>*>(_cache_connections[0]);
        auto conn = redis->get_connection();
        (static_cast<connection*>(conn))->insert_into(queryable_);

        return true;
    }

    template<typename Queryable>
    bool remove(Queryable&& queryable_) {

        return true;
    }

    template<typename Queryable>
    bool update(Queryable&& queryable_) {

        return true;
    }

    template<typename Queryable>
    Queryable seletct() {
//        auto redis = static_cast<RedisConnection*>(get_redis());
//        auto condition = [](int id) { return id == 50 ? true : false; };
//        select<Queryable>().where(condition).join().execute(*redis);
        return Queryable{};
    }

    RedisConnection& get_redis() {
        auto redis = static_cast<ConnectionPool<RedisConnection>*>(_cache_connections[0]);
        return *reinterpret_cast<RedisConnection*>(redis->get_connection());
    }

    ConnectionIF* get_post() {
        auto redis = static_cast<ConnectionPool<RedisConnection>*>(_cache_connections[0]);
        return redis->get_connection();
    }

private:
    std::vector<void*> _cache_connections;
    std::vector<void*> _sql_connections;

};

} // namespace db
} // namespace drift

#endif // __drift_db_database_hpp__
