#pragma once

#include <string>
#include <iostream>
#include <pqxx\pqxx>
using namespace std;

int main()
{
   string conn = "host=localhost port=5432 dbname=tesdb user=postgres password =1234567890";

    try
    {
        pqxx::connection connectionObject(conn.c_str());

        pqxx::work worker(connectionObject);

        pqxx::result res = worker.exec("SELECT * FROM public.customers");

        for (size_t i = 0; i < res.size(); i++)
        {
            cout << "Id: " << res[i][0] << " Full name: " << res[i][1] << " Email: " << res[i][2]  << endl;
        }
        

        worker.commit();

    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }

    system("pause");
}