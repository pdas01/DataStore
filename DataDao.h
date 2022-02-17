#ifndef DATADAO_H
#define DATADAO_H

#include <iostream>
#include <vector>
#include <sqlite3.h> 

// Holds meta-data to be stored in DB
struct DataEntity
{
   std::string entity_id;
   std::string entity_name;
   int version;
   
   DataEntity(){}
   DataEntity(std::string key, std::string value)
   : entity_id(key), entity_name(value), version(0){};

    bool checkEqual(const DataEntity& e){
        return e.entity_id.compare(entity_id) == 0 && 
            e.entity_name.compare(entity_name) == 0;
    }
};

#define CHECK_STATUS(rc)                                                         \
   {                                                                             \
      if( rc ) {                                                                 \
      std::cout << "Can't open database:" << sqlite3_errmsg(db)<< std::endl;     \
      exit (1);                                                                  \
      } else {                                                                   \
      std::cout << "Opened database successfully" << std::endl;                  \
      }                                                                          \
   } 

#define CHECK_STATUS_EXEC(rc, err_msg){                  \
       if (rc != SQLITE_OK ) {                           \
        fprintf(stderr, "SQL error: %s\n", err_msg);     \
        sqlite3_free(err_msg);                           \
        sqlite3_close(db);                               \
        exit (1);                                        \
      }                                                  \
}                                                        \


class DataDao
{
public:
    static DataDao* GetInstance();
    void Init();
    DataEntity GetValue(std::string entity_id);
    void InsertData(const DataEntity& entity);
    ~DataDao();
    
private:
    void CreateTable();
    void DeleteTable();
    // Private constructor so no objects can be created
    DataDao();
    DataDao(DataDao const& copy);            // Not Implemented
    DataDao& operator=(DataDao const& copy); // Not Implemented

    static DataDao *instance;

    sqlite3 *db;
    char *err_msg = 0;
    int rc;
};


#endif
