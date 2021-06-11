
#include "orm.hpp"

#include "database.hpp"
#include "pg_connection.hpp"
#include "server_config.hpp"

#include <iostream>
#include <string>
#include <vector>

struct Test {
    int id1;
    uint32_t id2;
    std::string value1;
    std::vector<int> value2;
};

DRIFT_ORM(Test,                        // struct type
          CommunicationType::Msgpack,  // communication protocol type
          KV_DB_Type::RedisString,     // noSQL database store type
          id1,                         // primary key
          id1, id2, value1, value2);   // data field

void test_orm(const char* path_file_) {
    auto config = drift::parse_config(path_file_);
    std::cout << config.self_config.addr << std::endl;

    drift::db::DataBase db;
    db.connect<drift::db::RedisConnection, 1>("127.0.0.1:6379");

    const char* postgresql = "dbname=traindata user=postgres password=123456 hostaddr=127.0.0.1 port=5432";
    // dbname | user | pwd | addr | port
    const char* nn = "pgdb, pgusr, 123, 127.0.0.1:5432";

//    db.connect<drift::db::PGConnection, 1>("127.0.0.1, 6379, user, passwd, database1");

    auto conn = db.get_redis();

    auto temp(Test{50, 100, "temp", {12, 34}});

    auto insert_result = drift::db::insert(std::move(temp)).execute(conn);

    std::cout << insert_result.get_status() << std::endl;

    auto select_result = drift::db::select<Test>().where(FIELD(Test, id1).concat("50")).execute(conn).get_result();

    std::cout << select_result.id1 << std::endl;
    std::cout << select_result.id2 << std::endl;
    std::cout << select_result.value1 << std::endl;
    for (auto& v : select_result.value2) {
        std::cout << v << std::endl;
    }

}
