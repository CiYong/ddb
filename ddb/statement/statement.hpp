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

#ifndef __drift_db_statement_hpp__
#define __drift_db_statement_hpp__

#include "serializer.hpp"

#define FIELD(struct_name_, field_) \
    drift::db::TextExpression(drift::field<drift::traits::visitable<struct_name_>::fields::field_, struct_name_>())

namespace drift {
namespace db {

class ToQuery {
public:
    virtual std::string to_query() { return {}; };
};

class ExpressionConcat : ToQuery {
public:
    ExpressionConcat(std::string expression_) : _expression(expression_) {}

    virtual std::string to_query() override {
        return "ExpressionConcat to_query()";
    }

private:
    std::string _expression;
};

class ExpressionEquivalent {
public:
};

class ExpressionGreatThan {
public:
};

class ExpressionLessThan {
public:
};

class ExpressionLike {
public:
};

class ExpressionNotLike{
public:
};


template<typename First, typename Second>
class ExpressionPair : public ToQuery {
public:
    ExpressionPair(First first_, Second second_) : _first(first_), _second(second_) {}


//    virtual ExpressionPair<ExpressionPair<First, Second>, ExpressionConcat> concat(std::string& expression_) {
//        return {std::move(*this), ExpressionConcat(expression_)};
//    }

    inline virtual ExpressionPair& equivalent() {
        return *this;
    }

    inline virtual ExpressionPair& great_than() {

        return *this;
    }

    inline virtual ExpressionPair& less_than() {

        return *this;
    }

    inline virtual ExpressionPair& like() {

        return *this;
    }

    inline virtual ExpressionPair& not_like() {

        return *this;
    }

    inline virtual std::string operator()() {
        return {};
    }

    virtual std::string to_query() override {
//        return _first.to_query() + _second.to_query();
        return "";
    }

private:
    First _first;
    Second _second;
};


class TextExpression : public ToQuery {
public:
    TextExpression(const char* expression_) : _expression(expression_) {}
    TextExpression(std::string expression_) : _expression(expression_) {}
    TextExpression(std::string& expression_) : _expression(expression_) {}
    TextExpression(std::string&& expression_) : _expression(expression_) {}

    virtual TextExpression concat(std::string expression_) {
        _expression = expression_;
        return std::move(*this);
    }

    inline virtual TextExpression& equivalent() {
        return *this;
    }

    inline virtual TextExpression& great_than() {

        return *this;
    }

    inline virtual TextExpression& less_than() {

        return *this;
    }

    inline virtual TextExpression& like() {

        return *this;
    }

    inline virtual TextExpression& not_like() {

        return *this;
    }

    inline virtual std::string operator()() {
        return to_query();
    }

    virtual std::string to_query() override {

        return _expression;
    }

private:
    std::string _expression;
};

template<typename T>
class Result {
public:
    using Table = T;

    Result(redisReply* reply_) : _reply(reply_) {}
    ~Result() {
        if (_reply) {
            freeReplyObject(_reply);
            _reply = nullptr;
        }
    }

    enum Status {
        OK,
        NG
    };

    bool get_status() {

        return true;
    }

    T get_result() {
        RedisDeserializer deser;
        if (!_reply) {
            return {};
        }

        switch (_reply->type) {
        case REDIS_REPLY_STRING:
            return deser.deserialize<Table>(_reply->str);
        default:
            return {};
        }
    }

private:
    redisReply* _reply;
};


class NullQuery : public ToQuery {
public:
    virtual std::string to_query() { return {}; };
};

class BaseExpression : public ToQuery {
public:
    BaseExpression(TextExpression&& expression_) : _expression(expression_) {}

    TextExpression _expression;
};

class WhereStatement : public BaseExpression {
public:
    WhereStatement(TextExpression&& expression_) : BaseExpression(std::move(expression_)) {}

    inline virtual std::string to_query() {
        return _expression.to_query() + " ";
    }
};

class JoinStatement : public ToQuery {
public:
    JoinStatement(TextExpression&& expression_) : _expression(expression_) {}

    inline virtual std::string to_query() {
        return "join ";
    }

private:
    TextExpression _expression;
};

class LeftJoinStatement : public ToQuery {
public:
    inline virtual std::string to_query() {
        return "left join ";
    }
};

class RightJoinStatement : public ToQuery {
public:
    inline virtual std::string to_query() {
        return "right join ";
    }
};

template<typename First, typename Second, typename T>
class Chain : public ToQuery {
public:
    using Table = T;

    Chain(First first_, Second second_) : _first(first_), _second(second_) {}

    template<typename Connection>
    inline decltype(auto) execute(Connection& conn_) {
        auto query = to_query();
        auto reply = conn_.execute(query);

        return Result<Table>(reply);
    }

    template<typename Condition>
    inline Chain<Chain<First, Second, Table>*, WhereStatement, Table> where(TextExpression&& expression_) {
        return {this, WhereStatement(std::move(expression_))};
    }

    inline Chain<Chain<First, Second, Table>*, JoinStatement, Table> join() {
        return {this, JoinStatement("")};
    }

    inline virtual std::string to_query() {
        return _first->to_query() + _second.to_query();
    }

private:
    First _first;
    Second _second;
};

template<typename T>
class BaseStatement : public ToQuery {
public:
    using Table = T;

    template<typename Connection>
    inline decltype(auto) execute(Connection& conn_) {
        auto query = to_query();
        auto reply = conn_.execute(query);

        return Result<Table>(reply);
    }

    inline Chain<BaseStatement*, WhereStatement, Table> where(TextExpression expression_) {
        return {std::move(this), WhereStatement(std::move(expression_))};
    }

    inline Chain<BaseStatement*, JoinStatement, Table> join(TextExpression expression_) {
        return {std::move(this), JoinStatement(std::move(expression_))};
    }

    inline Chain<BaseStatement*, JoinStatement, Table> equivalent(TextExpression expression_) {
        return {std::move(this), JoinStatement(std::move(expression_))};
    }

    inline Chain<BaseStatement*, JoinStatement, Table> great_than(TextExpression expression_) {
        return {std::move(this), JoinStatement(std::move(expression_))};
    }

    inline Chain<BaseStatement*, JoinStatement, Table> less_than(TextExpression expression_) {
        return {std::move(this), JoinStatement(std::move(expression_))};
    }

    inline Chain<BaseStatement*, JoinStatement, Table> like(TextExpression expression_) {
        return {std::move(this), JoinStatement(std::move(expression_))};
    }

    inline Chain<BaseStatement*, JoinStatement, Table> not_like(TextExpression expression_) {
        return {std::move(this), JoinStatement(std::move(expression_))};
    }
};

template<typename T>
class InsertStatement : public BaseStatement<T> {
public:
    using Table = T;

    InsertStatement(T arg_) : _arg(arg_) {}

    inline virtual std::string to_query() override {
        RedisSerializer ser;
        auto pair = ser.serialize(_arg);

        auto name = drift::traits::visitable<Table>::name();

        switch (drift::traits::visitable<Table>::orm_type()) {
        case KV_DB_Type::RedisHash:
            return std::string("HMSET ") + pair.key() + " " + pair.value();
        case KV_DB_Type::RedisString:
            return std::string("SET ") + pair.key() + " " + pair.value();
        default:
            return {};
        }
    }
private:
    T _arg;
};


template<typename T>
class SelectStatement : public BaseStatement<T> {
public:
    using Table = T;

    inline virtual std::string to_query() {
        auto name = drift::traits::visitable<Table>::name();

        switch (drift::traits::visitable<Table>::orm_type()) {
        case KV_DB_Type::RedisHash:
            return std::string("HMGET ") + name + ":";
        case KV_DB_Type::RedisString:
            return std::string("GET ") + name + ":";
        default:
            return {};
        }
    }
};

template<typename T>
InsertStatement<T> insert(T&& arg_) {
    return InsertStatement<T>(arg_);
}

template<typename T>
SelectStatement<T> select() {
    return SelectStatement<T>();
}

} // namespace db
} // namespace drift

#endif // __drift_db_statement_hpp__
