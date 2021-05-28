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

#ifndef __drift_db_serializer_hpp__
#define __drift_db_serializer_hpp__

#include "decl.hpp"

namespace drift {
namespace db {

template<typename Stream>
struct serializer {
    serializer(Stream& s);

    template <typename T>
    serializer<Stream>& pack(const T& v);

    serializer<Stream>& pack_array(uint32_t n);

    serializer<Stream>& pack_int(int d);

private:
    template <typename T>
    void pack_imp_int32(T d);

private:
    Stream& m_stream;
};

template <typename Stream>
inline serializer<Stream>::serializer(Stream& s) : m_stream(s) { }

template <typename Stream>
template <typename T>
inline serializer<Stream>& serializer<Stream>::pack(const T& v)
{
    operator<<(*this, v);
    return *this;
}

//template <typename T, typename Enabler>
//struct convert {
//    object const& operator()(object const& o, T& v) const;
//};

template <typename T, typename Enabler>
struct Insertable {
    template <typename Stream>
    serializer<Stream>& operator()(serializer<Stream>& o, T const& v) const;
};

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
//    serializer<Stream>&
//>::type
//operator<< (serializer<Stream>& o, T const& v) {
//    return adaptor::pack<T>()(o, v);
//}
//template <typename Stream, typename T, std::size_t N>
//inline
//serializer<Stream>& operator<< (serializer<Stream>& o, const T(&v)[N]) {
//    return adaptor::pack<T[N]>()(o, v);
//}

template <typename Stream>
inline serializer<Stream>& serializer<Stream>::pack_array(uint32_t n)
{
    return *this;
}

template <typename Stream>
template <typename T>
inline void serializer<Stream>::pack_imp_int32(T d)
{
    m_stream << d;
}

template <typename Stream>
inline serializer<Stream>& serializer<Stream>::pack_int(int d)
{
    pack_imp_int32(d);
    return *this;
}


} // namespace db
} // namespace drift

#endif // __drift_db_serializer_hpp__
