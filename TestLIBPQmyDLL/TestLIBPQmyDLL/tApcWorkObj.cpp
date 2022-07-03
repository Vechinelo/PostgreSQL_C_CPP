#define _CRT_SECURE_NO_WARNINGS
#include "tApcWorkObj.h"

#include <iostream>
#include "InterfaceApcPostgreSQLDrv.h"
#include <string>
#include <chrono>
#include <memory>
#include<fstream>

tApcWorkObj::~tApcWorkObj()
{
    fout.close();

}

tApcWorkObj::tApcWorkObj()
{
    fout.open(logfilename);

}


int tApcWorkObj::start()
{
    int nResWrite = 0, nResWriteBlob = 0;
    int nResRead = 0, nResReadBlob = 0;


    //10 раз повторяем тесты
    int nNum = 10;

    if (create_test())
    {
        std::cout << "ERROR IN CREATE TEST" << std::endl;
        return 1;
    }
    if (checkDB_test()) {
        std::cout << "ERROR IN CHECK TEST" << std::endl;
        return 1;
    }
    if (rollback_test()) {
        std::cout << "ERROR IN ROLLBACK TEST" << std::endl;
        return 1;
    }

    int nTime = 0;
    for (int i = 0; i < nNum; i++)
    {



        fout << "-----------------------------------------------------------------" << std::endl;
        fout << "-----------------------------" << i + 1 << "-----------------------------------" << std::endl << std::endl;

        if (int nRes = write(nTime))
        {

            fout << "ERROR IN WRITE" << std::endl;
            return nRes;
        }
        nResWrite += nTime;
        fout << std::endl << "-----------------------------------------------------------------" << std::endl << std::endl;

        std::cout << "Write done..." << std::endl;

        if (int nRes = writeBlob(nTime))
        {
            fout << "ERROR IN WRITE BLOB" << std::endl;
            return nRes;
        }
        nResWriteBlob += nTime;
        fout << std::endl << "-----------------------------------------------------------------" << std::endl << std::endl;

        std::cout << "WriteBlob done..." << std::endl;

        if (int nRes = read(nTime))
        {
            fout << "ERROR IN READ" << std::endl;
            return nRes;
        }
        nResRead += nTime;

        fout << std::endl << "-----------------------------------------------------------------" << std::endl << std::endl;

        std::cout << "Read done..." << std::endl;

        if (int nRes = readBlob(nTime))
        {
            fout << "ERROR IN READ BLOB" << std::endl;
            return nRes;
        }
        nResReadBlob += nTime;

        fout << std::endl << "-----------------------------------------------------------------" << std::endl;

        std::cout << "ReadBlob done..." << std::endl;
        std::cout << "STAGE " << i + 1 << "/" << nNum << std::endl;
    }

    nResWrite /= nNum;
    fout << "*****************************************************************" << std::endl;
    fout << "AVERAGE VALUE FOR WRITE: " + std::to_string(nResWrite) << " ms" << std::endl;
    fout << std::endl;

    nResWriteBlob /= nNum;
    fout << "AVERAGE VALUE FOR WRITE BLOB: " + std::to_string(nResWriteBlob) << " ms" << std::endl;
    fout << std::endl;

    nResRead /= nNum;

    fout << "AVERAGE VALUE FOR READ: " + std::to_string(nResRead) << " ms" << std::endl;
    fout << std::endl;

    nResReadBlob /= nNum;
    fout << "AVERAGE VALUE FOR READ BLOB: " + std::to_string(nResReadBlob) << " ms" << std::endl;
    fout << std::endl;
    fout << "*****************************************************************" << std::endl;



    return 0;
}

int tApcWorkObj::write(int& anRes)
{
    // iApcPostgreSQLDrv* spDrv = new iApcPostgreSQLDrv;
    //std::unique_ptr<InterfaceApcPostgreSQLDrv> spDrv = InterfaceApcPostgreSQLDrv::createInstance();
    InterfaceApcPostgreSQLDrv* spDrv = InterfaceApcPostgreSQLDrv::createInstance();
    int nRes = spDrv->connect(strConnect);
    if (nRes) {
        fout << "error in connect" << std::endl;
        return 1;
    }

    std::string strSQL = "DELETE FROM public.customers";
    nRes = spDrv->executeSQL(strSQL);
    if (nRes) {
        fout << "error in execute" << std::endl;
        return 1;
    }

    strSQL = "DELETE FROM public.ch";
    nRes = spDrv->executeSQL(strSQL);
    if (nRes) {
        fout << "error in execute" << std::endl;
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 1; i <= nTest; i++) {

        std::string strSQL = "INSERT INTO public.customers VALUES (" + std::to_string(i) + ", 'some name', 'some e-mail')";
        nRes = spDrv->executeSQL(strSQL);
        if (nRes) {
            fout << "error in execute" << std::endl;
            return 1;
        }

    }
    auto end = std::chrono::high_resolution_clock::now();
    spDrv->disconnect();

    //std::chrono::duration<float> duration = end - start;
    std::chrono::duration<double, std::milli> duration = end - start;

    fout << "Duration of 30000 INSERT:" << duration.count() << " ms" << std::endl;
    fout << "Average time per request:" << duration.count() / nTest << " ms" << std::endl;
    anRes = duration.count();


    return 0;
}

int tApcWorkObj::read(int& anRes)
{
    //std::unique_ptr<InterfaceApcPostgreSQLDrv> spDrv = InterfaceApcPostgreSQLDrv::createInstance();
    InterfaceApcPostgreSQLDrv* spDrv = InterfaceApcPostgreSQLDrv::createInstance();
    int nRes = spDrv->connect(strConnect);
    if (nRes) {
        fout << "error in connect" << std::endl;
        return 1;
    }
    auto start = std::chrono::high_resolution_clock::now();


    std::string strSQL = "SELECT * FROM public.customers";
    nRes = spDrv->openQuery(strSQL);
    if (nRes) {
        fout << "error in openQuery" << std::endl;
        return 1;
    }


    auto end = std::chrono::high_resolution_clock::now();
    spDrv->disconnect();

    // std::chrono::duration<float> duration = end - start;
    std::chrono::duration<double, std::milli> duration = end - start;

    fout << "Duration of  SELECT * FROM table with 30000 tuples:" << duration.count() << " ms" << std::endl;
    fout << "Average time per tuple:" << duration.count() / nTest << " ms" << std::endl;
    anRes = duration.count();

    return 0;
}

int tApcWorkObj::writeBlob(int& anRes)
{
    FILE* file = fopen("testblob.jpg", "rb");
    if (file == NULL) {
        std::cout << std::endl << "FILE WAS UNABLE TO BE READED" << std::endl;
        std::cout << strerror(errno) << std::endl;
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long int fileSize = ftell(file);

    rewind(file);

    unsigned char* buffRead = (unsigned char*)malloc(fileSize);
    size_t bytes_read = fread(buffRead, 1, fileSize, file);
    if (bytes_read != fileSize) std::cout << std::endl << "fread reading Error";
    fclose(file);

    // size_t* sizeP = new size_t(fileSize);

    const char* paramValues[2];
    paramValues[0] = "testblob";
    paramValues[1] = reinterpret_cast<const char*>(buffRead);   // type cast required to PQexeParams

    const int paramFormats[2]{ 0,1 };   //0 = text, 1 = binary
    const int paramLenghts[2]{ strlen(paramValues[0]), fileSize };

   // std::unique_ptr<iApcPostgreSQLDrv> spDrv(new iApcPostgreSQLDrv);
    //std::unique_ptr<InterfaceApcPostgreSQLDrv> spDrv = InterfaceApcPostgreSQLDrv::createInstance();
    std::unique_ptr<InterfaceApcPostgreSQLDrv> spDrv(InterfaceApcPostgreSQLDrv::createInstance());
    //InterfaceApcPostgreSQLDrv* spDrv = InterfaceApcPostgreSQLDrv::createInstance();
    int nRes = spDrv->connect(strConnect);
    if (nRes) {
        fout << "error in connect" << std::endl;
        return 1;
    }

    std::string strSQL = "DELETE FROM public.ch";
    nRes = spDrv->executeSQL(strSQL);
    if (nRes) {
        fout << "error in execute (delete)" << std::endl;
        return 1;
    }
    strSQL = "INSERT INTO public.ch (fname, bfile) VALUES($1::text, $2::bytea)";

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 1; i <= nTest; i++) {

        /* strSQL = "INSERT INTO public.ch VALUES (" + std::to_string(i) + ", '11111111')";
         nRes = spDrv->executeSQL(strSQL);
         if (nRes) {
             fout << "error in execute" << std::endl;
             return 1;
         }*/


        nRes = spDrv->executeWithParams(strSQL,
            2,              /* params num */
            paramValues,
            paramLenghts,
            paramFormats,
            1);
        if (nRes) {
            fout << "error in executeWithParams" << std::endl;
            return 1;
        }

    }
    auto end = std::chrono::high_resolution_clock::now();
    spDrv->disconnect();

    //std::chrono::duration<float> duration = end - start;
    std::chrono::duration<double, std::milli> duration = end - start;

    fout << "Duration of 30000 INSERT bytea:" << duration.count() << " ms" << std::endl;
    fout << "Average time per request:" << duration.count() / nTest << " ms" << std::endl;

    anRes = duration.count();

    free(buffRead);

    return 0;

}

int tApcWorkObj::readBlob(int& anRes)
{
    //std::unique_ptr<InterfaceApcPostgreSQLDrv> spDrv = InterfaceApcPostgreSQLDrv::createInstance();
    std::unique_ptr<InterfaceApcPostgreSQLDrv> spDrv(InterfaceApcPostgreSQLDrv::createInstance());
    int nRes = spDrv->connect(strConnect);
    if (nRes) {
        fout << "error in connect" << std::endl;
        return 1;
    }
    auto start = std::chrono::high_resolution_clock::now();


    std::string strSQL = "SELECT * FROM public.ch";
    nRes = spDrv->openQuery(strSQL);
    if (nRes) {
        fout << "error in openQuery" << std::endl;
        return 1;
    }


    auto end = std::chrono::high_resolution_clock::now();
    spDrv->disconnect();

    // std::chrono::duration<float> duration = end - start;
    std::chrono::duration<double, std::milli> duration = end - start;

    fout << "Duration of SELECT * FROM table with 30000 tuples (bytea):" << duration.count() << " ms" << std::endl;
    fout << "Average time per tuple:" << duration.count() / nTest << " ms" << std::endl;

    anRes = duration.count();

    return 0;

}

int tApcWorkObj::create_test()
{
    //std::unique_ptr<InterfaceApcPostgreSQLDrv> spDrv = InterfaceApcPostgreSQLDrv::createInstance();
    std::unique_ptr<InterfaceApcPostgreSQLDrv> spDrv(InterfaceApcPostgreSQLDrv::createInstance());
    std::string str_conn = "host=localhost port=5432 dbname=template1 user=postgres password=1234567890";
    std::string str_create = "CREATE DATABASE createTest";
    int nRes = spDrv->createDB(str_conn, str_create);
    if (nRes) {
        std::cout << "error in create" << std::endl;
        return 1;
    }

    return 0;
}

int tApcWorkObj::checkDB_test()
{
    //std::unique_ptr<InterfaceApcPostgreSQLDrv> spDrv = InterfaceApcPostgreSQLDrv::createInstance();
    std::unique_ptr<InterfaceApcPostgreSQLDrv> spDrv(InterfaceApcPostgreSQLDrv::createInstance());
    std::string str_conn = "host=localhost port=5432 dbname=tesdb user=postgres password=1234567890";
    int nRes = spDrv->connect(str_conn);
    if (nRes) {
        std::cout << "error in connect" << std::endl;
        return 1;
    }

    nRes = spDrv->checkDBVersion();
    if (nRes == 0) {
        std::cout << "CORRECT VERSION" << std::endl;
        spDrv->disconnect();
        return 0;
    }

    spDrv->disconnect();
    return 1;
}

int tApcWorkObj::rollback_test()
{
    //std::unique_ptr<InterfaceApcPostgreSQLDrv> spDrv = InterfaceApcPostgreSQLDrv::createInstance();
    std::unique_ptr<InterfaceApcPostgreSQLDrv> spDrv(InterfaceApcPostgreSQLDrv::createInstance());
   // InterfaceApcPostgreSQLDrv* sp = InterfaceApcPostgreSQLDrv::createInstance();
    std::string str_conn = "host=localhost port=5432 dbname=tesdb user=postgres password=1234567890";
    int nRes = spDrv->connect(str_conn);
    if (nRes) {
        std::cout << "error in connect" << std::endl;
        return 1;
    }

    nRes = spDrv->startTransaction();
    if (nRes) {
        std::cout << "error in start trabsaction" << std::endl;
        return 1;
    }

    nRes = spDrv->executeSQL("INSERT INTO unitest VALUES('1')");
    if (nRes) {
        std::cout << "error in executeSQL 1" << std::endl;
        return 1;
    }

    nRes = spDrv->executeSQL("INSERT INTO unitest VALUES('2')");
    if (nRes) {
        std::cout << "error in executeSQL 2" << std::endl;
        return 1;
    }

    nRes = spDrv->rollback();
    if (nRes) {
        std::cout << "error in rollback" << std::endl;
        return 1;
    }

    spDrv->disconnect();

    std::cout << "ROLLBACK SUCESS" << std::endl;
    return 0;
}

/*
int tApcWorkObj::writeTransaction(int& anRes)
{
    int nRes = spDrv->connect(strConnect);
    if (nRes) {
        fout << "error in connect" << std::endl;
        return 1;
    }
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 1; i <= nTest; i++) {

        nRes = spDrv->startTransaction();
        if (nRes) {
            fout << "error in startTransaction" << std::endl;
            return 1;
        }
        std::string strSQL = "INSERT INTO public.customers VALUES (" + std::to_string(i) + ", 'some name', 'some e-mail')";
        nRes = spDrv->executeSQL(strSQL);
        if (nRes) {
            fout << "error in execute" << std::endl;
            return 1;
        }
        nRes = spDrv->commit();
        if (nRes) {
            fout << "error in commit" << std::endl;
            return 1;
        }

    }
    spDrv->disconnect();
    auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<float> duration = end - start;
    std::chrono::duration<double, std::milli> duration = end - start;

    fout << "Duration of 30000 INSERT with BEGIN/COMMIT:" << duration.count() << " ms" << std::endl;
    fout << "Average time per request:" << duration.count() / nTest << " ms" << std::endl;
}






int nRes = spDrv->connect(strConnect);
if (nRes) {
    fout << "error in connect" << std::endl;
    return 1;
}
std::string strSQL = "DELETE FROM public.customers";
nRes = spDrv->executeSQL(strSQL);
if (nRes) {
    fout << "error in execute" << std::endl;
    return 1;
}

strSQL = "DELETE FROM public.ch";
nRes = spDrv->executeSQL(strSQL);
if (nRes) {
    fout << "error in execute" << std::endl;
    return 1;
}*/
