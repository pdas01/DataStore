# DataStore

### Goal: 
To implement a local storage class which writes to database(`sqlite3`) supplemented by in-memory LRU cache.

### Class:
- `DataDao`: Handles database read/write
- `LRUCache`: Handles read/write, cache miss and cache hit
- `LRUCacheTest`: For integration testing

## Build and run executable
### Prerequistes:
- Install sqlite3
- g++ compiler
### Run
- Clone repository
- cd `<cloned repository>`
- `make`  
- `./bin/data-store`