
#include <iostream>
#include "iApcPostgreSQLDrv.h"
#include <string>
#include <chrono>
#include<fstream>

int main()
{
    /* iApcPostgreSQLDrv spDrv = iApcPostgreSQLDrv();

    

    if (spDrv.connect("host=localhost port=5432 dbname=tesdb user=postgres password=1234567890") == 1) std::cout << "OK" << std::endl;

   // spDrv.disconnect();
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 5; i < 1000; i++) {
       // spDrv.connect("host=localhost port=5432 dbname=tesdb user=postgres password=1234567890");
     //   spDrv.startTransaction();
        std::string strSQL = "INSERT INTO public.customers VALUES (" + std::to_string(i) + ", 'some name', 'some e-mail')";
      
        spDrv.executeSQL(strSQL);
      //  spDrv.commit();
       // spDrv.disconnect();
    }
    spDrv.disconnect();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;*/

    std::ofstream fout("logfile.txt");
    iApcPostgreSQLDrv* spDrv = new iApcPostgreSQLDrv;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 1; i < 3000; i++) {
        int nRes = spDrv->connect("host=localhost port=5432 dbname=tesdb user=postgres password=1234567890");
        if (nRes) {
            fout << "error in connect" << std::endl;
            return 1;
        }
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
        spDrv->disconnect();

    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;

    fout << "Duration of 3000 sessions with the db with 1 commit in each:" << duration.count() << " s" << std::endl;
}

