#include "interface.hh"
#include "lightkv_db.hh"
#include "sqlite_db.hh"
#include "timing.hh"
#include <unistd.h>

#include <vector>

using namespace std;

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
        db = new SqliteDB("/tmp/data.sqlite");
    }

    int i;
    vector<uint64_t> rids;
    std::string val("value..........");
    {
        Timing t("insert of 5000000");
        for (i=0; i< 5000000; i++) {
            stringstream ss;
            ss<<"key_"<<i;
            std::string k = ss.str();
            rids.push_back(db->Insert(k, val));
        }
    }

    {
        val = "newvalue";
        Timing t("update of 5000000");
        for (i=0; i< 5000000; i++) {
            stringstream ss;
            ss<<"key_"<<i;
            std::string k = ss.str();
            db->Update(rids.at(i), k, val);
        }
    }

    {
        Timing t("Get of 5000000");
        for (i=0; i< 5000000; i++) {
            string k,v;
            db->Get(rids.at(i), k, v);
        }
    }

    {
        Timing t("Delete of 5000000");
        for (i=0; i< 5000000; i++) {
            string k,v;
            db->Delete(rids.at(i));
        }
    }

    return 0;
}
