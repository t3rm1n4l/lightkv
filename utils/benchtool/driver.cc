#include "interface.hh"
#include "lightkv_db.hh"
#include "sqlite_db.hh"
#include "timing.hh"
#include <unistd.h>

int main(int argc, char **argv) {
    std::string dbtype("sqlite");
    BaseDB *db;
    char c;

    while ((c = getopt (argc, argv, "d:")) != -1) {
        switch (c) {
            case 'd':
                dbtype = optarg;
                break;
        }
    }

    std::cout<<"DB type:"<<dbtype<<std::endl;

    if (dbtype == "lightkv") {
        db = new LightKVDB("/tmp/");
    } else {
        db = new SqliteDB("/tmp/test.sqlite");
    }

    int i;
    std::string val("value..........");
    {
        Timing t("insert of 50000");
        for (i=0; i< 50000; i++) {
            stringstream ss;
            ss<<"key_"<<i;
            std::string k = ss.str();
            db->Insert(k, val);
        }
    }

    return 0;
}
