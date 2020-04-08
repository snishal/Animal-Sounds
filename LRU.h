#include <queue>
#include<unordered_map>
#include<iostream>
using namespace std;

class LRU
{
private:
    int capacity;
    unordered_map<string, string> map;
    queue<string> q;
public:
    LRU(int _capacity){
        capacity = _capacity;
    }

    void add(string key, string val){
        if(q.size() == capacity){
            map.erase(q.front());
            q.pop();
        }
        q.push(key);
        map[key] = val;
    }

    string search(string key){
        return map[key];
    }
};