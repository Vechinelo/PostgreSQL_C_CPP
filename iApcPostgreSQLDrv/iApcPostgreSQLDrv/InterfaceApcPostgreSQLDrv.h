#pragma once
//
#include <string>




class InterfaceApcPostgreSQLDrv
{

public:
    static __declspec(dllexport) InterfaceApcPostgreSQLDrv* createInstance();
    //static __declspec(dllexport) InterfaceApcPostgreSQLDrv* create();

    virtual int createDB(
        const std::string& astrConnectionString,
        const std::string& astrCreateDBString)=0;
    virtual int connect(const std::string& strConnect)=0;
    virtual int checkDBVersion()=0;
    virtual int disconnect()=0;
    virtual int executeSQL(
        const std::string& astrSQL)=0;
    virtual int openQuery(const std::string& astrSQL)=0;
    virtual int executeWithParams(
        const std::string& astrSQL,
        int nParams,
        const char* const* paramValues,
        const int* paramLengths,
        const int* paramFormats,
        int resultFormat)=0;
    virtual int startTransaction(
    )=0;
    virtual int commit(
    )=0;
    virtual int rollback(
    )=0;

    const std::string strVersion = "14.3";
};

