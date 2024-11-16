#ifndef REDIS_SERVER_H
#define REDIS_SERVER_H


#include <unordered_map>
#include <string>
using namespace std;


class RedisServer {
public:
    void set(const string& key, const string& value);
    string get(const string& key) const;
    bool del(const string& key);

private:
    unordered_map<string, string> store;
};

#endif