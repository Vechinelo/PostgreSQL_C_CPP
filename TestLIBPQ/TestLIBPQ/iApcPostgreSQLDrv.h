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
  //  virtual int executeWithParams();
    virtual int startTransaction(
        );
    virtual int commit(
        );
      
    
    PGconn* conn = NULL;
  //  std::unique_ptr<PGconn> connec(new );
    
    
};
