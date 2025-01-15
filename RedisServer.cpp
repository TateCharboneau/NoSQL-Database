#include "RedisServer.hpp"


// basic functions
void RedisServer::set(const string& key, const string& value) {
    lock_guard<mutex> lock(mtx); // lock mutex
    store[key] = value;
}

string RedisServer::get(const string& key) const {
    lock_guard<mutex> lock(mtx); // lock mutex
    auto cur = store.find(key);
    if (cur != store.end()) { // checks if cur = end of store
        return cur->second; // first stores key and second stores data value
    }
    return "";
}

bool RedisServer::del(const string& key) {
    lock_guard<mutex> lock(mtx); // lock mutex
    return store.erase(key) != 0; // store.erase(key): Returns 1 if key found, 0 if not found
}

void RedisServer::saveToFile(const string& key, const string& value) {
    lock_guard<mutex> lock(mtx); // lock mutex
    ofstream file("database.txt", ios::app);
    if (file.is_open()) {
        file << key << " " << value << "\n";
        file.close();
    } 
    else {
        cerr << "Error: Could not open database.txt for writing." << endl;
    }
}

void RedisServer::loadFromFile() {
    lock_guard<mutex> lock(mtx); // lock mutex
    ifstream file("database.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string key, value;
            if (iss >> key >> value) {
                store[key] = value;
            }
        }
        file.close();
    } 
    else {
        cerr << "Error: Could not open database.txt for reading." << endl;
    }
}