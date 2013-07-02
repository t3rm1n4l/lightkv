// Base class for db layer interface

using namespace std;

class DBIterator {
public:
    virtual DBIterator(BaseDB *d) = 0;
    virtual ~DBIterator() {}
    virtual bool Next(uint64_t &tokem string &key, string &val) = 0;
};

class BaseDB {
public:
    virtual BaseDB(string basepath) = 0;
    virtual ~BaseDB() {}
    virtual uint64_t Insert(string &key, string &val) = 0;
    virtual uint64_t Update(uint64_t token, string &key, string &val) = 0;
    virtual bool Get(uint64_t token, string &key, string &val) = 0;
    virtual bool Delete(uint64_t token) = 0;
    virtual DBIterator Iterator() = 0;
};
