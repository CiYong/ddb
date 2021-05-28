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

#ifndef __drift_db_command_hpp__
#define __drift_db_command_hpp__

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <type_traits>

namespace drift {
namespace db {
namespace sql_command {

constexpr const char* insert_into   = "INSERT INTO";
constexpr const char* delete_str    = "DELETE";
constexpr const char* update        = "UPDATE";
constexpr const char* select        = "SELECT";

constexpr const char* values        = "VALUES";
constexpr const char* insert_update = "ON DUPLICATE KEY UPDATE";

} // namespace sql_command
} // namespace db
} // namespace drift

#endif // __drift_db_command_hpp__
