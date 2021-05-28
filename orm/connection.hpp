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

#ifndef __drift_db_connection_hpp__
#define __drift_db_connection_hpp__

#include "orm.hpp"

#include <iostream>

namespace drift {
namespace db {

class Connection {
public:
    Connection() {}
    ~Connection() {}

    bool connect(std::string addr_) { return true; }
    void disconnet() {}

    bool execute(std::string&& raw_sql_content_) {
        return true;
    }

    template<typename T>
    T load() {
        auto table_name = T::table();
        return T{};
    }

    bool transaction() {

        return true;
    }

    void commit() {
    }

    void rollback() {
    }

    template<typename T>
    bool insert_into(T&& arg) {
        auto sql_content = to_raw_query(std::move(arg));
        std::cout << "[Connection]:" << sql_content.str() << std::endl;
        return true;
    }

    bool delete_row(std::string&& sql_content_) {
        log(sql_content_);
        return true;
    }

    bool update(std::string&& sql_content_) {
        log(sql_content_);
        return true;
    }

    bool select(std::string&& sql_content_) {
        log(sql_content_);
        return true;
    }

    void log(std::stringstream content_) {
        std::cout << "[Connection]:" << content_.str() << std::endl;
    }

    void log(std::string content_) {
        std::cout << "[Connection]:" << content_ << std::endl;
    }

private:

};

} // namespace db
} // namespace drift

#endif // __drift_db_connection_hpp__
