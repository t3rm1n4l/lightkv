#include "interface.hh"
#include "lightkv_db.hh"
#include "sqlite_db.hh"

int main() {
    BaseDB *db1,*db2;
    db1 = new LightKVDB("/tmp/");
    db2 = new SqliteDB("/tmp/test.sqlite");
    return 0;
}
