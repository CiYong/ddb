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

#ifndef __drift_db_connection_if_hpp__
#define __drift_db_connection_if_hpp__

#include <string>

namespace drift {
namespace db {

class ConnectionIF {
public:
    virtual bool connect(std::string arg_) = 0;
    virtual void disconnect() = 0;
};

} // namespace db
} // namespace drift

#endif // __drift_db_connection_if_hpp__
