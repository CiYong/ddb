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

#ifndef __drift_db_metadata_hpp__
#define __drift_db_metadata_hpp__

#include <string>
#include <vector>

namespace drift {
namespace db {

class TableMeta {
public:
    std::vector<std::string> members() { return _members; }
    std::string table_name() { return _name; }

    size_t size() { return _members.size(); }

    std::string& operator[](int index) { return _members[index]; }
    std::string operator[](int index) const { return _members[index]; }

private:
    std::string _name;
    std::vector<std::string> _members;
};


} // namespace db
} // namespace drift

#endif // __drift_db_metadata_hpp__
