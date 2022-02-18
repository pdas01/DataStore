#ifndef LRUCACHE_H
#define LRUCACHE_H

#include <string>
#include <unordered_map>
#include <list>
#include <vector>
#include <utility>

#include "DataDao.h"

class LRUCache {
public:
    LRUCache(int maxCacheCapacity);
    ~LRUCache();
    
    DataEntity Get(const std::string& entity_id);
    void Put(DataEntity entity);

private:
    void Update(const std::string& entity_id);
    void Delete();
    void HandleCacheMiss(const DataEntity& entity);
    DataDao *db;
    // Double ended Linked list which holds all the Entity_id.
    typedef std::list<std::string> queue; 
    // Pair of DataEntity and iterator for list
    typedef std::pair<DataEntity, queue::iterator> cache_value;
    
    // Key is Entity_id and Value is DataEntity
    std::unordered_map<std::string,  cache_value> cache;
    queue lru;

    int maxCacheCapacity;
};

#endif