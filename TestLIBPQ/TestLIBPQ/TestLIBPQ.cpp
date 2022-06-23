
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

    int tMode = 3; //0-4 to select test, other to clean table
    int nTest = 3000;
    std::string logf = "logfile" + std::to_string(tMode) + ".txt";
    std::ofstream fout(logf);
    iApcPostgreSQLDrv* spDrv = new iApcPostgreSQLDrv;
    //int nRes = 0;

    switch (tMode){
        /* 3000 connections with 3000 INSERTs */
    case 0: { 
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 1; i <= nTest; i++) {
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
        fout << "Average time per request:" << duration.count() / nTest << " s" << std::endl;
        fout.close();
        break;
    }
    
          /* 3000 INSERTs */
    case 1: {
        int nRes = spDrv->connect("host=localhost port=5432 dbname=tesdb user=postgres password=1234567890");
        if (nRes) {
            fout << "error in connect" << std::endl;
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
        spDrv->disconnect();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;

        fout << "Duration of 3000 INSERT:" << duration.count() << " s" << std::endl;
        fout << "Average time per request:" << duration.count() / nTest << " s" << std::endl;
        fout.close();
        break;
    }
          /* 3000 INSERTs with BEGIN/COMMIT */
    case 2: {
        int nRes = spDrv->connect("host=localhost port=5432 dbname=tesdb user=postgres password=1234567890");
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
        std::chrono::duration<float> duration = end - start;

        fout << "Duration of 3000 INSERT with BEGIN/COMMIT:" << duration.count() << " s" << std::endl;
        fout << "Average time per request:" << duration.count() / nTest << " s" << std::endl;
        fout.close();
        break;
    }
          /* 3000 SELECT * FROM table with 3000 tuples */
    case 3: {
        int nRes = spDrv->connect("host=localhost port=5432 dbname=tesdb user=postgres password=1234567890");
        if (nRes) {
            fout << "error in connect" << std::endl;
            return 1;
        }
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 1; i <= nTest; i++) {

            std::string strSQL = "SELECT * FROM public.customers";
            nRes = spDrv->openQuery(strSQL);
            if (nRes) {
                fout << "error in openQuery" << std::endl;
                return 1;
            }

        }
        spDrv->disconnect();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;

        fout << "Duration of 3000 SELECT * FROM table with 3000 tuples:" << duration.count() << " s" << std::endl;
        fout << "Average time per request:" << duration.count() / nTest << " s" << std::endl;
        fout.close();
        break;
    }

       /*   CLEANING TABLE */
    default: {
        int nRes = spDrv->connect("host=localhost port=5432 dbname=tesdb user=postgres password=1234567890");
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
        break;
    }
        
    }

}

