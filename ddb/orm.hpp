/*
 *
 * Copyright 2021 dorm authors.
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

#ifndef __drift_orm_hpp__
#define __drift_orm_hpp__

#include "visit.hpp"
#include "macro_msgpack.hpp"
#include "macro_no_sql.hpp"

/***
 * These macros are used with VISIT_STRUCT_PP_MAP
 */
namespace drift {

#define VISIT_STRUCT_FIELD_COUNT(field_name)                                                       \
  + 1

#define VISIT_STRUCT_FIELD_HELPER(field_name)                                                      \
  std::forward<Visitor>(visitor)(#field_name, std::forward<Struct>(struct_instance_).field_name);

#define VISIT_STRUCT_FIELD_HELPER_PTR(field_name)                                                  \
  std::forward<Visitor>(visitor)(#field_name, &self_type::field_name);

#define VISIT_STRUCT_FIELD_HELPER_TYPE(field_name)                                                 \
  std::forward<Visitor>(visitor)(#field_name, drift::type_c<decltype(self_type::field_name)>{});

#define VISIT_STRUCT_FIELD_HELPER_ACC(field_name)                                                  \
  std::forward<Visitor>(visitor)(#field_name, drift::accessor<decltype(&self_type::field_name), &self_type::field_name>{});


#define VISIT_STRUCT_FIELD_HELPER_PAIR(field_name)                                                 \
  std::forward<Visitor>(visitor)(#field_name, std::forward<Struct1>(struct1_).field_name, std::forward<Struct2>(struct2_).field_name);

#define VISIT_STRUCT_MAKE_GETTERS(field_name)                                                      \
  template <typename Struct>                                                                       \
  static DRIFT_CONSTEXPR auto                                                                      \
    get_value(std::integral_constant<int, fields::field_name>, Struct && struct_) ->               \
    decltype((std::forward<Struct>(struct_).field_name)) {                                         \
    return std::forward<Struct>(struct_).field_name;                                               \
  }                                                                                                \
                                                                                                   \
  static DRIFT_CONSTEXPR auto                                                                      \
    name(std::integral_constant<int, fields::field_name>) ->                                       \
      decltype(#field_name) {                                                                      \
    return #field_name;                                                                            \
  }                                                                                                \
                                                                                                   \
  static DRIFT_CONSTEXPR auto                                                                      \
    pointer(std::integral_constant<int, fields::field_name>) ->                                    \
      decltype(&self_type::field_name) {                                                           \
    return &self_type::field_name;                                                                 \
  }                                                                                                \
                                                                                                   \
  static DRIFT_CONSTEXPR auto                                                                      \
    get_accessor(std::integral_constant<int, fields::field_name>) ->                               \
      drift::accessor<decltype(&self_type::field_name), &self_type::field_name > {                 \
    return {};                                                                                     \
  }                                                                                                \
                                                                                                   \
  static auto                                                                                      \
    type_at(std::integral_constant<int, fields::field_name>) ->                                    \
      drift::type_c<decltype(self_type::field_name)>;


} // namespace drift


#define DRIFT_FIELD_PACK(field_name)     o.pack(v.field_name);
#define DRIFT_FIELD_UNPACK(field_name)   o.via.array.ptr[drift::traits::visitable<self_type>::fields::field_name].as<decltype(self_type::field_name)>(),


#define DRIFT_ORM(struct_name_, conmmunicate_type_, orm_type_, primary_key_, ...)                  \
namespace drift {                                                                                  \
namespace traits {                                                                                 \
                                                                                                   \
template <>                                                                                        \
struct visitable<struct_name_, void> {                                                             \
  using self_type = struct_name_;                                                                  \
  static DRIFT_CONSTEXPR int count = VISIT_STRUCT_PP_NARG(__VA_ARGS__);                            \
                                                                                                   \
  static DRIFT_CONSTEXPR auto name()                                                               \
    -> decltype(#struct_name_) {                                                                   \
    return #struct_name_;                                                                          \
  }                                                                                                \
  static DRIFT_CONSTEXPR auto primary_key()                                                        \
    -> decltype(#primary_key_) {                                                                   \
    return #primary_key_;                                                                          \
  }                                                                                                \
                                                                                                   \
  static DRIFT_CONSTEXPR auto communicate_type()                                                   \
    -> decltype(conmmunicate_type_) {                                                              \
    return conmmunicate_type_;                                                                     \
  }                                                                                                \
  static DRIFT_CONSTEXPR auto orm_type()                                                           \
    -> decltype(orm_type_) {                                                                       \
    return orm_type_;                                                                              \
  }                                                                                                \
                                                                                                   \
  static DRIFT_CONSTEXPR const std::size_t field_count = 0                                         \
    VISIT_STRUCT_PP_MAP(VISIT_STRUCT_FIELD_COUNT, __VA_ARGS__);                                    \
                                                                                                   \
  template <typename Visitor, typename Struct>                                                     \
  DRIFT_CONSTEXPR static void apply(Visitor&& visitor, Struct&& struct_instance_)                  \
  {                                                                                                \
    VISIT_STRUCT_PP_MAP(VISIT_STRUCT_FIELD_HELPER, __VA_ARGS__)                                    \
  }                                                                                                \
                                                                                                   \
  template <typename Visitor, typename Struct1, typename Struct2>                                  \
  DRIFT_CONSTEXPR static void apply(Visitor&& visitor, Struct1 && struct1_, Struct2 && struct2_)   \
  {                                                                                                \
    VISIT_STRUCT_PP_MAP(VISIT_STRUCT_FIELD_HELPER_PAIR, __VA_ARGS__)                               \
  }                                                                                                \
                                                                                                   \
  template <typename Visitor>                                                                      \
  DRIFT_CONSTEXPR static void pointers(Visitor&& visitor)                                          \
  {                                                                                                \
    VISIT_STRUCT_PP_MAP(VISIT_STRUCT_FIELD_HELPER_PTR, __VA_ARGS__)                                \
  }                                                                                                \
                                                                                                   \
  template <typename Visitor>                                                                      \
  DRIFT_CONSTEXPR static void types(Visitor&& visitor)                                             \
  {                                                                                                \
    VISIT_STRUCT_PP_MAP(VISIT_STRUCT_FIELD_HELPER_TYPE, __VA_ARGS__)                               \
  }                                                                                                \
                                                                                                   \
  template <typename Visitor>                                                                      \
  DRIFT_CONSTEXPR static void visit_accessors(Visitor&& visitor)                                   \
  {                                                                                                \
    VISIT_STRUCT_PP_MAP(VISIT_STRUCT_FIELD_HELPER_ACC, __VA_ARGS__)                                \
  }                                                                                                \
                                                                                                   \
  struct fields {                                                                                  \
    enum index { __VA_ARGS__ };                                                                    \
  };                                                                                               \
                                                                                                   \
  VISIT_STRUCT_PP_MAP(VISIT_STRUCT_MAKE_GETTERS, __VA_ARGS__)                                      \
                                                                                                   \
  static DRIFT_CONSTEXPR const bool value = true;                                                  \
};                                                                                                 \
                                                                                                   \
}                                                                                                  \
}                                                                                                  \
                                                                                                   \
namespace msgpack {                                                                                \
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {                                            \
namespace adaptor {                                                                                \
template<>                                                                                         \
struct pack<struct_name_> {                                                                        \
    template <typename Stream>                                                                     \
    packer<Stream>& operator()(msgpack::packer<Stream>& o, struct_name_ const& v) const {          \
        o.pack_array(VISIT_STRUCT_PP_NARG(__VA_ARGS__));                                           \
        VISIT_STRUCT_PP_MAP(DRIFT_FIELD_PACK, __VA_ARGS__)                                         \
        return o;                                                                                  \
    }                                                                                              \
};                                                                                                 \
                                                                                                   \
template<>                                                                                         \
struct convert<struct_name_> {                                                                     \
    using self_type = struct_name_;                                                                \
    msgpack::object const& operator()(msgpack::object const& o, struct_name_& v) const {           \
        if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();                           \
        if (o.via.array.size != VISIT_STRUCT_PP_NARG(__VA_ARGS__)) throw msgpack::type_error();    \
        v = struct_name_ {                                                                         \
            VISIT_STRUCT_PP_MAP(DRIFT_FIELD_UNPACK, __VA_ARGS__)                                   \
            };                                                                                     \
        return o;                                                                                  \
    }                                                                                              \
};                                                                                                 \
                                                                                                   \
}                                                                                                  \
}                                                                                                  \
}                                                                                                  \
static_assert(true, "")



#endif // __drift_orm_hpp__
