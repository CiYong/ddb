/*
 *
 * Copyright 2021 dchat authors.
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

#ifndef __drift_db_connection_pool_hpp__
#define __drift_db_connection_pool_hpp__

#include "connection_if.hpp"

#include <string>
#include <atomic>
#include <vector>

namespace drift {
namespace db {

template<typename connection>
class ConnectionPool {
public:
    ConnectionPool(uint32_t size_)
        : _pools(size_, nullptr),
          _free(0) {

        for (uint32_t i = 0; i < size_; i++) {
            auto conn = new connection;
            _pools.push_back(conn);
            _free.fetch_add(1);
        }
    }

    ~ConnectionPool() {
        disconnect();
    }

    bool connect(std::string addr_) {
        for (auto iter = _pools.begin(); iter != _pools.end(); iter++) {
            if (*iter) {
                (*iter)->connect(addr_);
            }
        }

        return true;
    }

    void disconnect() {
        for (auto iter = _pools.begin(); iter != _pools.end(); iter++) {
            if (*iter) {
                (*iter)->disconnect();
            }
        }
    }

    ConnectionIF* get_connection() {
        return _pools[_free.fetch_sub(1)];
    }

private:
    std::vector<ConnectionIF*> _pools;
    std::atomic_uint _free;
};

} // namespace db
} // namespace drift

#endif // __drift_db_connection_pool_hpp__
