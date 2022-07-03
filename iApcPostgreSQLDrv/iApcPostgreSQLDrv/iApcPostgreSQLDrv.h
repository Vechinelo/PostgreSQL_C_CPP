#pragma once



#include "InterfaceApcPostgreSQLDrv.h"
#include <string>
#include <memory>
#include <libpq-fe.h>

class __declspec(dllexport) pgconn_smartptr {

public:

    pgconn_smartptr(PGconn* conn) : smartptr(conn)
    {
    }
    pgconn_smartptr() :smartptr(nullptr)
    {
    }

    ~pgconn_smartptr()
    {

        if (PQstatus(smartptr) == CONNECTION_OK) PQfinish(smartptr);
    }

    PGconn* smartptr = nullptr;
};

class __declspec(dllexport) pgresult_smartptr {

public:

    pgresult_smartptr(PGresult* res) :smartptr(res)
    {
    }

    ~pgresult_smartptr()
    {
        PQclear(smartptr);
    }

    PGresult* smartptr = nullptr;
};

static class __declspec(dllexport) iApcPostgreSQLDrv : public InterfaceApcPostgreSQLDrv
{
public:
    iApcPostgreSQLDrv();
    ~iApcPostgreSQLDrv();

    int createDB(
        const std::string& astrConnectionString,
        const std::string& astrCreateDBString);
    int connect(const std::string& strConnect);
    int checkDBVersion();
    int disconnect();
    int executeSQL(
        const std::string& astrSQL);
    int openQuery(const std::string& astrSQL);
    int executeWithParams(
        const std::string& astrSQL,
        int nParams,
        const char* const* paramValues,
        const int* paramLengths,
        const int* paramFormats,
        int resultFormat);
    int startTransaction(
    );
    int commit(
    );
    int rollback(
    );


    //  PGconn* conn = NULL;
    pgconn_smartptr conn = pgconn_smartptr();
    



};

