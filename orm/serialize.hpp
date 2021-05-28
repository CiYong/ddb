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

#ifndef __drift_db_serialize_hpp__
#define __drift_db_serialize_hpp__

#include "decl.hpp"
#include "serializer.hpp"

namespace drift {
namespace db {


//template <typename T, typename Enabler>
//struct convert {
//    object const& operator()(object const& o, T& v) const;
//};


//template <typename T, typename Enabler>
//struct object {
//    void operator()(object& o, T const& v) const;
//};

//template <typename T, typename Enabler>
//struct object_with_zone {
//    void operator()(object::with_zone& o, T const& v) const;
//};


//template <typename Stream, typename T>
//inline
//typename std::enable_if<
//    !std::is_array<T>::value,
//    packer<Stream>&
//>::type
//operator<< (packer<Stream>& o, T const& v) {
//    return adaptor::pack<T>()(o, v);
//}
//template <typename Stream, typename T, std::size_t N>
//inline
//packer<Stream>& operator<< (packer<Stream>& o, const T(&v)[N]) {
//    return adaptor::pack<T[N]>()(o, v);
//}



template <typename Stream, typename T>
inline
typename std::enable_if<
    !std::is_array<T>::value,
    serializer<Stream>&
>::type
operator<< (serializer<Stream>& o, T const& v) {
    return Insertable<T>()(o, v);
}

template <>
struct Insertable<signed int> {
    template <typename Stream>
    serializer<Stream>& operator()(serializer<Stream>& o, signed int v) const
    { o.pack_int(v); return o; }
};

template <typename Stream, typename T>
inline void serialize(Stream& s, const T& v)
{
    serializer<Stream>(s).pack(v);
}


} // namespace db
} // namespace drift

#endif // __drift_db_serialize_hpp__

