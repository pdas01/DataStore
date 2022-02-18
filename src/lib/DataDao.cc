#include "DataDao.h"
     
DataDao::DataDao(){
    Init();
    CreateTable();
}   

DataDao::~DataDao(){
    DeleteTable();
    sqlite3_close(db);
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

/**
 * Function: Init -  set a conection to database object
 * @ return type-  void 
 */
void DataDao::Init(){
    rc = sqlite3_open("cisco.db", &db);
    CHECK_STATUS(rc);
}

/**
 * Function: CreateTable - Create database table
 */
void DataDao::CreateTable()
{
    char const *sql= " CREATE TABLE IF NOT EXISTS CISCO (                \
                ENTITY_ID        TEXT      PRIMARY KEY      NOT NULL,     \
                ENTITY_NAME      TEXT      NOT NULL ,                     \
                MODIFY_TIME      TEXT      NOT NULL ,                      \
                VERSION          INT      NOT NULL                        \
                );";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    CHECK_STATUS_EXEC(rc,err_msg);
}

/**
 * Function: DeleteTable - Delete database table
 */
void DataDao::DeleteTable()
{
    char const *sql= " DROP TABLE CISCO;";
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    CHECK_STATUS_EXEC(rc,err_msg);
}

/**
 * Function: GetValue - function fetches value from db
 * @ param value - entity_id
 * @ return type- DataEntity 
 */
DataEntity DataDao::GetValue(std::string entity_id){
    DataEntity result(entity_id, "");
    sqlite3_stmt *res;
    char const *sql = "SELECT * FROM CISCO WHERE ENTITY_ID = ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    CHECK_STATUS_EXEC(rc, err_msg);
    sqlite3_bind_text(res, 1, entity_id.c_str(), strlen(entity_id.c_str()), NULL);
    int step = sqlite3_step(res);
    if (step == SQLITE_ROW) {
        std::string entity_name_result  = std::string(reinterpret_cast<const char*>
                                (sqlite3_column_text(res, 1)));

        int version = sqlite3_column_double(res, 3);

        result.entity_name = entity_name_result;
        result.version = version;
    }
    sqlite3_finalize(res);
    return result;
}

/**
 * Function: InsertData - Insert data to db
 * @ param value - DataEntity
 */
void DataDao::InsertData(const DataEntity& entity){
    sqlite3_stmt *res;
    
    char const *sql = "INSERT INTO CISCO (ENTITY_ID, ENTITY_NAME, MODIFY_TIME, VERSION) VALUES ( ?,?, datetime('now'), ?);";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    CHECK_STATUS_EXEC(rc, err_msg);
    sqlite3_bind_text(res, 1, entity.entity_id.c_str(), strlen(entity.entity_id.c_str()), NULL);
    sqlite3_bind_text(res, 2, entity.entity_name.c_str(), strlen(entity.entity_name.c_str()), NULL);
    sqlite3_bind_int(res, 3, entity.version);

    sqlite3_step(res);
    sqlite3_finalize(res);
}


DataDao *DataDao::instance = 0;
DataDao* DataDao::GetInstance(){
    if(!instance)
        instance = new DataDao();

    return instance;
        
}