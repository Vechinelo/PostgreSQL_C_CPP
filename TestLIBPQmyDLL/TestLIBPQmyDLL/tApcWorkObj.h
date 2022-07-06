#pragma once
#include <string>
#include <iostream>
#include <fstream>

const std::string logfilename = "logfile.txt";
static std::ofstream fout;
const std::string strConnect = "host=localhost port=5432 dbname=tesdb user=postgres password=1234567890";
const int nTest = 1000;

class tApcWorkObj
{
public:
    tApcWorkObj();
    ~tApcWorkObj();

    int start();

protected:

    int write(int& anRes);
    int read(int& anRes);
    int writeBlob(int& anRes);
    int readBlob(int& anRes);

    int create_test();
    int checkDB_test();
    int rollback_test();

};

