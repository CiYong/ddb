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

#ifndef __drift_db_define_hpp__
#define __drift_db_define_hpp__


#define DDB_ORM_ADD(struct_name, primary_key, ...) \
    namespace drift { \
    namespace db { \
    constexpr const char* talbe_##struct_name = #__VA_ARGS__; \
    struct Query { \
        std::string& convert(std::string& o, struct_name& v) const { \
            o.append(#primary_key); \
            std::vector<std::string> values; \
            collect(values, #__VA_ARGS__); \
            for (auto& value : values) { \
                o.append(value); \
            } \
            return o; \
        } \
    }; \
    } \
    }


#endif // __drift_db_define_hpp__
