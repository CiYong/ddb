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


#ifndef __drift_orm_serializer_hpp__
#define __drift_orm_serializer_hpp__

#include "orm.hpp"

#include "hiredis/hiredis.h"

#include <sstream>
#include <vector>
#include <iostream>


namespace drift {

using spair = std::pair<std::string, std::string>;
using vpair = std::vector<std::pair<std::string, std::string>>;

class SerializerBase {
public:
//    std::string values() {
//        return _stream.pair.values.str();
//    }

//    std::string fields() {
//        return _stream.pair.fields.str();
//    }

//    std::string datas() {
//        return _stream.signle.pairs.str();
//    }

//    std::stringstream& values_ref() {
//        return _stream.pair.values;
//    }

//    std::stringstream& fields_ref() {
//        return _stream.pair.fields;
//    }

//    std::stringstream& datas_ref() {
//        return _stream.signle.pairs;
//    }

    template<typename T>
    SerializerBase& pack(T& v) {

        return *this;
    }

    SerializerBase& pack_u8(int d) {
        pack_u8_imp(d);
        return *this;
    }

//    union meta_data_t {
//        struct {
//            std::stringstream fields;
//            std::stringstream values;
//        } pair;
//        struct {
//            std::stringstream pairs;
//        } signle;
//    };


protected:
    template<typename T>
    void pack_u8_imp(T d) {

    }

protected:
//    meta_data_t _stream;

};

//class SQLSerializer : public SerializerBase {
//public:
//    template <typename T>
//    void operator()(const char* field_, const T& value_) {
//        fields_ref() << field_;
//        values_ref() << value_;
//    }

//    std::vector<spair> result;

//    void operator()(const char * name, const int & v) {
//      result.emplace_back(spair{std::string{name}, std::to_string(v)});
//    }

//    void operator()(const char * name, const std::string & s) {
//      result.emplace_back(spair{std::string{name}, s});
//    }

//    void operator()(const char * name, const std::vector<int> & v) {
//        std::string str;
//        for (auto& val : v) {
//            str = str + std::to_string(val);
//        }
//      result.emplace_back(spair{std::string{name}, str});
//    }
//};

template<typename T>
struct TVisitor;


template<>
struct TVisitor<int> {
    std::string& operator()(std::string& content_, int value_) {
        content_.append(std::to_string(value_));

        return content_;
    }
};


struct test_visitor_ptr {
  std::vector<spair> result;

  template <typename C>
  void operator()(const char* name, int C::* a) {
      printf("pointer int=%p\n", a);
    result.emplace_back(spair{std::string{name}, "int"});
  }

  template <typename C>
  void operator()(const char* name, uint32_t C::* a) {
      printf("pointer uint32_t=%p\n", a);
    result.emplace_back(spair{std::string{name}, "uint32_t"});
  }

  template <typename C>
  void operator()(const char* name, std::string C::* a) {
      printf("pointer string=%p\n", a);
    result.emplace_back(spair{std::string{name}, "std::string"});
  }

  template <typename C>
  void operator()(const char* name, std::vector<int> C::* a) {
      printf("pointer vector=%p\n", a);
    result.emplace_back(spair{std::string{name}, "std::vector<int>"});
  }
};

struct test_visitor_type {
  std::vector<spair> result;

  void operator()(const char* name, drift::type_c<int>) {
    result.emplace_back(spair{std::string{name}, "int"});
  }

  void operator()(const char* name, drift::type_c<uint32_t>) {
    result.emplace_back(spair{std::string{name}, "uint32_t"});
  }

  void operator()(const char* name, drift::type_c<std::string>) {
    result.emplace_back(spair{std::string{name}, "std::string"});
  }

  void operator()(const char* name, drift::type_c<std::vector<int>>) {
    result.emplace_back(spair{std::string{name}, "std::vector<int>"});
  }
};



class RedisSerializer : public SerializerBase {
public:

    struct BaseVisitor {
        std::string _cmd;
        std::string _name;
        std::string _content;
        std::string _pk_name;
        std::string _pk_value;

        virtual ~BaseVisitor() = default;

        inline virtual const char* command() { return "SET"; };

        inline std::string key() {
            return _name + ":" + _pk_value;
        }

        inline std::string value() {
            return _content;
        }

        inline std::string generate() {
            return _name + ":" + _pk_value + " " + _content;
        }

        virtual void operator()(const char* name_, const int& value_) = 0;
        virtual void operator()(const char* name_, const uint32_t& value_) = 0;
        virtual void operator()(const char* name_, const std::string& value_) = 0;
        virtual void operator()(const char* name_, const std::vector<int>& value_) = 0;
    };

    struct HashVisitor : public BaseVisitor {
        ~HashVisitor() = default;

        inline virtual const char* command() { return "HSET"; };

        void operator()(const char* name_, const int& value_) {
            if (_pk_name == name_) {
                _pk_value = std::to_string(value_);
            }
            _content.append(std::string{name_} + " " + std::to_string(value_) + " ");
        }

        void operator()(const char* name_, const uint32_t& value_) {
            if (_pk_name == name_) {
                _pk_value = std::to_string(value_);
            }
            _content.append(std::string{name_} + " " + std::to_string(value_) + " ");
        }

        void operator()(const char* name_, const std::string& value_) {
            if (_pk_name == name_) {
                _pk_value = value_;
            }
            _content.append(std::string{name_} + " " + value_ + " ");
        }

        void operator()(const char* name_, const std::vector<int>& value_) {
            _content.append(name_);
            _content.append(" ");
            for (auto& val : value_) {
                _content.append(std::to_string(val) + ",");
            }
        }

        template <typename T>
        void operator()(const char * name, const T & t) {
            //    result.emplace_back(spair{std::string{name}, std::to_string(t)});
        }
    };


    struct StringVisitor : public BaseVisitor {
        ~StringVisitor() = default;

        inline virtual const char* command() { return "SET"; };

        void operator()(const char* name_, const int& value_) {
            if (_pk_name == name_) {
                _pk_value = std::to_string(value_);
            }
        }

        void operator()(const char* name_, const uint32_t& value_) {
            if (_pk_name == name_) {
                _pk_value = std::to_string(value_);
            }
        }

        void operator()(const char* name_, const std::string& value_) {
            if (_pk_name == name_) {
                _pk_value = value_;
            }
        }

        void operator()(const char* name_, const std::vector<int>& value_) {
        }
    };


    struct Pair {
        std::string _cmd;
        std::string _key;
        std::string _value;

        inline const char* cmd() {
            return _cmd.c_str();
        }
        inline const char* key() {
            return _key.c_str();
        }
        inline const char* value() {
            return _value.c_str();
        }
    };

    template<typename T>
    inline void basic_op(BaseVisitor& visitor_, T& value_) {
        visitor_._name = drift::traits::visitable<T>::name();
        visitor_._pk_name = drift::traits::visitable<T>::primary_key();
        drift::for_each(value_, visitor_);
    }

    template<typename Result = Pair, typename T>
    Result serialize(T& value_) {
        auto type = drift::traits::visitable<T>::orm_type();
        BaseVisitor* visitor = nullptr;

        switch (drift::traits::visitable<T>::orm_type()) {
        case KV_DB_Type::RedisHash:
            visitor = new HashVisitor;
            break;
        case KV_DB_Type::RedisString:
            visitor = new StringVisitor;
            visitor->_content = drift::orm::ser_msgpack<std::string>(value_);
            break;
        default:
            return {};
        }

        basic_op(*visitor, value_);

        auto cmd = visitor->command();
        auto key = visitor->key();
        auto value = visitor->value();
        delete visitor;

        return {cmd, key, value};
    }
};


// Deserializer
class DeserializerBase {
public:

};


class RedisDeserializer : public DeserializerBase {
public:
    vpair result;

    void operator()(const char * name, const int & v) {
      result.emplace_back(spair{std::string{name}, std::to_string(v)});
    }

    void operator()(const char * name, const uint32_t & v) {
      result.emplace_back(spair{std::string{name}, std::to_string(v)});
    }

    void operator()(const char * name, const std::string & s) {
      result.emplace_back(spair{std::string{name}, s});
    }

    void operator()(const char * name, const std::vector<int> & v) {
        std::string str;
        for (auto& val : v) {
            str = str + std::to_string(val) + " ";
        }
      result.emplace_back(spair{std::string{name}, str});
    }

    std::vector<int> parse_vec_int(std::string& str_) {
        std::vector<int> ret;
        size_t offset = 0;

        for (size_t i = 0; i < str_.size(); i++) {
            if ((str_[i]) == ',') {
                auto val = std::stoi(std::string(str_.data() + offset, i)) ;
                ret.push_back(val);
                offset += i + 1;
            }
        }

        return ret;
    }

//    template<typename T>
//    T deserialize(vpair& pairs_) {
//        T temp{};
//        void* ptr = &temp;

//        for (size_t i = 0; i < pairs_.size(); i++) {
//            if (pairs_[i].first == "id1") {
//                auto value = static_cast<int32_t*>(ptr);
//                *value = std::stoi(pairs_[i].second);
//                ptr += sizeof(int32_t);
//            }
//            else if (pairs_[i].first == "id2") {
//                auto value = static_cast<uint32_t*>(ptr);
//                *value = std::stoul(pairs_[i].second);
//                ptr += sizeof(uint32_t);
//            }
//            else if (pairs_[i].first == "value1") {
//                auto value = static_cast<std::string*>(ptr);
//                *value = pairs_[i].second;
//                ptr += sizeof(std::string);
//            }
//            else if (pairs_[i].first == "value2") {
//                auto value = static_cast<std::vector<int>*>(ptr);
//                *value = parse_vec_int(pairs_[i].second);
//                ptr += sizeof(temp);
//            }
//        }

//        return temp;
//    }

    template<typename T, typename content_string>
    T deserialize(content_string& content_) {
        return drift::orm::deser_msgpack<T>(std::string(content_));
    }


//    template<typename T>
//    T deserialize_reply(redisReply* reply_) {
//        T temp{0};
//        void* ptr = &temp;

//        switch (reply_->type) {
//        case REDIS_REPLY_INTEGER:
//            ptr = parse_reply<int>(ptr, reply_->integer);
//            break;
//        case REDIS_REPLY_STRING:
//            ptr = parse_reply<std::string>(ptr, reply_->str);
//            break;
//        case REDIS_REPLY_ARRAY:

//            for (size_t i = 1; i < reply_->elements; i += 2) {
//                if (reply_->element[i]->type == REDIS_REPLY_INTEGER) {
//                    std::cout << "array int" << std::endl;
//                    ptr = parse_reply<int>(ptr, reply_->element[i]->integer);
//                }
//                else if (reply_->element[i]->type == REDIS_REPLY_STRING) {
//                    std::cout << "array string=" << std::stoi(reply_->element[i]->str) << std::endl;
//                    ptr = parse_reply<int>(ptr, std::stoi(reply_->element[i]->str));
//                }
//            }
//            break;
//        case REDIS_REPLY_SET:
////            ptr = parse_reply<int>(ptr, reply_->integer);
//            break;
//        case REDIS_REPLY_PUSH:
////            ptr = parse_reply<int>(ptr, reply_->integer);
//            break;
//        default:
//            break;
//        }

//        return temp;
//    }


private:
    template<typename DestType, typename SrcType>
    void* parse_reply(void* ptr_, SrcType value_) {
        auto memory = static_cast<DestType*>(ptr_);
        *memory = DestType(value_);
        ptr_ += sizeof(DestType);
        return ptr_;
    }

};


} // namespace drift


#endif // __drift_orm_serializer_hpp__
