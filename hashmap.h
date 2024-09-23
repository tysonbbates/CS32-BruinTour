#ifndef hashmap_h
#define hashmap_h
#include "hashmap.h"
#include <string>
#include <list>
#include <algorithm>
#include <utility>
#include <iostream>
using namespace std;

const int START_BUCKETS = 10;

template <typename T>
class HashMap
{
public:
    HashMap(double max_load = 0.75): maxLoad(max_load), numBuckets(START_BUCKETS), numKeys(0)
    {
        if(maxLoad <= 0) maxLoad = 0.75;
        hashTable = new list<pair<string, T>>[START_BUCKETS]();
    }
    ~HashMap(){
        delete [] hashTable;
    }
    int size() const{
        return numKeys;
    }
    // The insert method associates one item (key) with another (value).
    // If no association currently exists with that key, this method inserts
    // a new association into the hashmap with that key/value pair. If there is
    // already an association with that key in the hashmap, then the item
    // associated with that key is replaced by the second parameter (value).
    // Thus, the hashmap must contain no duplicate keys.
    void insert(const std::string& key, const T& value){
        size_t h = hash<string>()(key);
        int bucketNum = h % numBuckets;
        for(typename list<pair<string, T>>::iterator p = hashTable[bucketNum].begin(); p != hashTable[bucketNum].end(); p++){
            if((*p).first == key){ //key already in hashTable
                (*p).second = value;
                return;
            }
        }
        //bucket is empty or bucket does not already include key
        pair <string, T> pairToAdd(key, value);
        (hashTable+bucketNum)->push_back(pairToAdd);
        numKeys++;
        if(((double)numKeys)/numBuckets > maxLoad)
            rehash();
        return;
    }
 // Defines the bracket operator for HashMap, so you can use your map like this:
 // your_map["david"] = 2.99;
 // If the key does not exist in the hashmap, this will create a new entry in
 // the hashmap and map it to the default value of type T (0 for builtin types).
 // It returns a reference to the newly created value in the map.
    T& operator[](const std::string& key){
        T* valuePtr = find(key);
        if(valuePtr == nullptr){
            insert(key, T());
            return *(find(key));
        }
        return (*valuePtr);
    }
 // If no association exists with the given key, return nullptr; otherwise,
 // return a pointer to the value associated with that key. This pointer can be
 // used to examine that value within the map.
    const T* find(const std::string& key) const{
        size_t h = hash<string>()(key);
        int bucketNum = h % numBuckets;
        for(typename list<pair<string, T>>::iterator p = hashTable[bucketNum].begin(); p != hashTable[bucketNum].end(); p++){
            if((*p).first == key) return &((*p).second);
        }
        return nullptr;
    }
 // If no association exists with the given key, return nullptr; otherwise,
 // return a pointer to the value associated with that key. This pointer can be
 // used to examine that value or modify it directly within the map.
 T* find(const std::string& key) {
 const auto& hm = *this;
 return const_cast<T*>(hm.find(key));
 }
private:
    HashMap(const HashMap&) = delete;
    HashMap& operator=(const HashMap&) = delete;

    double maxLoad;
    int numBuckets;
    int numKeys;
    list<pair<string, T>>* hashTable;
    
    //helper functions
    void rehash(){
        list<pair<string, T>>* tempTable = new list<pair<string, T>>[2*numBuckets]();
        for(int i = 0; i < numBuckets; i++){
            for(auto const& p : *(hashTable+i)){
                size_t h = hash<string>()(p.first);
                int bucketNum = h % (2*numBuckets);
                (tempTable+bucketNum)->emplace_back(std::move(p));
            }
        }
        delete [] hashTable;
        hashTable = tempTable;
        numBuckets *= 2;
    }
};

#endif
