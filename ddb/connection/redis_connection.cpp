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


#include "redis_connection.hpp"

#include "hiredis/hiredis.h"

namespace drift {
namespace db {

bool RedisConnection::connect(std::string arg) {
    auto pos = arg.find(":");
    std::string addr(arg.data(), pos);
    int port = atoi(arg.data() + pos + 1);

    _ctx = redisConnect(addr.c_str(), port);
    if (_ctx->err != 0) {
        std::cout << "[RedisConnection] ctx->err=" << _ctx->err << std::endl;
        return false;
    }

    return true;
}

void RedisConnection::disconnect() {
    std::cout << "[RedisConnection] disconnect=" << _ctx->err << std::endl;
    if (_ctx) {
        redisFree(_ctx);
        _ctx = nullptr;
    }
}

} // namespace db
} // namespace drift
