
#include <iostream>
#include <libpq-fe.h>
using namespace std;

int main()
{
    PGconn* conn = PQconnectdb("host=localhost port=5432 dbname=tesdb user=postgres password=1234567890");
    if (PQstatus(conn) == CONNECTION_BAD)
    {
        cout << PQerrorMessage(conn);
        PQfinish(conn);
        return 1; 
    }
    cout << "Connection ok" << endl;
 
    /* SELECT demo */

    PGresult* res = PQexec(conn, "SELECT * FROM public.customers");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
       cout << "Select failed: " << PQresultErrorMessage(res) << endl;
    }
    else {
        cout << "Get " << PQntuples(res) << " tuples, each tuple has "
            << PQnfields(res) << " fields" << endl;
        // print column name
        for (int i = 0; i < PQnfields(res); i++) {
            cout << PQfname(res, i) << "              ";
        }
        cout << endl;
        // print column values
        for (int i = 0; i < PQntuples(res); i++) {
            for (int j = 0; j < PQnfields(res); j++) {
                cout << PQgetvalue(res, i, j) << "   ";
            }
           cout << endl;
        }
    }
    PQclear(res);
    //End connection
    PQfinish(conn);
    cout << "Disconnected" << endl;
}