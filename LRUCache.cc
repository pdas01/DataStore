#include "LRUCache.h"

#include <iostream>
#include <stdexcept>

LRUCache::LRUCache(int maxCacheCapacity) 
: db(DataDao::GetInstance()), maxCacheCapacity(maxCacheCapacity) { 
}

LRUCache::~LRUCache() {
    cache.clear();
    lru.clear();
}

/**
 * Function: Get - function fetches from cache if present else 
 * fetches from db
 * @ param value - entity_id
 * @ return type- DataEntity 
 */
DataEntity LRUCache::Get(const std::string& entity_id) {
    auto itr = cache.find(entity_id);
    if(itr != cache.end()){
        Update(entity_id);
        std::cout << "Found in Cache: " << "ID: " << cache[entity_id].first.entity_id << " Name: " << cache[entity_id].first.entity_name << std::endl;
        return cache[entity_id].first;
    } else { 
        DataEntity entity = db->GetValue(entity_id);
        HandleCacheMiss(entity);
        std::cout << "Found in DB:"<< "ID: " << entity.entity_id << " Name: " << entity.entity_name << std::endl;
        return entity;
    }
    throw std::invalid_argument( "Entity Id not found" );
}

/**
 * Function: Put - inserts DB and if entity present 
 *                 in cache it Updates freq 
 * 
 * @ param value - DataEntity
 * @ return type - void
 */
void LRUCache::Put(DataEntity entity) {

    // increament the version to indicate the entity is mutated
    entity.version++;
    auto itr = cache.find(entity.entity_id);

    // if found then update value 
    if( itr != cache.end()){
        Update(entity.entity_id);
        cache[entity.entity_id].first = entity;
    } else { //or else add new value to cache 
        HandleCacheMiss(entity);
    }
    db->InsertData(entity);
}

/**
 * Function: Update -  Updates the freq of access to DataEntity
 * @ param value - entity_id
 * @ return type - void
 */
void LRUCache::Update(const std::string& entity_id) {
    lru.erase(cache[entity_id].second);
    lru.push_front(entity_id);
    cache[entity_id].second = lru.begin();
}

/**
 * Function: Delete -  Deletes the least recently used DataEntity
 * @ param value - none
 * @ return type - void
 */
void LRUCache::Delete() {
    cache.erase(lru.back());
    lru.pop_back();
}

/**
 * Function: HandleCacheMiss - Handles cache miss
 * @ param value - DataEntity
 * @ return type - void
 */
void LRUCache::HandleCacheMiss(const DataEntity& entity){
    // check cache size
    if(lru.size() >= maxCacheCapacity){
        Delete();
    }
    lru.push_front(entity.entity_id);
    cache[entity.entity_id] = {entity, lru.begin()};
}