#include "interface.hh"
#include "lightkv_db.hh"

main() {
    BaseDB *db;
    db = new LightKVDB("/tmp/");

}
