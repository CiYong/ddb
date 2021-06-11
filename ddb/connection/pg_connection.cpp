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


#include "pg_connection.hpp"


namespace drift {
namespace db {

bool PGConnection::connect(std::string arg_) {
    auto addr_pos = arg_.find("/");
    std::string address(arg_.data(), addr_pos);
    auto ip_pos = address.find(":");

    auto port = atoi(address.data() + ip_pos + 1);

    auto name = arg_.find("/");

    auto passwd = arg_.find("/");

    auto database = arg_.find("/");




    return true;
}

void PGConnection::disconnect()  {

}

bool PGConnection::raw_query(std::string sql_text_) {

    return true;
}

} // namespace db
} // namespace drift
