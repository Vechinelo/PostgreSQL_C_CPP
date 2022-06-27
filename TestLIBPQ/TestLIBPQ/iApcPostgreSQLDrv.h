#pragma once
#include <string>
#include <memory>
#include <libpq-fe.h>
class iApcPostgreSQLDrv
{
public:
    iApcPostgreSQLDrv();
    ~iApcPostgreSQLDrv();


    virtual int connect(const std::string& strConnect);
    virtual int disconnect();
    virtual int executeSQL(
        const std::string& astrSQL);
    virtual int openQuery(const std::string& astrSQL);
    virtual int executeWithParams(
        const std::string& astrSQL, 
        int nParams,
        const char* const* paramValues,
        const int* paramLengths,
        const int* paramFormats,
        int resultFormat);
    virtual int startTransaction(
        );
    virtual int commit(
        );
    virtual int rollback(
        );
      
    
    PGconn* conn = NULL;
 
    
    
};

