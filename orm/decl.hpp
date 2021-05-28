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

#ifndef __drift_db_decl_hpp__
#define __drift_db_decl_hpp__

#include <string>
#include <vector>
#include <type_traits>

namespace drift {
namespace db {

template<class ...Args>
void collect(Args ...args);

template<>
void collect(std::vector<std::string> value) {}

template<class T, class... Args>
void collect(std::vector<std::string>& value, T t, Args... args) {
    value.emplace_back(t);
    collect(value, args...);
}

template <typename T, typename Enabler = void>
struct Insertable;

template <typename T, typename Enabler = void>
struct Deletable;

template <typename T, typename Enabler = void>
struct Updatable;

template <typename T, typename Enabler = void>
struct Queryable;

} // namespace db
} // namespace drift


#endif // __drift_db_decl_hpp__
