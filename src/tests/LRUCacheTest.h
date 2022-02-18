#ifndef LRUCACHETEST_H
#define LRUCACHETEST_H

#include <assert.h>
#include <string>

#include "../lib/LRUCache.h"

class LRUCacheTest {
public:
    LRUCacheTest(){}
    ~LRUCacheTest() {}
    void testRunner();

private:
    void testPut1Get1FromCache();
    void testPut2Get1FromDB();
    void testPut2Get2FromDB();
    std::vector<DataEntity> testExecuterUtil(int cacheSize, std::vector<std::vector<std::string>> testData);
    DataEntity emptyDataEntity;
};
#endif