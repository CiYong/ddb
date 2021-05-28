#include "connection.hpp"

using namespace std;

struct Test {
    int id;
    int key;
    int value;
};

namespace drift {
namespace db {

template<>
struct Insertable<Test> {
    template <typename Stream>
    serializer<Stream>& operator()(serializer<Stream>& o, Test const& v) const {
        o.pack(v.id);
        o.pack(v.key);
        o.pack(v.value);

        return o;
    }
};

} // namespace db
} // namespace drift

DDB_ORM_ADD(Test,
            id:pk,
            key:pk,
            value)


void test_example() {
    using namespace drift::db;

    Test test{50,100,200};

    Connection conn;
    conn.insert_into(std::move(test));
}
