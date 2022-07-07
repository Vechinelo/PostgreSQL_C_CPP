#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>

#include "tApcPostgreSQLDrv.h"
#include <libpq-fe.h>
#include <string>
#include <iostream>



tApcPostgreSQLDrv::tApcPostgreSQLDrv()
{

}
//-----------------------------------------------------------------------------

tApcPostgreSQLDrv::~tApcPostgreSQLDrv()
{
    if (PQstatus(conn.smartptr) == CONNECTION_OK)
    {
        disconnect();

       // std::cout << "DESTRUCTOR" << std::endl;
    }

}

int tApcPostgreSQLDrv::createDB(
    const std::string& astrConnectionString,
    const std::string& astrCreateDBString)
{
    pgconn_smartptr tmp_conn = pgconn_smartptr(PQconnectdb(astrConnectionString.c_str()));
    //  PGconn* tmp_conn = PQconnectdb(astrConnectionString.c_str());
    if (PQstatus(tmp_conn.smartptr) != CONNECTION_OK)
    {
        PQfinish(tmp_conn.smartptr);
        return 1;
    }

    pgresult_smartptr res = pgresult_smartptr(PQexec(tmp_conn.smartptr, astrCreateDBString.c_str()));

    //PGresult* res = PQexec(tmp_conn.smartptr, astrCreateDBString.c_str());
    if (PQresultStatus(res.smartptr) != PGRES_COMMAND_OK) {


        return 1;
    }

    // PQfinish(tmp_conn.smartptr);

    return 0;


}

int tApcPostgreSQLDrv::connect(const std::string& strConnect)
{
    conn.smartptr = PQconnectdb(strConnect.c_str());
    if (PQstatus(conn.smartptr) == CONNECTION_BAD)
    {
        PQfinish(conn.smartptr);
        return 1;
    }


    return 0;

}
int tApcPostgreSQLDrv::checkDBVersion()
{
    const std::string strSQL = "SHOW server_version";
    //PGresult* res = PQexec(conn.smartptr, strSQL.c_str());
    pgresult_smartptr res = pgresult_smartptr(PQexec(conn.smartptr, strSQL.c_str()));
    if (PQresultStatus(res.smartptr) != PGRES_TUPLES_OK)
    {

        return 1;
    }

    if (strVersion == PQgetvalue(res.smartptr, 0, 0)) {

        return 0;
    }

    return 1;
}
int tApcPostgreSQLDrv::disconnect()
{
    PQfinish(conn.smartptr); //nullptr is no problem
    return 0;
}

int tApcPostgreSQLDrv::startTransaction()
{
    // PGresult* res = PQexec(conn.smartptr, "BEGIN");
    pgresult_smartptr res = pgresult_smartptr(PQexec(conn.smartptr, "BEGIN"));
    if (PQresultStatus(res.smartptr) != PGRES_COMMAND_OK)
    {

        //   PQclear(res);
        return 1;
    }
    //  PQclear(res);
    return 0;
}

int tApcPostgreSQLDrv::commit()
{
    // PGresult* res = PQexec(conn.smartptr, "COMMIT");
    pgresult_smartptr res = pgresult_smartptr(PQexec(conn.smartptr, "COMMIT"));
    if (PQresultStatus(res.smartptr) != PGRES_COMMAND_OK)
    {

        //   PQclear(res);
        return 1;
    }
    //  PQclear(res);
    return 0;
}

int tApcPostgreSQLDrv::rollback()
{
    //PGresult* res = PQexec(conn.smartptr, "ROLLBACK");
    pgresult_smartptr res = pgresult_smartptr(PQexec(conn.smartptr, "ROLLBACK"));
    if (PQresultStatus(res.smartptr) != PGRES_COMMAND_OK)
    {

        // PQclear(res);
        return 1;
    }
    //  PQclear(res);
    return 0;
}

int tApcPostgreSQLDrv::executeSQL(const std::string& astrSQL)
{
    //PGresult* res = PQexec(conn.smartptr, astrSQL.c_str());
    pgresult_smartptr res = pgresult_smartptr(PQexec(conn.smartptr, astrSQL.c_str()));
    if (PQresultStatus(res.smartptr) != PGRES_COMMAND_OK) {

        //   PQclear(res);
        //  std:: cout << "Select failed: " << PQresultErrorMessage(res) << std::endl;
        return 1;
    }
    // PQclear(res);
    return 0;
}
int tApcPostgreSQLDrv::executeWithParams(
    const std::string& astrSQL,
    int nParams,
    const char* const* paramValues,
    const int* paramLengths,
    const int* paramFormats,
    int resultFormat)
{
    /* PGresult* res = PQexecParams(conn.smartptr, astrSQL.c_str(),
         nParams,              // params num
         NULL,           // let the backend deduce param type
         paramValues,
         paramLengths,
         paramFormats,
         resultFormat);*/
    pgresult_smartptr res = pgresult_smartptr(PQexecParams(conn.smartptr, astrSQL.c_str(),
        nParams,              // params num 
        NULL,           // let the backend deduce param type 
        paramValues,
        paramLengths,
        paramFormats,
        resultFormat));
    if (PQresultStatus(res.smartptr) != PGRES_COMMAND_OK) {

        //PQclear(res);
        //  std:: cout << "Select failed: " << PQresultErrorMessage(res) << std::endl;
        return 1;
    }
    //PQclear(res);
    return 0;

}


int tApcPostgreSQLDrv::openQuery(const std::string& astrSQL)
{
    if (!PQsendQuery(conn.smartptr, astrSQL.c_str())) {
        return 1;
    }

    if (!PQsetSingleRowMode(conn.smartptr)) {
        return 1;
    }
    //PGresult* res = PQgetResult(conn.smartptr);
    pgresult_smartptr res(PQgetResult(conn.smartptr));
    while (res.smartptr != nullptr)
    {
        if (PQresultStatus(res.smartptr) == PGRES_FATAL_ERROR) {
            return 1;
        }
        /*    for (int i = 0; i < PQntuples(res); i++) {
                for (int j = 0; j < PQnfields(res); j++) {
                    std::cout << PQgetvalue(res, i, j) << "   ";
                }
                std::cout << std::endl;
            }*/
        PQclear(res.smartptr);
        res.smartptr = PQgetResult(conn.smartptr);

    }

    //PQclear(res);
    return 0;

}

