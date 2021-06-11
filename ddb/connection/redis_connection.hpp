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

#ifndef __drift_db_redis_connection_hpp__
#define __drift_db_redis_connection_hpp__

#include "connection_if.hpp"
#include "hiredis/hiredis.h"
#include "hiredis/async.h"

#include "serializer.hpp"

#include <cstdlib>
#include <cstdio>

#include <sstream>
#include <iostream>
#include <vector>


struct redisContext;

namespace drift {
namespace db {

class RedisConnection : public ConnectionIF {
public:
    virtual bool connect(std::string arg) override;
    virtual void disconnect() override;

    RedisConnection() : _ctx(nullptr) {}
    ~RedisConnection() {}

    template<typename T>
    T load() {
        auto table_name = T::table();
        return T{};
    }

    bool transaction() {

        return true;
    }

    void commit() {
    }

    void rollback() {
    }

    void ProcessReply( redisReply * pReply )
    {
        redisReply * pSubReply = NULL;

        if ( pReply != NULL && pReply->elements == 3 )
        {
            pSubReply = pReply->element[2];
            printf( "Msg [%s]\n", pSubReply->str );
        }
    }

    inline void redis_command(const char* cmd_, const char* key_, const char* value_) {
        if (_ctx->err == 0) {
            auto reply = static_cast<redisReply*>(redisCommand(_ctx, "%s %s %s", cmd_, key_, value_));
            freeReplyObject(reply);

            auto get_reply = static_cast<redisReply*>(redisCommand(_ctx, "GET %s", key_));

            std::cout << get_reply->type << std::endl;
            std::cout << get_reply->len << std::endl;
            std::cout << get_reply->str << std::endl;
            freeReplyObject(reply);
        }
    }

    template<typename ...Args>
    inline redisReply* execute(const char* cmd, const char* fmt, Args... args_) {
        return static_cast<redisReply*>(redisCommand(_ctx, fmt, cmd, args_...));
    }

    inline redisReply* execute(const char* content_) {
        return static_cast<redisReply*>(redisCommand(_ctx, content_));
    }

    inline redisReply* execute(std::string& content_) {
        return static_cast<redisReply*>(redisCommand(_ctx, content_.c_str()));
    }

    inline bool set(const char* cmd_, const char* key_, const char* value_) {
        auto reply = execute(cmd_, "%s %s %s", key_, value_);
        auto result = parse_reply(reply);
        freeReplyObject(reply);

        return true;
    }

    inline std::string get(const char* cmd_, const char* key_) {
        auto reply = execute(cmd_, "%s %s", key_);
        auto result = parse_reply(reply);
        freeReplyObject(reply);

        return result;
    }

    template<typename Queryable>
    inline bool insert_into(Queryable& arg_) {
        RedisSerializer ser;
        auto pair = ser.serialize(arg_);
        set(pair.cmd(), pair.key(), pair.value());

        return true;
    }

//    class Result {
//    public:
//        std::string get_result() {
//            return {};
//        }
//    };

//    class ToQuery {
//    public:
//        virtual std::string to_query() = 0;
//    };

//    class NullQuery : public ToQuery {
//    public:
//        virtual std::string to_query() { return {}; };
//    };

//    template<typename Condition>
//    class WhereStatement : public ToQuery {
//    public:
//        WhereStatement(Condition condition_) : _condition(condition_) {}
//        inline virtual std::string to_query() {
//            auto abc = _condition(1);

//            return "where ";
//        }

//    private:
//        Condition _condition;
//    };

//    class JoinStatement : public ToQuery {
//    public:
//        inline virtual std::string to_query() {
//            return "join ";
//        }
//    };

//    class LeftJoinStatement : public ToQuery {
//    public:
//        inline virtual std::string to_query() {
//            return "left join ";
//        }
//    };

//    class RightJoinStatement : public ToQuery {
//    public:
//        inline virtual std::string to_query() {
//            return "right join ";
//        }
//    };

//    template<typename First, typename Second, typename T>
//    class StatementPair : public ToQuery {
//    public:
//        using Table = T;

//        StatementPair(First first_, Second second_) : _first(first_), _second(second_) {}

//        template<typename Connection>
//        inline Result execute(Connection& conn_) {
//            auto query = to_query();
////            auto content = conn_.execute(query);
////            auto content = conn_.get("", "");

//            RedisDeserializer deser;
////            auto result = deser.deserialize<Table>(content);

//            std::cout << query << std::endl;

//            return {};
//        }

//        template<typename Condition>
//        inline StatementPair<StatementPair<First, Second, typename First::Table>, WhereStatement<Condition>, Table> where(Condition condition_) {
//            return {*this, WhereStatement<Condition>(condition_)};
//        }

//        inline StatementPair<StatementPair<First, Second, typename First::Table>, JoinStatement, Table> join() {
//            return {*this, JoinStatement{}};
//        }

//        inline virtual std::string to_query() {
//            return _first.to_query() + _second.to_query();
//        }

//    private:
//        First _first;
//        Second _second;
//    };

//    template<typename T, typename connection_type>
//    class SelectStatement : public ToQuery {
//    public:
//        using Table = T;

//        SelectStatement(connection_type& conn_) : _conn(conn_) {}

//        inline virtual std::string to_query() {
//            auto name = drift::traits::visitable<Table>::name();

//            switch (drift::traits::visitable<Table>::orm_type()) {
//            case KV_DB_Type::RedisHash:
//                return std::string("HMSET ") + name + ":";
//            case KV_DB_Type::RedisString:
//                return std::string("SET ") + name + ":";
//            default:
//                return {};
//            }
//        }

//        template<typename Condition>
//        inline StatementPair<SelectStatement, WhereStatement<Condition>, Table> where(Condition condition_) {
//            return {std::move(*this), WhereStatement<Condition>(condition_)};
//        }

//        inline StatementPair<SelectStatement, JoinStatement, Table> join() {
//            return {std::move(*this), JoinStatement{}};
//        }

//    private:
//        connection_type& _conn;
//    };

//    template<typename T>
//    SelectStatement<T, RedisConnection&> select() {
//        return SelectStatement<T, RedisConnection&>(*this);
//    }

    inline std::string parse_reply(redisReply* reply_) {
        std::stringstream ss;

        switch (reply_->type) {
        case REDIS_REPLY_STATUS:
            return {};
        case REDIS_REPLY_ERROR:
            return {};
        case REDIS_REPLY_INTEGER: // int
            ss << reply_->integer;
            break;
        case REDIS_REPLY_STRING: // string
            ss << reply_->str;
            break;
        case REDIS_REPLY_ARRAY: // hash
            for (size_t i = 1; i < reply_->elements; i + 2) {
                ss << parse_reply(reply_->element[i]);
            }
            break;
        case REDIS_REPLY_SET: // set
            std::cout << "[REDIS_REPLY_SET]:" << std::endl;
            break;
        case REDIS_REPLY_PUSH: // publish
            std::cout << "[REDIS_REPLY_PUSH]:" << std::endl;
            break;
        default:
            break;
        }

        return ss.str();
    }

    bool delete_row(std::string&& sql_content_) {
        log(sql_content_);
        return true;
    }

    bool update(std::string&& sql_content_) {
        log(sql_content_);
        return true;
    }

    void log(std::stringstream content_) {
        std::cout << "[Connection]:" << content_.str() << std::endl;
    }

    void log(std::string content_) {
        std::cout << "[Connection]:" << content_ << std::endl;
    }

private:
    redisContext* _ctx;
};

} // namespace db
} // namespace drift

#endif // __drift_db_redis_connection_hpp__
