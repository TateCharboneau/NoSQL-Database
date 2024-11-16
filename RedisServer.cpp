#include "RedisServer.hpp"


//basic functions
void RedisServer::set(const string& key, const string& value) {
    store[key] = value;
}

string RedisServer::get(const string& key) const {
    auto cur = store.find(key);
    if (cur != store.end()) { // checks if cur = end of store
        return cur->second; // first stores key and second stores data value
    }
    return "";
}

bool RedisServer::del(const string& key) {
    return store.erase(key) != 0; // store.erase(key): Returns 1 if key found, 0 if not found
}