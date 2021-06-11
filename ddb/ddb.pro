TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += connection statement

LIBS += -lhiredis

SOURCES += \
        connection/cache_connection.cpp \
        connection/database.cpp \
        connection/pg_connection.cpp \
        connection/redis_connection.cpp \
        main.cpp \
        test.cpp

HEADERS += \
    connection/cache_connection.hpp \
    connection/connection_if.hpp \
    connection/connection_pool.hpp \
    connection/database.hpp \
    connection/pg_connection.hpp \
    connection/redis_connection.hpp \
    macro_msgpack.hpp \
    macro_no_sql.hpp \
    marco_expend.hpp \
    orm.hpp \
    serializer.hpp \
    server_config.hpp \
    statement/statement.hpp \
    visit.hpp
