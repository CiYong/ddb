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

#ifndef __drift_db_pg_connection_hpp__
#define __drift_db_pg_connection_hpp__

#include "connection_if.hpp"
#include <iostream>

namespace drift {
namespace db {

class PGConnection : public ConnectionIF {
public:
    virtual bool connect(std::string arg_) override;
    virtual void disconnect() override;

    virtual bool raw_query(std::string sql_text_);
};

} // namespace db
} // namespace drift

#endif // __drift_db_pg_connection_hpp__
