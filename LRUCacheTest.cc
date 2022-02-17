#include "LRUCacheTest.h"

/**
 * Function: testExecuterUtil - Helper function to create test data
 */
std::vector<DataEntity> LRUCacheTest::testExecuterUtil(int cacheSize, std::vector<std::vector<std::string>> testData) {
    std::vector<DataEntity> result;
    LRUCache* cache = new LRUCache(cacheSize);

    for (std::vector<std::string> currentTest :testData) {
        // For Get scenario
        if (currentTest.size() == 2 && currentTest[0] == "GET") {
            std::string entity_id = currentTest[1];
            DataEntity actualOutcome = cache->Get(entity_id);
            result.push_back(actualOutcome);
        } // For Put scenario
        else if (currentTest.size() == 3 && currentTest[0] == "PUT") {
            cache->Put(DataEntity(currentTest[1], currentTest[2]));
            result.push_back(emptyDataEntity);
        } else {
            throw std::invalid_argument( "Invalid test case" );
        }
    }
    return result;
}

/**
 * When 
 *  cache size equals to 1
 *  UUID1 Put -> Adds to cache and db
 * Then 
 *  UUID1 Get -> returns from cache
 */
void LRUCacheTest::testPut1Get1FromCache(){
    std::cout<< "Running test 1" << std::endl;
    std::vector<std::vector<std::string>> testData1 = {
        {"PUT", "UUID1", "Jon"},
        {"GET", "UUID1"}
    };
    std::vector<DataEntity> result = testExecuterUtil(1, testData1);
    assert(2 == result.size());
    
    assert(true == emptyDataEntity.checkEqual(result[0]));
    DataEntity expected = DataEntity("UUID1", "Jon");
    assert(true == expected.checkEqual(result[1]));
}

/**
 * When 
 *  cache size is 1
 *  UUID1 Put -> Adds to cache and db
 *  UUID2 Put -> UUID1 is removed from cache UUID2 is added to cache and db
 * Then 
 *  UUID1 Get -> returns from db
 */

void LRUCacheTest::testPut2Get1FromDB(){
    std::vector<std::vector<std::string>> testData2 = {
        {"PUT", "UUID1", "Jon"},
        {"PUT", "UUID2", "Alice"},
        {"GET", "UUID1"}
    };
    std::cout<< "Running test 2" << std::endl;
    std::vector<DataEntity> result = testExecuterUtil(1, testData2);
    assert(3 == result.size());
    
    assert(true == emptyDataEntity.checkEqual(result[0]));
    assert(true == emptyDataEntity.checkEqual(result[1]));
    
    DataEntity expected = DataEntity("UUID1", "Jon");
    assert(true == expected.checkEqual(result[2]));
}

/**
 * When
 *  cache size equals to 2
 *  UUID1 Put -> Adds to cache and db
 * Then
 *  UUID1 Get -> returns from cache with version 1
 * When
 *  UUID1 Put ->Updates cache value, version  and db
 * Then 
 *  UUID1 Get -> returns from cache with version 2
 * 
 * 
std::vector<std::vector<std::string>> testData3 = {
    {"PUT", "UUID1", "Jon"},
    {"GET", "UUID1"},
    {"PUT", "UUID2", "Alice"},
    {"GET", "UUID1"}
};
 */
void LRUCacheTest::testPut2Get2FromDB() {
    std::cout<< "Running test 3" << std::endl;
    LRUCache* cache = new LRUCache(2);
    std::string entity_id = "UUID1";
    cache->Put(DataEntity(entity_id, "Jon"));

    DataEntity existingObject = cache->Get(entity_id);
    existingObject.entity_name = "Alice";
    assert(1 == existingObject.version);

    cache->Put(existingObject);
    DataEntity mutatedObject = cache->Get(entity_id);
    assert(2 == mutatedObject.version);
}

void LRUCacheTest::testRunner() {
    testPut1Get1FromCache();
    testPut2Get1FromDB();
    testPut2Get2FromDB();
}