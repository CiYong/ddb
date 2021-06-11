/*
 *
 * Copyright 2021 drift server authors.
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


#ifndef __drift_server_config_parser_hpp__
#define __drift_server_config_parser_hpp__

#include <iostream>
#include <string>
#include <fstream>
#include <array>

#include <cstring>
#include <cstdlib>

#define STRING_HELPER(str) \
    static constexpr const char* str = #str;

STRING_HELPER(server_addr)
STRING_HELPER(server_port)
STRING_HELPER(redis_addr)
STRING_HELPER(redis_port)
STRING_HELPER(pg_addr)
STRING_HELPER(pg_port)
STRING_HELPER(pg_database)
STRING_HELPER(pg_user)
STRING_HELPER(pg_passwd)

namespace drift {

struct RedisConfig {
    std::string addr;
    uint32_t port;
    std::string user;
    std::string passwd;
};

struct PGConfig {
    std::string addr;
    uint32_t port;
    std::string user;
    std::string passwd;
};

struct ServerConfig {
    std::string addr;
    uint32_t port;
};

struct BackendServerConfig {
    ServerConfig self_config;
    RedisConfig redis_config;
    PGConfig pg_config;
};

bool config_get(const char* path_file_) {
    const unsigned int line_size = 128;
    std::string tmp;

    std::ifstream file;
    file.open(path_file_, std::ios::in);

    for (std::array<char, line_size> line; file.getline(&line[0], line_size); ) {
        tmp.clear();
        tmp = line.data();
        std::cout << tmp << std::endl;

//        if (tmp.find(key) == 0) {
//            config.leak_timeout = atol(tmp.data() + strlen(key));
//        }

//        if (tmp.find(HISTORY_LIMIT_SECTION) == 0) {
//            config.history_limit = atol(tmp.data() + strlen(HISTORY_LIMIT_SECTION));
//        }
    }

    file.close();

    return true;
}



BackendServerConfig parse_config(const char* path_file_) {
    ServerConfig server_config;
    RedisConfig redis_config;
    PGConfig pg_config;

    auto result = config_get(path_file_);

    return {server_config, redis_config, pg_config};
}

} // namespace drift


#endif // __drift_server_config_parser_hpp__
