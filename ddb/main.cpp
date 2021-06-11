#include <iostream>

#include "test.cpp"

using namespace std;

int main(int argc, char* argv[])
{
    const char* path_file = "./ddb.conf";
    test_orm(path_file);
    return 0;
}
