#ifndef REDIS_SERVER_H
#define REDIS_SERVER_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;


class RedisServer {
public:
    // commands
    void set(const string& key, const string& value);
    string get(const string& key) const;
    bool del(const string& key);

    // persistence
    void saveToFile(const std::string& key, const std::string& value);
    void loadFromFile();

private:
    unordered_map<string, string> store;
};

#endif